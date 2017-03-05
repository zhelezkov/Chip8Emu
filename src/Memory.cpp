//
//  Memory.cpp
//  Chip8Emulator
//

#include "Memory.hpp"

void Memory::clear() {
    for (int i = 0; i < MEM_SIZE; i++)
        data[i] = 0;
}

unsigned char Memory::operator[](int i) {
    if (i < MEM_START || i >= MEM_SIZE) throw "Bad index";
    return data[i];
}
