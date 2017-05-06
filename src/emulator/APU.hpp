//
//  APU.hpp
//  Chip8Emulator
//

#ifndef APU_hpp
#define APU_hpp

#include "CHIP.h"
#include "SDL.h"

class APU {
public:
    APU();
    ~APU();
    void reset();
    
    void playSound();
private:
    SDL_AudioSpec wavSpec;
};

#endif /* APU_hpp */
