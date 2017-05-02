//
//  Debugger.hpp
//  Chip8
//

#ifndef Debugger_hpp
#define Debugger_hpp

#include "CHIP.h"

class Debugger {
public:
    Debugger();
    ~Debugger();
    void attachDebugger(CPU* cpu);
    bool hasFocus() { return windowFocused; };
    bool isAttached() { return attached; };
    int getWindowID() { return windowID; };
    void handleWindowEvent(SDL_Event& ev);
    void handleKeyEvent(SDL_Event& ev);
private:
    SDL_Window* window = nullptr;
    int windowID;
    SDL_Renderer* renderer = nullptr;
    bool windowFocused = false;
    bool attached = false;
};

#endif /* Debugger_hpp */
