//
//  Memory.cpp
//  Chip8Emulator
//

#include "Memory.hpp"

void Memory::clear() {
    for (int i = 0; i < MEM_SIZE; i++)
        data[i] = 0;
}

byte& Memory::operator[](int i) {
    assert(i >= MEM_START && i < MEM_SIZE);
    return data[i];
}

void Memory::reset() {
    clear();
}
