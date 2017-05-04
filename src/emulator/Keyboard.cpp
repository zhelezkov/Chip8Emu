//
//  Keyboard.cpp
//  Chip8Emulator
//

#include <SDL.h>

#include "Keyboard.hpp"
#include "CPU.hpp"

static std::map<SDL_Scancode, byte> sdlKeys = {
    {SDL_SCANCODE_X, 0},
    {SDL_SCANCODE_1, 1},
    {SDL_SCANCODE_2, 2},
    {SDL_SCANCODE_3, 3},
    {SDL_SCANCODE_Q, 4},
    {SDL_SCANCODE_W, 5},
    {SDL_SCANCODE_E, 6},
    {SDL_SCANCODE_A, 7},
    {SDL_SCANCODE_S, 8},
    {SDL_SCANCODE_D, 9},
    {SDL_SCANCODE_Z, 0xA},
    {SDL_SCANCODE_C, 0xB},
    {SDL_SCANCODE_4, 0xC},
    {SDL_SCANCODE_R, 0xD},
    {SDL_SCANCODE_F, 0xE},
    {SDL_SCANCODE_V, 0xF}
};

void Keyboard::reset() {
	for (int i = 0; i < 16; i++)
		key[i] = 0;
}

bool Keyboard::isPressed(byte index) {
	return key[index];
}

void Keyboard::keyDown(ushort index) {
    if (CPU::getInstance()->waitingForKey) {
        CPU* cpu = CPU::getInstance();
        cpu->setRegisterV(cpu->registerWaitingKey, sdlKeyToChipKey(index));
        cpu->waitingForKey = false;
    }
    key[sdlKeyToChipKey(index)] = true;
}

void Keyboard::keyUp(ushort index) {
    key[sdlKeyToChipKey(index)] = false;
}

byte sdlKeyToChipKey(ushort index) {
    return sdlKeys[(SDL_Scancode) index];
}
