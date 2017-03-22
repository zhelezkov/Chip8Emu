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

}

bool Keyboard::isPressed(byte index) {
	return key[index];
}

void Keyboard::keyDown(byte index) {
    key[index] = true;
}

void Keyboard::keyUp(byte index) {
    key[index] = false;
}
