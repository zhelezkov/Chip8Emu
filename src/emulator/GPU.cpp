//
//  GPU.cpp
//  Chip8Emulator
//

#include "GPU.hpp"

void GPU::clearScreen() {
    for (ushort i = 0; i < width * height; i++) {
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
	setResolutionMode(LOW);
	clearScreen();
}

void GPU::setPixel(byte pix, ushort x, ushort y) {
    assert((x + y * width) < width * height);
    videoMem[x + y * width] = pix;
}

byte GPU::getPixel(ushort x, ushort y) {
    assert((x + y * width) < width * height);
    return videoMem[x + y * width];
}
