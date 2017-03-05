//
//  Memory.hpp
//  Chip8Emulator
//

#ifndef Memory_hpp
#define Memory_hpp

#include <stdio.h>

#define MEM_SIZE 4096
#define START_MEM 0x200

class Memory
{
	unsigned char data[MEM_SIZE];

public:

	unsigned char operator[](int i);
	void clear();
};

#endif /* Memory_hpp */
