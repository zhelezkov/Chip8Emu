//
//  Keyboard.cpp
//  Chip8Emulator
//

#include "Keyboard.hpp"

void Keyboard::reset()
{
	// reset the keys
	for (int i = 0; i < 16; i++)
		key[i] = 0;

	// load the fonts
	//for (int i = 0; i < 80; ++i)
		//memory[i] = font[i];

	// load the bigfonts
	//for (int i = 80; i < 80 + 16 * 10; ++i)
		//memory[i] = bigfont[i];
}

bool Keyboard::isPressed(unsigned char index) {
	return key[index];
}
