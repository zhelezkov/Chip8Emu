//
//  Debugger.cpp
//  Chip8
//

#include <SDL.h>

#include "Debugger.hpp"

Debugger::Debugger() {
    LOG_F(INFO, "Initializing debugger");
    window = SDL_CreateWindow("Debugger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320 * 2, 240 * 2, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    CHECK_F(window != nullptr, "Error during creating window: %s", SDL_GetError());
    
    windowID = SDL_GetWindowID(window);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    CHECK_F(renderer != nullptr, "Error during creating renderer: %s", SDL_GetError());
    
    LOG_F(INFO, "Debugger window successfully created");
}

Debugger::~Debugger() {
    //TODO remove window
}

void Debugger::attachDebugger(CPU *cpu) {
    
}

void Debugger::handleKeyEvent(SDL_Event &ev) {
    
}

void Debugger::handleWindowEvent(SDL_Event &ev) {
    if (ev.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        windowFocused = false;
        DLOG_F(INFO, "Debugger window focus lost");
    } else if (ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        windowFocused = true;
        DLOG_F(INFO, "Debugger window focus gained");
    }
}
