//
//  Debugger.cpp
//  Chip8
//

#include "Debugger.hpp"
#include "Emulator.hpp"
#include "CPU.hpp"
#include "TimersManager.hpp"
#include "../disasm/Disassembler.hpp"
#include "Memory.hpp"

Debugger::Debugger(Emulator& emulator) : emulator(emulator), startAddress(0x200), endAddress(0x200 + 60), pause(false) {
    LOG_F(INFO, "Initializing debugger");
    int x, y;
    SDL_GetWindowPosition(emulator.window, &x, &y);
    window = SDL_CreateWindow("Debugger", x + EMULATOR_WINDOW_WIDTH * EMULATOR_WINDOW_SCALE, y, DEBUGGER_WINDOW_WIDTH * DEBUGGER_WINDOW_SCALE, DEBUGGER_WINDOW_HEIGHT * DEBUGGER_WINDOW_SCALE, SDL_WINDOW_OPENGL);
    CHECK_F(window != nullptr, "Error during creating window: %s", SDL_GetError());
    
    windowID = SDL_GetWindowID(window);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    CHECK_F(renderer != nullptr, "Error during creating renderer: %s", SDL_GetError());
    
    renderTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DEBUGGER_WINDOW_WIDTH, DEBUGGER_WINDOW_HEIGHT);
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
    if (ev.type == SDL_KEYUP) {
        SDL_Scancode key = ev.key.keysym.scancode;
        if (key == SDL_SCANCODE_SPACE) {
            pause = !pause;
        }
        if (key == SDL_SCANCODE_N && pause) {
            emulator.cpu->tick();
        }
    }
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

void Debugger::drawText(const char* str, int x, int y) {
    SDL_Rect src = {0, 0, FONT_WIDTH, FONT_HEIGHT};
    SDL_Rect dst = {x, y, FONT_WIDTH, FONT_HEIGHT};
    for (int i = 0; i < strlen(str); i++) {
        char c = str[i];
        if (c > 32 && c < 127) {
            src.x = (c - 33) * 6;
            SDL_RenderCopy(renderer, fontTexture, &src, &dst);
        }
        dst.x += FONT_WIDTH + 2;
    }
}

void Debugger::renderRegisters() {
    SDL_Rect drawRect = {0, 0, DEBUGGER_REGISTERS_VIEW_WIDTH, DEBUGGER_WINDOW_HEIGHT};
    SDL_RenderSetViewport(renderer, &drawRect);
    
    char str[20];
    int curY = 0;
    for (int i = 0; i < V_REGISTERS_COUNT; i++) {
        sprintf(str, "V%x=#%x", i, emulator.cpu->getRegisterV(i));
        drawText(str, 0, curY);
        curY += 9;
    }
    sprintf(str, "DT=#%x", emulator.cpu->getTimersManager().getDelayTimer());
    drawText(str, 0, curY);
    curY += 9;
    sprintf(str, "ST=#%x", emulator.cpu->getTimersManager().getSoundTimer());
    drawText(str, 0, curY);
    curY += 9;
    sprintf(str, "I =#%x", emulator.cpu->getAddrRegister());
    drawText(str, 0, curY);
    curY += 9;
    sprintf(str, "PC=#%x", emulator.cpu->getPC());
    drawText(str, 0, curY);
    curY += 9;
    sprintf(str, "SP=#%x", emulator.cpu->getSP());
    drawText(str, 0, curY);
    curY += 9;
    for (int i = 0; i < R_REGISTERS_COUNT; i++) {
        sprintf(str, "R%x=#%x", i, emulator.cpu->getRegisterR(i));
        drawText(str, 0, curY);
        curY += 9;
    }
    
}

void Debugger::renderMemoryView() {
    SDL_Rect drawRect = {DEBUGGER_REGISTERS_VIEW_WIDTH, 0, DEBUGGER_REGISTERS_VIEW_WIDTH + DEBUGGER_MEMORY_VIEW_WIDTH, DEBUGGER_WINDOW_HEIGHT};
    SDL_RenderSetViewport(renderer, &drawRect);
    Memory& mem = emulator.cpu->getMemory();
    ushort PC = emulator.cpu->getPC();
    
    curAddress = PC;
    
    if (curAddress < startAddress || curAddress > endAddress - 2) {
        startAddress = curAddress - 2;
        endAddress = startAddress + DEBUGGER_FONT_SCROLL_OFFSET;
    }
    
    int curY = 0;
    char str[32];
    for (int i = startAddress; i < endAddress; i += 2) {
        const char* disStr = Disassembler::disasmBytecode(mem[i] << 8 | mem[i + 1]).c_str();
        sprintf(str, "#%x - %s", i, disStr);
        if (curAddress == i) {
            SDL_Rect curAddressRect = {0, curY, DEBUGGER_MEMORY_VIEW_WIDTH, 9};
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            SDL_RenderFillRect(renderer, &curAddressRect);
        }
        drawText(str, 0, curY);
        curY += 9;
    }
}

void Debugger::render() {
    SDL_SetRenderTarget(renderer, renderTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    renderRegisters();
    renderMemoryView();
    
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    SDL_RenderCopy(renderer, renderTexture, nullptr, nullptr);
    
    SDL_RenderPresent(renderer);
}
