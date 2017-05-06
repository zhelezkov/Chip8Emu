//
//  Debugger.hpp
//  Chip8
//

#ifndef Debugger_hpp
#define Debugger_hpp

#include "CHIP.h"

#include <SDL.h>


//64px for V registers, then 128px for memory
static const uint DEBUGGER_REGISTERS_VIEW_WIDTH = 64;
static const uint DEBUGGER_MEMORY_VIEW_WIDTH = 160;

static const uint DEBUGGER_WINDOW_WIDTH = DEBUGGER_REGISTERS_VIEW_WIDTH + DEBUGGER_MEMORY_VIEW_WIDTH;
static const uint DEBUGGER_WINDOW_HEIGHT = 270; //30
static const uint DEBUGGER_WINDOW_SCALE = 2;

static const uint FONT_WIDTH = 5;
static const uint FONT_HEIGHT = 7;

static const uint DEBUGGER_FONT_SCROLL_OFFSET = 60;

class Emulator;

class Debugger {
public:
    Debugger(Emulator&);
    ~Debugger();
    void show();
    bool hasFocus() { return windowFocused; };
    int getWindowID() { return windowID; };
    void handleWindowEvent(SDL_Event& ev);
    void handleKeyEvent(SDL_Event& ev);
    void render();
    void drawText(const char* str, int x, int y);
    bool paused() { return pause; };
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
    Emulator& emulator;
    
    ushort startAddress;
    ushort curAddress;
    ushort endAddress;
    
    bool pause;
};

#endif /* Debugger_hpp */
