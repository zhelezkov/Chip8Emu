//
//  GPU.hpp
//  Chip8Emulator
//

#ifndef GPU_hpp
#define GPU_hpp

#include "CHIP.h"

#define WIDTH 128
#define HEIGHT 64

class GPU
{
private:
	unsigned char gfx[WIDTH][HEIGHT];
    CHIPMode mode;
    int width, height;
    
public:
    GPU();
    void setMode(const CHIPMode mode);
    CHIPMode getMode() const { return mode; };
	void clearScreen();
	unsigned char operator()(int i, int j) const;

	int getWidth();
	int getHeight();
};
#endif /* GPU_hpp */
