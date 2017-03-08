//
//  Emulator.cpp
//  Chip8Emulator
//

#include "Emulator.hpp"

//#include "CPU.hpp"
//#include "OpCodes.hpp"

Emulator::Emulator() {
    Memory mem;
    GPU gpu;
    TimersManager timers;
    Keyboard keyboard;
    CPU cpu(mem, gpu, timers, keyboard);
    
    printf("EMULATOR STARTED");
}

int main() {
    Emulator emulator;
    return 0;
}
