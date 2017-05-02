//
//  Instructions.h
//  Chip8DisAsm
//

#ifndef Instructions_h
#define Instructions_h

#include <cstdlib>
#include <fstream>

void fn_nop(std::ofstream& out, const OpcodeData data) {
	out << "byte #" << data.n1 << data.n2 << std::endl;
	out << "byte #" << data.n3 << data.n4 << std::endl;
}

/// 0x00E0 - clears the screen
void fn_0x00E0(std::ofstream& out, const OpcodeData data) {
	if(data.rawCode == 0x00E0)
		out << "CLS" << std::endl;
	else fn_nop(out, data);
}

/// 0x00EE - return from a subroutine
void fn_0x00EE(std::ofstream& out, const OpcodeData data) {
	if(data.rawCode == 0x00EE)
		out << "RET" << std::endl;
	else fn_nop(out, data);
}

/// 0x1nnn - jump to addr nnn
void fn_0x1nnn(std::ofstream& out, const OpcodeData data) {
    out << "JP " << "#" << data.n2 << data.n3 << data.n4 << std::endl;
}

/// 0x2nnn - call subroutine at nnn
void fn_0x2nnn(std::ofstream& out, const OpcodeData data) {
    out << "CALL " << "#" << data.n2 << data.n3 << data.n4 << std::endl;
}

/// 0x3xkk - skip next instruction if VX == kk
void fn_0x3xkk(std::ofstream& out, const OpcodeData data) {
    out << "SE V" << data.n2 << ", " << "#" << data.n3 << data.n4 << std::endl;
}

/// 0x4xkk - skip next instruction if VX != kk
void fn_0x4xkk(std::ofstream& out, const OpcodeData data) {
    out << "SNE V" << data.n2 << ", " << "#" << data.n3 << data.n4 << std::endl;
}

/// 0x5xy0 - skip next instruction if VX == VY
void fn_0x5xy0(std::ofstream& out, const OpcodeData data) {
	if(data.n4 == 0)
		out << "SE V" << data.n2 << ", V" << data.n3 << std::endl;
	else fn_nop(out, data);
}

/// 0x6xkk - set Vx = kk
void fn_0x6xkk(std::ofstream& out, const OpcodeData data) {
    out << "LD V" << data.n2 << ", " << "#" << data.n3 << data.n4 << std::endl;
}

/// 0x7xkk - set VX += kk
void fn_0x7xkk(std::ofstream& out, const OpcodeData data) {
    out << "ADD V" << data.n2 << ", " << "#" << data.n3 << data.n4 << std::endl;
}

/// 0x8xy0 - set Vx = Vy
void fn_0x8xy0(std::ofstream& out, const OpcodeData data) {
    out << "LD V" << data.n2 << ", V" << data.n3 << std::endl;
}

/// 0x8xy1 - set Vx = Vx | Vy
void fn_0x8xy1(std::ofstream& out, const OpcodeData data) {
    out << "OR V" << data.n2 << ", V" << data.n3 << std::endl;
}

/// 0x8xy2 - set Vx = Vx & Vy
void fn_0x8xy2(std::ofstream& out, const OpcodeData data) {
    out << "AND V" << data.n2 << ", V" << data.n3 << std::endl;
}

/// 8xy3 - set Vx = Vx ^ Vy
void fn_0x8xy3(std::ofstream& out, const OpcodeData data) {
    out << "XOR V" << data.n2 << ", V" << data.n3 << std::endl;
}

/// 0x8xy4 - set Vx = Vx + Vy, VF = 1 if overflow else VF = 0
void fn_0x8xy4(std::ofstream& out, const OpcodeData data) {
    out << "ADD V" << data.n2 << ", V" << data.n3 << std::endl;
}

/// 0x8xy5 - set Vx = Vx - Vy, VF = 1 if not borrow else 0
void fn_0x8xy5(std::ofstream& out, const OpcodeData data) {
    out << "SUB V" << data.n2 << ", V" << data.n3 << std::endl;
}

/// 8xy6 - set Vx = Vx >> 1, VF = least-significant bit
void fn_0x8xy6(std::ofstream& out, const OpcodeData data) {
    out << "SHR V" << data.n2 << std::endl;
}

/// 0x8xy7 - set Vx = Vy - Vx, VF = 1 if not borrow else 0
void fn_0x8xy7(std::ofstream& out, const OpcodeData data) {
    out << "SUBN V" << data.n2 << ", V" << data.n3 << std::endl;
}

/// 0x8xyE - set VX = VX << 1, VF = most-significant bit
void fn_0x8xyE(std::ofstream& out, const OpcodeData data) {
    out << "SHL V" << data.n2 << std::endl;
}

/// 0x9xy0 - skip next instruction if Vx != Vy
void fn_0x9xy0(std::ofstream& out, const OpcodeData data) {
	if(data.n4 == 0)
		out << "SNE V" << data.n2 << ", V" << data.n3 << std::endl;
	else fn_nop(out, data);
}

/// 0xAnnn - set I = Addr
void fn_0xAnnn(std::ofstream& out, const OpcodeData data) {
    out << "LD I, " << "#" << data.n2 << data.n3 << data.n4 << std::endl;
}
/// 0xBnnn - jump to Addr + V0
void fn_0xBnnn(std::ofstream& out, const OpcodeData data) {
    out << "JP V0, " << "#" << data.n2 << data.n3 << data.n4 << std::endl;
}

