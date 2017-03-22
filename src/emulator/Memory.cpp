//
//  Memory.cpp
//  Chip8Emulator
//

#include "Memory.hpp"

void Memory::clear() {
    for (int i = 0; i < MEM_SIZE; i++)
        data[i] = 0;
}

byte& Memory::operator[](ushort i) {
    assert(i < MEM_SIZE);
    return data[i];
}

void Memory::reset() {
    clear();
    for (int i = 0; i < 16 * 5; i++) {
        data[i] = font[i];
    }
}

void Memory::fillMemory(byte* mem) {
    for (int i = MEM_START; i < MEM_SIZE; i++) {
        data[i] = mem[i - MEM_START];
    }
}
