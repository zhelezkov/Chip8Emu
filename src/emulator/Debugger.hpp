//
//  Debugger.hpp
//  Chip8
//

#ifndef Debugger_hpp
#define Debugger_hpp

#include <string>
#include "CHIP.h"

//64px for V registers, then 128px for memory
static const uint DEBUGGER_REGISTERS_VIEW_WIDTH = 64;
static const uint DEBUGGER_MEMORY_VIEW_WIDTH = 128;

static const uint DEBUGGER_WINDOW_WIDTH = DEBUGGER_REGISTERS_VIEW_WIDTH + DEBUGGER_MEMORY_VIEW_WIDTH;
static const uint DEBUGGER_WINDOW_HEIGHT = 256;
static const uint DEBUGGER_WINDOW_SCALE = 2;

static const uint FONT_WIDTH = 5;
static const uint FONT_HEIGHT = 7;

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
    void renderRegisters();
    void renderMemoryView();
    
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
