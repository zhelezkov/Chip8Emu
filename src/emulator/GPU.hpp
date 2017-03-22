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
    LOW = 2,
    HIGH = 1
};

class GPU
{
private:
    ResolutionMode resMode;
    ushort width, height;
    byte* videoMem;

public:
    GPU() : videoMem(nullptr) { };
    ~GPU();
    void setResolutionMode(const ResolutionMode mode);
    ResolutionMode getResolutionMode() const { return resMode; };
	void clearScreen();
    void reset();
    byte getPixel(ushort x, ushort y);
    void setPixel(byte pix, ushort x, ushort y);
    void xorPixel(ushort x, ushort y);
    
    ushort getWidth() { return width; };
    ushort getHeight() { return height; };
};
#endif /* GPU_hpp */
