//
//  Emulator.cpp
//  Chip8Emulator
//

#include <SDL.h>
#include <iostream>
#include <tinyfiledialogs.h>
#define LOGURU_IMPLEMENTATION 1
#include <loguru.h>

#include "Emulator.hpp"
#include "CPU.hpp"
#include "GPU.hpp"
#include "Memory.hpp"
#include "TimersManager.hpp"
#include "Keyboard.hpp"

#include "Debugger.hpp"

Emulator::Emulator() {
    initEmulator();
}

Emulator::~Emulator() {
    SDL_DestroyTexture(screenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Emulator::initEmulator() {
    LOG_F(INFO, "Initializing CPU");
    cpu = new CPU();
}

bool Emulator::loadRom() {
    const char* path = tinyfd_openFileDialog("Open ROM", "", 0, nullptr, "ROM File", 0);
    if (!path) return false;
    LOG_F(INFO, "Loading ROM: %s", path);
    
    FILE* inFile = fopen(path, "rb");
    byte* buffer = new byte[MEM_SIZE];
    fread(buffer, sizeof(byte), MEM_SIZE, inFile);
    fclose(inFile);
    
    cpu->reset();
    
    cpu->getMemory().fillMemory(buffer);
    
    delete[] buffer;
    return true;
}

void Emulator::render() {
    SDL_SetRenderTarget(renderer, screenTexture);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderClear(renderer);
    GPU& gpu = cpu->getGpu();
    for (Uint32 x = 0; x < gpu.getWidth(); x++) {
        for (Uint32 y = 0; y < gpu.getHeight(); y++) {
            byte pix = gpu.getPixel(x, y);
            
            if (pix == 1) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            else if (pix == 0) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            else SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void Emulator::swap() {
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderClear(renderer);
    
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
    
    SDL_RenderPresent(renderer);
}

void Emulator::run() {
    if (window == nullptr) initWindow();
    SDL_RaiseWindow(window);
    
    Uint32 timer = SDL_GetTicks();
    bool running = true;

    int ticks = 0;
    int frames = 0;
    int ops = 0;
    
    bool hasAppFocus = true;
    
    Uint32 now;
    Uint32 elapsedTime = 0;
    Uint32 unprocessedTime = 0;
    
    SDL_Event ev;
    while (running) {
        now = SDL_GetTicks();
        
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT || (ev.window.windowID == windowID && ev.window.event == SDL_WINDOWEVENT_CLOSE)) {
                running = false;
                break;
            }
            
            if (ev.window.windowID == windowID && ev.type == SDL_WINDOWEVENT)
                handleWindowEvent(ev);
            else if (hasDebugger && ev.window.windowID == debugger->getWindowID() && ev.type == SDL_WINDOWEVENT)
                debugger->handleWindowEvent(ev);
            
            if (hasFocus()) {
                handleKeyEvent(ev);
            } else if (hasDebugger && debugger->hasFocus()) {
                debugger->handleKeyEvent(ev);
            }
        }
        
        hasAppFocus = hasFocus() || (hasDebugger && debugger->hasFocus());
        
        if (!hasAppFocus) {
            //when window is not focused, vsync doesn't work, so don't waste cpu time
            SDL_Delay(100);
            continue;
        }
        
        unprocessedTime += elapsedTime;
        while (unprocessedTime > 1) {
            cpu->tick();
            ops++;
            unprocessedTime--;
        }
        
        cpu->getTimersManager().update();
        
        render();
        
        frames++;
        
        if (SDL_GetTicks() > timer + 1000) {
            printf("fps: %d, ops/sec: %d\n", frames, ops);
            timer += 1000;
            frames = 0;
            ticks = 0;
            ops = 0;
        }
        
        swap();
        
        elapsedTime = SDL_GetTicks() - now;
    }
}

void Emulator::handleWindowEvent(SDL_Event& ev) {
    if (ev.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        windowFocused = false;
        DLOG_F(INFO, "Emulator window focus lost");
    } else if (ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        windowFocused = true;
        DLOG_F(INFO, "Emulator window focus gained");
    }
}

void Emulator::handleKeyEvent(SDL_Event& ev) {
    if (ev.type == SDL_KEYDOWN) {
        SDL_Scancode key = ev.key.keysym.scancode;
        cpu->getKeyboard().keyDown(key);
    } else if (ev.type == SDL_KEYUP) {
        SDL_Scancode key = ev.key.keysym.scancode;
        if (key == SDL_SCANCODE_F1) {
            loadRom();
        } else if (key == SDL_SCANCODE_G) {
            if (debugger == nullptr) debugger = new Debugger();
            hasDebugger = true;
        }
        cpu->getKeyboard().keyUp(key);
    }
}

void Emulator::initWindow() {
    CHECK_F(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Error during SDL initialization: %s", SDL_GetError());
    
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320 * 2, 240 * 2, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    CHECK_F(window != nullptr, "Error during creating window: %s", SDL_GetError());
    
    windowID = SDL_GetWindowID(window);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    CHECK_F(renderer != nullptr, "Error during creating renderer: %s", SDL_GetError());
    
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 32);
    CHECK_F(screenTexture != nullptr, "Error during creating texture: %s", SDL_GetError());

	LOG_F(INFO, "Window successfully created");
}

int main(int argc, char* argv[]) {
    loguru::init(argc, argv);
    
    loguru::add_file("emulator.log", loguru::Truncate, loguru::Verbosity_MAX);

    Emulator emulator;
    if (emulator.loadRom()) {
        emulator.run();
    }
    
    LOG_F(INFO, "Bye bye");
    
    return 0;
}
