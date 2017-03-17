//
//  GPU.cpp
//  Chip8Emulator
//

#include "GPU.hpp"

void GPU::clearScreen() {
    for (int i = 0; i < width * height; i++) {
        videoMem[i] = 0;
    }
}

void GPU::setResolutionMode(const ResolutionMode mode) {
    if (resMode == mode) return;
    resMode = mode;
    width = WIDTH / resMode;
    height = HEIGHT / resMode;
    if (videoMem)
        delete[] videoMem;
    videoMem = new byte[width * height];
}

void GPU::reset() {
    clearScreen();
    setResolutionMode(LOW);
}

void GPU::setPixel(byte pix, int x, int y) {
    videoMem[x + y * width] = pix;
}

byte GPU::getPixel(int x, int y) {
    return videoMem[x + y * width];
}

int GPU::getWidth()
{
	return width;
}

int GPU::getHeight()
{
	return height;
}
