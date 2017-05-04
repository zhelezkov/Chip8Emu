//
//  Keyboard.hpp
//  Chip8Emulator
//

#ifndef Keyboard_hpp
#define Keyboard_hpp

#include <map>
#include "CHIP.h"

byte sdlKeyToChipKey(ushort index);

class Keyboard {
public:
	void reset();
	bool isPressed(byte index);
    void keyDown(ushort index);
    void keyUp(ushort index);
private:
    bool key[16];
};

#endif /* Keyboard_hpp */
