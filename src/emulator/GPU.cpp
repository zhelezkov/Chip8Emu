//
//  GPU.cpp
//  Chip8Emulator
//

#include "GPU.hpp"

GPU::GPU() {
    clearScreen();
}

void GPU::clearScreen()
{
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            gfx[x][y] = 0;
}

void GPU::setMode(const CHIPMode mode) {
    this->mode = mode;
    width = WIDTH / mode;
    height = HEIGHT / mode;
}

/*byte GPU::operator()(const int x, const int y) const
{
	if (x < 0 || x >= width || y >= height || y < 0) throw "Bad Index";
	return gfx[x][y];
}*/

int GPU::getWidth()
{
	return width;
}

int GPU::getHeight()
{
	return height;
}
