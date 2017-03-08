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
    CHIPMode mode;
    int width, height;
    
public:
    GPU();
    void setMode(const CHIPMode mode);
    CHIPMode getMode() const { return mode; };
	void clearScreen();
	//byte operator()(const int i, const int j) const;
    
	int getWidth();
	int getHeight();

	unsigned char gfx[WIDTH][HEIGHT];
};
#endif /* GPU_hpp */
