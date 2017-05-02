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
#include "Defines.h"

#define HEX_BYTE(n1, n2) (n1 << 4 | n2)
extern std::vector <std::pair<int, bool>> file;		  // in Assembler.cpp
extern std::map<std::string, int> labels;			  // in Assembler.cpp
extern std::map<std::string, byte> var;				  // in Assembler.cpp					
extern std::map<std::string, byte> equ;				  // in Assembler.cpp	
extern std::vector<VECTOR_TYPE> ovector;			  // in Assembler.cpp

void fn_nop(std::ofstream& out, int strNum) {
    //throw "Unknown op";
	for (int i = CMD_IND(strNum).first + 1; i <= CMD_IND(strNum).second; i++)
		out << (byte)strToNumber(CMD_STR(strNum, i));
}

/// 0x00E0 - clears the screen
void fn_0x00E0(std::ofstream& out, int strNum) {
	out << (byte)0x00 << (byte)0xE0;
}

/// 0x00EE - return from a subroutine
void fn_0x00EE(std::ofstream& out, int strNum) {
	out << (byte)0x00 << (byte)0xEE;
}

/// 0x1nnn - jump to addr nnn
void fn_0x1nnn(std::ofstream& out, int strNum) {
	ushort addr;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// label
	{
		addr = file[labels[CMD_STR(strNum, CMD_IND(strNum).first + 1)]].first;
	}
	else
	{
		addr = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1));	// number
	}
	OpcodeData data(addr);
	out << (byte)(HEX_BYTE(1, data.n2)) << (byte)(HEX_BYTE(data.n3, data.n4));
}

/// 0x2nnn - call subroutine at nnn
void fn_0x2nnn(std::ofstream& out, int strNum) {
	ushort addr;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// label
	{
		addr = file[labels[CMD_STR(strNum, CMD_IND(strNum).first + 1)]].first;
	}
	else
	{
		addr = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1));	// number
	}
	OpcodeData data(addr);
	out << (byte)(HEX_BYTE(2, data.n2)) << (byte)(HEX_BYTE(data.n3, data.n4));
}

/// 0x3xkk - skip next instruction if VX == kk
void fn_0x3xkk(std::ofstream& out, int strNum) {

	byte regInd;
	byte kk;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// const
	{
		kk = equ[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		kk = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)(0x30 + regInd) << kk;
}

/// 0x4xkk - skip next instruction if VX != kk
void fn_0x4xkk(std::ofstream& out, int strNum) {
	byte regInd;
	byte kk;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// const
	{
		kk = equ[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		kk = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)(0x40 + regInd) << kk;
}

/// 0x5xy0 - skip next instruction if VX == VY
void fn_0x5xy0(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(5, regX) << (byte)HEX_BYTE(regY, 0);
}

/// 0x6xkk - set Vx = kk
void fn_0x6xkk(std::ofstream& out, int strNum) {
	byte regInd;
	byte kk;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// const
	{
		kk = equ[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		kk = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)(0x60 + regInd) << kk;
}

/// 0x7xkk - set VX += kk
void fn_0x7xkk(std::ofstream& out, int strNum) {
	byte regInd;
	byte kk;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// const
	{
		kk = equ[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		kk = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)(0x70 + regInd) << kk;
}

/// 0x8xy0 - set Vx = Vy
void fn_0x8xy0(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 0);
}

/// 0x8xy1 - set Vx = Vx | Vy
void fn_0x8xy1(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 1);
}

/// 0x8xy2 - set Vx = Vx & Vy
void fn_0x8xy2(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 2);
}

/// 8xy3 - set Vx = Vx ^ Vy
void fn_0x8xy3(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 3);
}

/// 0x8xy4 - set Vx = Vx + Vy, VF = 1 if overflow else VF = 0
void fn_0x8xy4(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 4);
}

/// 0x8xy5 - set Vx = Vx - Vy, VF = 1 if not borrow else 0
void fn_0x8xy5(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 5);
}

/// 8xy6 - set Vx = Vx >> 1, VF = least-significant bit
void fn_0x8xy6(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 6);
}

/// 0x8xy7 - set Vx = Vy - Vx, VF = 1 if not borrow else 0
void fn_0x8xy7(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 7);
}

/// 0x8xyE - set VX = VX << 1, VF = most-significant bit
void fn_0x8xyE(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(8, regX) << (byte)HEX_BYTE(regY, 0xE);
}

/// 0x9xy0 - skip next instruction if Vx != Vy
void fn_0x9xy0(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)HEX_BYTE(9, regX) << (byte)HEX_BYTE(regY, 0);
}

/// 0xAnnn - set I = Addr
void fn_0xAnnn(std::ofstream& out, int strNum) {
	ushort addr;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// label
	{
		addr = file[labels[CMD_STR(strNum, CMD_IND(strNum).first + 1)]].first;
	}
	else
	{
		addr = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1));	// number
	}
	OpcodeData data(addr);
	out << (byte)(HEX_BYTE(0xA, data.n2)) << (byte)(HEX_BYTE(data.n3, data.n4));
}
/// 0xBnnn - jump to Addr + V0
void fn_0xBnnn(std::ofstream& out, int strNum) {
	ushort addr;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// label
	{
		addr = file[labels[CMD_STR(strNum, CMD_IND(strNum).first + 1)]].first;
	}
	else
	{
		addr = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1));	// number
	}
	OpcodeData data(addr);
	out << (byte)(HEX_BYTE(0xB, data.n2)) << (byte)(HEX_BYTE(data.n3, data.n4));
}

