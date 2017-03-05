//
//  GPU.cpp
//  Chip8Emulator
//
//
//
//

#include "GPU.hpp"
#include <iostream>

void GPU::init()
{
	// очистка экрана
	for (int y = 0; y < 64; y++)
		for (int x = 0; x < 128; x++)
			gfx[y][x] = 0;
}

unsigned char GPU::operator()(int i, int j)
{
	if (i < 0 || i > 128 || j > 64 || j < 0) std::cout << "Bad index";
	else return gfx[i][j];
}