//
//  Debugger.hpp
//  Chip8
//

#ifndef Debugger_hpp
#define Debugger_hpp

#include "CHIP.h"
#include <string>

class Emulator;

class Debugger {
public:
    Debugger(Emulator&);
    ~Debugger();
    void show();
    bool hasFocus() { return windowFocused; };
    bool isAttached() { return attached; };
    int getWindowID() { return windowID; };
    void handleWindowEvent(SDL_Event& ev);
    void handleKeyEvent(SDL_Event& ev);
    void render();
    void drawText(std::string& str, int x, int y);
private:
    void loadFont();
    SDL_Window* window = nullptr;
    int windowID;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* renderTexture = nullptr;
    SDL_Texture* fontTexture = nullptr;
    
    bool windowFocused = false;
    bool attached = false;
    Emulator& emulator;
};

#endif /* Debugger_hpp */
