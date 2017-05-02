//
//  Debugger.cpp
//  Chip8
//

#include <SDL.h>

#include "Debugger.hpp"
#include "Emulator.hpp"
#include "CPU.hpp"

#define FONT_WIDTH 5
#define FONT_HEIGHT 7

Debugger::Debugger(Emulator& emulator) : emulator(emulator) {
    LOG_F(INFO, "Initializing debugger");
    int x, y;
    SDL_GetWindowPosition(emulator.window, &x, &y);
    window = SDL_CreateWindow("Debugger", x + 320 * 2, y, 64 * 2, 200 * 2, SDL_WINDOW_OPENGL);
    CHECK_F(window != nullptr, "Error during creating window: %s", SDL_GetError());
    
    windowID = SDL_GetWindowID(window);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    CHECK_F(renderer != nullptr, "Error during creating renderer: %s", SDL_GetError());
    
    renderTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 200);
    CHECK_F(renderTexture != nullptr, "Error during creating texture: %s", SDL_GetError());

    
    LOG_F(INFO, "Debugger window successfully created");
    
    loadFont();
}

Debugger::~Debugger() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Debugger::loadFont() {
    LOG_F(INFO, "Loading font.bmp");
    
    SDL_Surface* surf = SDL_LoadBMP("font.bmp");
    SDL_SetColorKey(surf, 1, SDL_MapRGB(surf->format, 255, 0, 255));
    
    fontTexture = SDL_CreateTextureFromSurface(renderer, surf);
}

void Debugger::show() {
    int x, y;
    SDL_GetWindowPosition(emulator.window, &x, &y);
    SDL_SetWindowPosition(window, x + 320 * 2, y);
    SDL_ShowWindow(window);
}

void Debugger::handleKeyEvent(SDL_Event& ev) {
    
}

void Debugger::handleWindowEvent(SDL_Event& ev) {
    if (ev.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        windowFocused = false;
        DLOG_F(INFO, "Debugger window focus lost");
    } else if (ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        windowFocused = true;
        DLOG_F(INFO, "Debugger window focus gained");
    } else if (ev.window.event == SDL_WINDOWEVENT_CLOSE) {
        emulator.disableDebugger();
        SDL_HideWindow(window);
    }
}

void Debugger::drawText(std::string& str, int x, int y) {
    SDL_Rect src = {0, 0, FONT_WIDTH, FONT_HEIGHT};
    SDL_Rect dst = {x, y, FONT_WIDTH, FONT_HEIGHT};
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c > 32 && c < 127) {
            src.x = (c - 33) * 6;
            SDL_RenderCopy(renderer, fontTexture, &src, &dst);
        }
        dst.x += FONT_WIDTH + 2;
    }
}

void Debugger::render() {
    SDL_SetRenderTarget(renderer, renderTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < V_REGISTERS_COUNT; i++) {
        char cStr[20];
        sprintf(cStr, "V%x=%x", i, emulator.cpu->getRegisterV(i));
        std::string str = cStr;
        drawText(str, 0, i * (FONT_HEIGHT + 2));
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    SDL_RenderCopy(renderer, renderTexture, nullptr, nullptr);
    
    SDL_RenderPresent(renderer);
}
