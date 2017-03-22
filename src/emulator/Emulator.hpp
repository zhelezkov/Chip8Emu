//
//  Emulator.hpp
//  Chip8Emulator
//

#ifndef Emulator_hpp
#define Emulator_hpp

#include "CHIP.h"

#include "CPU.hpp"
#include "Memory.hpp"
#include "TimersManager.hpp"

class Emulator {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* screenTexture;
    CPU* cpu;
   
    void initWindow();
    void initEmulator();
   
    //void initEmulator();
public:
    Emulator();
    ~Emulator();
    void run();
    bool loadRom();
};

#endif /* Emulator_hpp */
