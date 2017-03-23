//
//  Emulator.hpp
//  Chip8Emulator
//

#ifndef Emulator_hpp
#define Emulator_hpp

#include "CHIP.h"

class Emulator {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* screenTexture = nullptr;
    CPU* cpu = nullptr;
   
    void initWindow();
    void initEmulator();
   
    //void initEmulator();
public:
    Emulator();
    ~Emulator();
    void run();
    void render();
    void swap();
    bool loadRom();
};

#endif /* Emulator_hpp */
