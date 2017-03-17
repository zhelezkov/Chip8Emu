//
//  GPU.hpp
//  Chip8Emulator
//

#ifndef GPU_hpp
#define GPU_hpp

#include "CHIP.h"

#define WIDTH 128
#define HEIGHT 64

enum ResolutionMode {
    LOW = 1,
    HIGH = 2
};

class GPU
{
private:
    ResolutionMode resMode;
    int width, height;
    byte* videoMem;

public:
    GPU() : videoMem(nullptr) { };
    void setResolutionMode(const ResolutionMode mode);
    ResolutionMode getResolutionMode() const { return resMode; };
	void clearScreen();
    void reset();
    byte getPixel(int x, int y);
    void setPixel(byte pix, int x, int y);
    
	int getWidth();
	int getHeight();
};
#endif /* GPU_hpp */
