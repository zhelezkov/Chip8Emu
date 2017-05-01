//
//  InstructionsAsm.h
//  Chip8Asm
//

#ifndef Instructions_h
#define Instructions_h

#include <cstdlib>
#include <fstream>
#include "HelpFunctions.h"
#include "StringToken.hpp"

void fn_nop(std::ofstream& out, VECTOR_TYPE& str) {
    //throw "Unknown op";
	/*for (int i = CommandToken.first; i <= CommandToken.second; i++)
		out << 0x00 << strToNumber(str.first[i].commandStr);*/
}

/// 0x00E0 - clears the screen
void fn_0x00E0(std::ofstream& out, VECTOR_TYPE& str) {
	out << 0x00E0;
}

/// 0x00EE - return from a subroutine
void fn_0x00EE(std::ofstream& out, VECTOR_TYPE& str) {
	out << 0x00EE;
}

/// 0x1nnn - jump to addr nnn
void fn_0x1nnn(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x2nnn - call subroutine at nnn
void fn_0x2nnn(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x3xkk - skip next instruction if VX == kk
void fn_0x3xkk(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x4xkk - skip next instruction if VX != kk
void fn_0x4xkk(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x5xy0 - skip next instruction if VX == VY
void fn_0x5xy0(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x6xkk - set Vx = kk
void fn_0x6xkk(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x7xkk - set VX += kk
void fn_0x7xkk(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x8xy0 - set Vx = Vy
void fn_0x8xy0(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x8xy1 - set Vx = Vx | Vy
void fn_0x8xy1(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x8xy2 - set Vx = Vx & Vy
void fn_0x8xy2(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 8xy3 - set Vx = Vx ^ Vy
void fn_0x8xy3(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x8xy4 - set Vx = Vx + Vy, VF = 1 if overflow else VF = 0
void fn_0x8xy4(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x8xy5 - set Vx = Vx - Vy, VF = 1 if not borrow else 0
void fn_0x8xy5(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 8xy6 - set Vx = Vx >> 1, VF = least-significant bit
void fn_0x8xy6(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x8xy7 - set Vx = Vy - Vx, VF = 1 if not borrow else 0
void fn_0x8xy7(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x8xyE - set VX = VX << 1, VF = most-significant bit
void fn_0x8xyE(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x9xy0 - skip next instruction if Vx != Vy
void fn_0x9xy0(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xAnnn - set I = Addr
void fn_0xAnnn(std::ofstream& out, VECTOR_TYPE& str) {

}
/// 0xBnnn - jump to Addr + V0
void fn_0xBnnn(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xCxkk - set Vx = random & kk
void fn_0xCxkk(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xDxyn - Draw 8xN sprite at I to VX, VY; VF = 1 if collision else 0
void fn_0xDxyn(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xEx9E - skip next instruction if key Vx down
void fn_0xEx9E(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xExA1 - skip next instruction if key Vx up
void fn_0xExA1(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx07 - set Vx = delayTimer
void fn_0xFx07(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx0A - set Vx = key, wait for keypress
void fn_0xFx0A(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx15 - set delayTimer = Vx
void fn_0xFx15(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx18 - set soundTimer = Vx
void fn_0xFx18(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx1E - I = I + VX; VF = 1 if I > 0xFFF else 0
void fn_0xFx1E(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx29 - point I to 5 byte numeric sprite for value in VX
void fn_0xFx29(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx33 - store BCD of VX in [I], [I+1], [I+2]
void fn_0xFx33(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx55 - store V0 .. VX (inclusive) in [I] .. [I + X]
void fn_0xFx55(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx65 - read V0 .. VX (inclusive) from [I] .. [I + X]
void fn_0xFx65(std::ofstream& out, VECTOR_TYPE& str) {

}

/************ Super CHIP-8 functions ************/

/// 0x00Bn: - scroll display N lines up (N/2 in low res mode)
void fn_0x00Bn(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x00Cn - scroll display N lines down (N/2 in low res mode)
void fn_0x00Cn(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x00FB - scroll right 4 pixels (2 pixels in low res mode)
void fn_0x00FB(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x00FC - scroll left 4 pixels (2 pixels in low res mode)
void fn_0x00FC(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x00FD - Quit the emulator
void fn_0x00FD(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x00FE -  enter low res mode
void fn_0x00FE(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0x00FF -  enter high res mode
void fn_0x00FF(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx30 - point I to 10 byte numeric sprite for value in VX
void fn_0xFx30(std::ofstream& out, VECTOR_TYPE& str) {

}

/// 0xFx75 - save V0...VX (X<8) in the RPL flags
void fn_0xFx75(std::ofstream& out, VECTOR_TYPE& str) {

}

/// FX85 - load V0...VX (X<8) from the RPL flags
void fn_0xFx85(std::ofstream& out, VECTOR_TYPE& str) {

}

/************ Extended CHIP-8 functions ************/
//TODO

#endif /* InstructionsAsm_h */
