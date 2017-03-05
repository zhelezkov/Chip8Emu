//
//  GPU.hpp
//  Chip8Emulator
//
//
//
//

#ifndef GPU_hpp
#define GPU_hpp

#include <stdio.h>

class GPU
{
	unsigned char gfx[128][64];

public:
	void init();
	unsigned char operator()(int i, int j);
};
#endif /* GPU_hpp */
