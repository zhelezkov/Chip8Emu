//
//  GPU.cpp
//  Chip8Emulator
//

#include "GPU.hpp"
#include <iostream>

GPU::GPU(const GPUModes mode) : mode(mode) {
    clearScreen();
}

void GPU::clearScreen()
{
    for (int x = 0; x < 128; x++)
        for (int y = 0; y < 64; y++)
			gfx[x][y] = 0;
}

void GPU::setMode(const GPUModes mode) {
    this->mode = mode;
    width = WIDTH / mode;
    height = HEIGHT / mode;
}

unsigned char GPU::operator()(int x, int y) const
{
	if (x < 0 || x >= width || y >= height || y < 0) throw "Bad Index";
	return gfx[x][y];
}