/// 0xCxkk - set Vx = random & kk
void fn_0xCxkk(std::ofstream& out, const OpcodeData data) {
    out << "RND V" << data.n2 << ", " << "#" << data.n3 << data.n4 << std::endl;
}

/// 0xDxyn - Draw 8xN sprite at I to VX, VY; VF = 1 if collision else 0
void fn_0xDxyn(std::ofstream& out, const OpcodeData data) {
    out << "DRW V" << data.n2 << ", V" << data.n3 << " " << "#" << data.n4 << std::endl;
}

/// 0xEx9E - skip next instruction if key Vx down
void fn_0xEx9E(std::ofstream& out, const OpcodeData data) {
    out << "SKP V" << data.n2 << std::endl;
}

/// 0xExA1 - skip next instruction if key Vx up
void fn_0xExA1(std::ofstream& out, const OpcodeData data) {
    out << "SKNP V" << data.n2 << std::endl;
}

/// 0xFx07 - set Vx = delayTimer
void fn_0xFx07(std::ofstream& out, const OpcodeData data) {
    out << "LD V" << data.n2 << ", DT" << std::endl;
}

/// 0xFx0A - set Vx = key, wait for keypress
void fn_0xFx0A(std::ofstream& out, const OpcodeData data) {
    out << "LD V" << data.n2 << ", K" << std::endl;
}

/// 0xFx15 - set delayTimer = Vx
void fn_0xFx15(std::ofstream& out, const OpcodeData data) {
    out << "LD DT, V" << data.n2 << std::endl;
}

/// 0xFx18 - set soundTimer = Vx
void fn_0xFx18(std::ofstream& out, const OpcodeData data) {
    out << "LD ST, V" << data.n2 << std::endl;
}

/// 0xFx1E - I = I + VX; VF = 1 if I > 0xFFF else 0
void fn_0xFx1E(std::ofstream& out, const OpcodeData data) {
    out << "ADD I, V" << data.n2 << std::endl;
}

/// 0xFx29 - point I to 5 byte numeric sprite for value in VX
void fn_0xFx29(std::ofstream& out, const OpcodeData data) {
    out << "LD F, V" << data.n2 << std::endl;
}

/// 0xFx33 - store BCD of VX in [I], [I+1], [I+2]
void fn_0xFx33(std::ofstream& out, const OpcodeData data) {
    out << "LD B, V" << data.n2 << std::endl;
}

/// 0xFx55 - store V0 .. VX (inclusive) in [I] .. [I + X]
void fn_0xFx55(std::ofstream& out, const OpcodeData data) {
    out << "LD [I], V" << data.n2 << std::endl;
}

/// 0xFx65 - read V0 .. VX (inclusive) from [I] .. [I + X]
void fn_0xFx65(std::ofstream& out, const OpcodeData data) {
    out << "LD V" << data.n2 << ", [I]" << std::endl;
}

/************ Super CHIP-8 functions ************/

/// 0x00Bn: - scroll display N lines up (N/2 in low res mode)
void fn_0x00Bn(std::ofstream& out, const OpcodeData data) {
	if(data.n2 == 0)
		out << "SCU " << "#" << data.n4 << std::endl;
	else fn_nop(out, data);
}

/// 0x00Cn - scroll display N lines down (N/2 in low res mode)
void fn_0x00Cn(std::ofstream& out, const OpcodeData data) {
	if(data.n2 == 0)
		out << "SCD " << "#" << data.n4 << std::endl;
	else fn_nop(out, data);
}

/// 0x00FB - scroll right 4 pixels (2 pixels in low res mode)
void fn_0x00FB(std::ofstream& out, const OpcodeData data) {
	if(data.rawCode == 0x00FB)
		out << "SCR" << std::endl;
	else fn_nop(out, data);
}

/// 0x00FC - scroll left 4 pixels (2 pixels in low res mode)
void fn_0x00FC(std::ofstream& out, const OpcodeData data) {
    out << "SCL" << std::endl;
}

/// 0x00FD - Quit the emulator
void fn_0x00FD(std::ofstream& out, const OpcodeData data) {
	if(data.rawCode == 0x00FD)
		out << "EXIT" << std::endl;
	else fn_nop(out, data);
}

/// 0x00FE -  enter low res mode
void fn_0x00FE(std::ofstream& out, const OpcodeData data) {
	if(data.rawCode == 0x00FE)
		out << "LOW" << std::endl;
	else fn_nop(out, data);
}

/// 0x00FF -  enter high res mode
void fn_0x00FF(std::ofstream& out, const OpcodeData data) {
	if(data.rawCode == 0x00FF)
		out << "HIGH" << std::endl;
	else fn_nop(out, data);
}

/// 0xFx30 - point I to 10 byte numeric sprite for value in VX
void fn_0xFx30(std::ofstream& out, const OpcodeData data) {
    out << "LD HF, V" << data.n2 << std::endl;
}

/// 0xFx75 - save V0...VX (X<8) in the RPL flags
void fn_0xFx75(std::ofstream& out, const OpcodeData data) {
    out << "LD R, V" << data.n2 << std::endl;
}

/// FX85 - load V0...VX (X<8) from the RPL flags
void fn_0xFx85(std::ofstream& out, const OpcodeData data) {
    out << "LD V" << data.n2 << ", R" << std::endl;
}

/************ Extended CHIP-8 functions ************/
//TODO

#endif /* Instructions_h */
