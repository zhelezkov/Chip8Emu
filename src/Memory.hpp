//
//  Memory.hpp
//  Chip8Emulator
//
//
//
//

#ifndef Memory_hpp
#define Memory_hpp

#include <stdio.h>

class Memory
{
	unsigned char data[4096];

public:

	unsigned char operator[](int i);
	void init();
};

#endif /* Memory_hpp */
