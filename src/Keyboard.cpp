//
//  Keyboard.cpp
//  Chip8Emulator
//
//  
//
//

#include "Keyboard.hpp""

void Keyboard::init()
{
	// обнуление ключей
	for (int i = 0; i < 16; i++)
		key[i] = 0;

	// загрузка шрифтов
	for (int i = 0; i < 50; ++i)
		memory[i] = font[i];
}