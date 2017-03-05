//
//  Memory.cpp
//  Chip8Emulator
//
//
//
//

#include "Memory.hpp"
#include <iostream>

void Memory::init()
{
	for (int i = 0; i < 4096; i++)
		data[i] = 0;
}

unsigned char Memory::operator[](int i)
{
	if(i < 0x200 || i > 0xFFF) std::cout << "Bad index";
	else return data[i];
}