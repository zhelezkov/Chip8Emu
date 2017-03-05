//
//  GPU.hpp
//  Chip8Emulator
//

#ifndef GPU_hpp
#define GPU_hpp

#include <stdio.h>

#define WIDTH 128
#define HEIGHT 64

enum GPUModes {
    LOW = 1,
    HIGH = 2
};

class GPU
{
private:
	unsigned char gfx[WIDTH][HEIGHT];
    GPUModes mode;
    int width, height;
    
public:
    GPU(const GPUModes mode);
    void setMode(const GPUModes mode);
    GPUModes getMode() const { return mode; };
	void clearScreen();
	unsigned char operator()(int i, int j) const;
};
#endif /* GPU_hpp */
