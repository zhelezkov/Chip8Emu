//
//  GPU.cpp
//  Chip8Emulator
//

#include "GPU.hpp"
#include "CHIP.h"

GPU::~GPU() {
    delete[] videoMem;
}

void GPU::clearScreen() {
    for (ushort i = 0; i < width * height; i++) {
        videoMem[i] = 0;
    }
}

void GPU::setResolutionMode(const ResolutionMode mode) {
    resMode = mode;
    width = GPU_DEFAULT_WIDTH * resMode;
    height = GPU_DEFAULT_HEIGHT * resMode;
    if (videoMem)
        delete[] videoMem;
    videoMem = new byte[width * height];
    clearScreen();
}

void GPU::reset() {
	setResolutionMode(LOW);
	clearScreen();
}

#define checkIndex      ushort index = (x + y * width);\
                        assert(index < width * height);

void GPU::setPixel(byte pix, ushort x, ushort y) {
    checkIndex
    videoMem[index] = pix;
}

byte GPU::getPixel(ushort x, ushort y) {
    checkIndex
    return videoMem[index];
}

void GPU::xorPixel(ushort x, ushort y) {
    checkIndex
    videoMem[index] ^= 1;
}

#undef checkIndex
