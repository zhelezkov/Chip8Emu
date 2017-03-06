//
//  Memory.hpp
//  Chip8Emulator
//

#ifndef Memory_hpp
#define Memory_hpp

#include "CHIP.h"

#define MEM_SIZE 0xFFF
#define MEM_START 0x200

class Memory {
private:
	byte data[MEM_SIZE];

public:
	byte& operator[](int i);
	void clear();
};

#endif /* Memory_hpp */