/// 0xCxkk - set Vx = random & kk
void fn_0xCxkk(std::ofstream& out, int strNum) {
	byte regInd;
	byte kk;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// const
	{
		kk = equ[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		kk = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	out << (byte)(0xC0 + regInd) << kk;
}

/// 0xDxyn - Draw 8xN sprite at I to VX, VY; VF = 1 if collision else 0
void fn_0xDxyn(std::ofstream& out, int strNum) {
	byte regX;
	byte regY;
	ushort n;
	/***************** regX ****************************/
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regX = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regX = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	/***************** regY ****************************/
	if (TOKEN(strNum, CMD_IND(strNum).first + 2).type == NAME)	// var
	{
		regY = var[CMD_STR(strNum, CMD_IND(strNum).first + 2)];
	}
	else
	{
		regY = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2)); // number
	}

	/******************* n ****************************/
	if (TOKEN(strNum, CMD_IND(strNum).first + 3).type == NAME)	// const
	{
		n = equ[CMD_STR(strNum, CMD_IND(strNum).first + 3)];
	}
	else
	{
		n = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 3)); // number
	}
	OpcodeData data(n);

	out << (byte)HEX_BYTE(0xD, regX) << (byte)HEX_BYTE(regY, data.n4);
}

/// 0xEx9E - skip next instruction if key Vx down
void fn_0xEx9E(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xE0 + regInd) << (byte)0x9E;
}

/// 0xExA1 - skip next instruction if key Vx up
void fn_0xExA1(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xE0 + regInd) << (byte)0xA1;
}

/// 0xFx07 - set Vx = delayTimer
void fn_0xFx07(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x07;
}

/// 0xFx0A - set Vx = key, wait for keypress
void fn_0xFx0A(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x0A;
}

/// 0xFx15 - set delayTimer = Vx
void fn_0xFx15(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x15;
}

/// 0xFx18 - set soundTimer = Vx
void fn_0xFx18(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x18;
}

/// 0xFx1E - I = I + VX; VF = 1 if I > 0xFFF else 0
void fn_0xFx1E(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x1E;
}

/// 0xFx29 - point I to 5 byte numeric sprite for value in VX
void fn_0xFx29(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x29;
}

/// 0xFx33 - store BCD of VX in [I], [I+1], [I+2]
void fn_0xFx33(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x33;
}

/// 0xFx55 - store V0 .. VX (inclusive) in [I] .. [I + X]
void fn_0xFx55(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x55;
}

/// 0xFx65 - read V0 .. VX (inclusive) from [I] .. [I + X]
void fn_0xFx65(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x65;
}

/************ Super CHIP-8 functions ************/

/// 0x00Bn: - scroll display N lines up (N/2 in low res mode)
void fn_0x00Bn(std::ofstream& out, int strNum) {
	ushort n;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// equ
	{
		n = equ[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		n = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}
	OpcodeData data(n);
	out << (byte)(0x00) << (byte)HEX_BYTE(0xB, data.n4);
}

/// 0x00Cn - scroll display N lines down (N/2 in low res mode)
void fn_0x00Cn(std::ofstream& out, int strNum) {
	ushort n;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// equ
	{
		n = equ[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		n = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}
	OpcodeData data(n);
	out << (byte)(0x00) << (byte)HEX_BYTE(0xC, data.n4);
}

/// 0x00FB - scroll right 4 pixels (2 pixels in low res mode)
void fn_0x00FB(std::ofstream& out, int strNum) {
	out << (byte)0x00 << (byte)0xFB;
}

/// 0x00FC - scroll left 4 pixels (2 pixels in low res mode)
void fn_0x00FC(std::ofstream& out, int strNum) {
	out << (byte)0x00 << (byte)0xFC;
}

/// 0x00FD - Quit the emulator
void fn_0x00FD(std::ofstream& out, int strNum) {
	out << (byte)0x00 << (byte)0xFD;
}

/// 0x00FE -  enter low res mode
void fn_0x00FE(std::ofstream& out, int strNum) {
	out << (byte)0x00 << (byte)0xFE;
}

/// 0x00FF -  enter high res mode
void fn_0x00FF(std::ofstream& out, int strNum) {
	out << (byte)0x00 << (byte)0xFF;
}

/// 0xFx30 - point I to 10 byte numeric sprite for value in VX
void fn_0xFx30(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x30;
}

/// 0xFx75 - save V0...VX (X<8) in the RPL flags
void fn_0xFx75(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x75;
}

/// FX85 - load V0...VX (X<8) from the RPL flags
void fn_0xFx85(std::ofstream& out, int strNum) {
	byte regInd;
	if (TOKEN(strNum, CMD_IND(strNum).first + 1).type == NAME)	// var
	{
		regInd = var[CMD_STR(strNum, CMD_IND(strNum).first + 1)];
	}
	else
	{
		regInd = (byte)strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 1)); // number
	}

	out << (byte)(0xF0 + regInd) << (byte)0x85;
}

/************ Extended CHIP-8 functions ************/
//TODO

#endif /* InstructionsAsm_h */
