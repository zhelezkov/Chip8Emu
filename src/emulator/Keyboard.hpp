//
//  Keyboard.hpp
//  Chip8Emulator
//

#ifndef Keyboard_hpp
#define Keyboard_hpp

#include <map>
#include "CHIP.h"

class Keyboard {
private:
	bool key[16];
    
public:
	void reset();
	bool isPressed(byte index);
    void keyDown(ushort index);
    void keyUp(ushort index);
};

#endif /* Keyboard_hpp */
