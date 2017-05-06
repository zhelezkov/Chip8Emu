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
    delete debugger;
    delete cpu;
    
    SDL_DestroyTexture(screenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Emulator::initEmulator() {
    LOG_F(INFO, "Initializing CPU");
    cpu = new CPU();
    resMode = cpu->getGpu().getResolutionMode();
}

bool Emulator::loadRom() {
    const char* path = tinyfd_openFileDialog("Open ROM", "", 0, nullptr, "ROM File", 0);
    if (!path) return false;
    LOG_F(INFO, "Loading ROM: %s", path);
    
    FILE* inFile = fopen(path, "rb");
    byte* buffer = new byte[MEM_SIZE];
    fread(buffer, sizeof(byte), MEM_SIZE, inFile);
    fclose(inFile);
    
    cpu->reset(true);
    
    cpu->getMemory().fillMemory(buffer);
    
    delete[] buffer;
    return true;
}

void Emulator::render() {
    GPU& gpu = cpu->getGpu();
    
    if (resMode != gpu.getResolutionMode()) {
        SDL_DestroyTexture(screenTexture);
        screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GPU_DEFAULT_WIDTH * gpu.getResolutionMode(), GPU_DEFAULT_HEIGHT * gpu.getResolutionMode());
        CHECK_F(screenTexture != nullptr, "Error during creating texture: %s", SDL_GetError());
    }
    
    SDL_SetRenderTarget(renderer, screenTexture);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderClear(renderer);
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
            else if (debuggerConnected && ev.window.windowID == debugger->getWindowID() && ev.type == SDL_WINDOWEVENT)
                debugger->handleWindowEvent(ev);
            
            if (ev.type == SDL_KEYUP && ev.key.keysym.scancode == SDL_SCANCODE_F1) {
                if(loadRom())
                    now = SDL_GetTicks(); //reset ticks to prevent ops overhead
            }
            
            if (hasFocus()) {
                handleKeyEvent(ev);
            } else if (debuggerConnected && debugger->hasFocus()) {
                debugger->handleKeyEvent(ev);
            }
        }
        
        hasAppFocus = hasFocus() || (debuggerConnected && debugger->hasFocus());
        
        if (!hasAppFocus) {
            //when window is not focused, vsync doesn't work, so don't waste cpu time
            SDL_Delay(100);
            continue;
        }
        
        unprocessedTime += elapsedTime;
        while (unprocessedTime > 1) {
            if (!(debuggerConnected && debugger->paused())) {
                cpu->tick();
            }
            ops++;
            unprocessedTime--;
        }
        
        cpu->getTimersManager().update();
        
        render();
        
        if (debuggerConnected) debugger->render();
        
        frames++;
        
        if (SDL_GetTicks() > timer + 1000) {
            LOG_F(INFO, "fps: %d, ops/sec: %d\n", frames, ops);
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
	}
	else if (ev.type == SDL_KEYUP) {
		SDL_Scancode key = ev.key.keysym.scancode;
		if (key == SDL_SCANCODE_G) {
			if (debugger == nullptr) debugger = new Debugger(*this);
			debugger->show();
			debuggerConnected = true;
		}
		if (key == SDL_SCANCODE_R && (ev.key.keysym.mod & KMOD_CTRL)) {
			reloadCurrentROM();
		}

        cpu->getKeyboard().keyUp(key);
    }
}

void Emulator::reloadCurrentROM() {
	cpu->reset(false);
}

void Emulator::disableDebugger() {
    debuggerConnected = false;
}

void Emulator::initWindow() {
    CHECK_F(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Error during SDL initialization: %s", SDL_GetError());
    
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, EMULATOR_WINDOW_WIDTH * EMULATOR_WINDOW_SCALE, EMULATOR_WINDOW_HEIGHT * EMULATOR_WINDOW_SCALE, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    CHECK_F(window != nullptr, "Error during creating window: %s", SDL_GetError());
    
    windowID = SDL_GetWindowID(window);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    CHECK_F(renderer != nullptr, "Error during creating renderer: %s", SDL_GetError());
    
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GPU_DEFAULT_WIDTH, GPU_DEFAULT_HEIGHT);
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
