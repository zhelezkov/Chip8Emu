//
//  Emulator.cpp
//  Chip8Emulator
//

#include "Emulator.hpp"

//#include "CPU.hpp"
//#include "OpCodes.hpp"


CPU* CPU::instance = nullptr;

int main() {
    CPU cpu;
    CPU::instance = &cpu;
    const Opcode& c = getOpcode({0xffff});
    
        const Opcode& cc = getOpcode({0xffff});
    
    const Opcode& ccc = getOpcode({0xffff});
    printf("Emulator");
    return 0;
}
