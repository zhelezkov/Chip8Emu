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
    CPU cpu(STANDART, mem, gpu, timers);
}

int main() {
    Emulator emulator;
    return 0;
}
