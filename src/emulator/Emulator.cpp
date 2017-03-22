//
//  Emulator.cpp
//  Chip8Emulator
//

#include <SDL.h>

#include <iostream>
#include "Emulator.hpp"
#include "utils/tinyfiledialogs.h"

//#include "CPU.hpp"
//#include "OpCodes.hpp"

Emulator::Emulator() {
    initWindow();
    initEmulator();
}

Emulator::~Emulator() {
    SDL_DestroyTexture(screenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Emulator::initEmulator() {
    cpu = new CPU();
}

static std::map<SDL_Scancode, byte> keys = {
    {SDL_SCANCODE_X, 0},
    {SDL_SCANCODE_1, 1},
    {SDL_SCANCODE_2, 2},
    {SDL_SCANCODE_3, 3},
    {SDL_SCANCODE_Q, 4},
    {SDL_SCANCODE_W, 5},
    {SDL_SCANCODE_E, 6},
    {SDL_SCANCODE_A, 7},
    {SDL_SCANCODE_S, 8},
    {SDL_SCANCODE_D, 9},
    {SDL_SCANCODE_Z, 0xA},
    {SDL_SCANCODE_C, 0xB},
    {SDL_SCANCODE_4, 0xC},
    {SDL_SCANCODE_R, 0xD},
    {SDL_SCANCODE_F, 0xE},
    {SDL_SCANCODE_V, 0xF}
};

void Emulator::run() {
    //60hz
    double msPerFrame = 1000.0 / 60.0;
    //double lag = 0.0;
    
    Uint32 lastTime = 0;
    Uint32 lastFrameTime = SDL_GetTicks();
    
    bool running = true;

    int ticks = 0;
    int frames = 0;
    
    SDL_Event ev;
    int ops = 0;
    
    while (running) {
        Uint32 now = SDL_GetTicks();
        //double deltaTime = (now - lastTime) / msPerFrame;
        //Uint32 delta = (now - lastTime);
        //lastTime = now;
        //lag += deltaTime;
        
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running = false;
                break;
            } else if (ev.type == SDL_KEYDOWN) {
                SDL_Scancode key = ev.key.keysym.scancode;
                if (key == SDL_SCANCODE_F1) {
                    loadRom();
                }
                byte mappedKey;
                mappedKey = keys[key];
                cpu->getKeyboard().keyDown(mappedKey);
            } else if (ev.type == SDL_KEYUP) {
                SDL_Scancode key = ev.key.keysym.scancode;
                byte mappedKey;
                mappedKey = keys[key];
                cpu->getKeyboard().keyUp(mappedKey);
            }
        }
        
        bool render = false;
        
        //~1000 ops/sec
        for (int i = 0; i < 16; i++) {
            cpu->tick();
            ops++;
        }
        
        cpu->getTimersManager().update();
        
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
        
        render = true;
        
        //frames++;
        
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderClear(renderer);
        
        SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
        
        SDL_RenderPresent(renderer);

        if (SDL_GetTicks() > lastFrameTime + 1000) {
            printf("ops/sec: %d\n", ops);
            lastFrameTime += 1000;
            frames = 0;
            ticks = 0;
            ops = 0;
        }
        
        if ((SDL_GetTicks() - now) < msPerFrame)
            SDL_Delay(msPerFrame - (SDL_GetTicks() - now));
    }
}

bool Emulator::loadRom() {
    const char* path = tinyfd_openFileDialog("Open ROM", "", 0, nullptr, "ROM File", 0);
    if (!path) return false;
    FILE* inFile = fopen(path, "rb");
    byte* buffer = new byte[MEM_SIZE];
    fread(buffer, sizeof(byte), MEM_SIZE, inFile);
    fclose(inFile);
    
    cpu->reset();
    cpu->getMemory().fillMemory(buffer);
    
    delete[] buffer;
    return true;
}

void Emulator::initWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Error during initialization: %s", SDL_GetError());
        exit(-1);
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320 * 2, 240 * 2, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Error during creating window: %s", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Error during creating renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(-1);
    }
    
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 32);
    if (!screenTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Error during creating texture: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(-1);
    }
}

int main() {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
    
    Emulator emulator;
    if (emulator.loadRom()) {
        emulator.run();
    }
    
    return 0;
}
