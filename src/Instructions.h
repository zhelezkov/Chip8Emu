//
//  Instructions.h
//  Chip8Emulator
//
//
//
//

#ifndef Instructions_h
#define Instructions_h

#include "OpCodes.hpp"
#include "CPU.hpp"

void Fun_0x00E0(CPU* cpu) // 0x00E0: Clears the screen
{
	for (int y = 0; y < 64; y++)
		for (int x = 0; x < 128; x++)
			gfx[x][y] = 0;
}

void Fun_0x00EE(CPU* cpu) // Clears the screen
{
	PC = stack[--SP];
}

void Fun_0x1nnn(CPU* cpu) // 1NNN - jump to addr
{
	PC = opcode & 0x0FFF;
}

void Fun_0x2nnn(CPU* cpu) // 2NNN - call subroutine
{
	stack[SP++] = PC;
	PC = opcode & 0x0FFF;
}

void Fun_0x3xkk(CPU* cpu) // 3XKK - skip next instruction if VX == Byte
{
	if (V[((opcode & 0x0F00) >> 8)] == (opcode & 0x00FF)) PC += 2;
}

void Fun_0x4xkk(CPU* cpu) // 4XKK - skip next instruction if VX != Byte
{
	if (V[((opcode & 0x0F00) >> 8)] != (opcode & 0x00FF)) PC += 2;
}

void Fun_0x5xy0(CPU* cpu) // 5XY0 - skip next instruction if VX == VY
{
	if (V[((opcode & 0x0F00) >> 8)] == V[((opcode & 0x00F0) >> 4)]) PC += 2;
}

void Fun_0x6xkk(CPU* cpu) // 6XKK - set VX = Byte
{
	V[((opcode & 0x0F00) >> 8)] = opcode & 0x00FF;
}

void Fun_0x7xkk(CPU* cpu) // 7XKK - set VX = VX + Byte
{
	V[((opcode & 0x0F00) >> 8)] += opcode & 0x00FF;
}

void Fun_0x8xy0(CPU* cpu) // 8XY0 - set VX = VY
{
	V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)];
}

void Fun_0x8xy1(CPU* cpu) // 8XY1 - set VX = VX | VY
{
	V[((opcode & 0x0F00) >> 8)] |= V[((opcode & 0x00F0) >> 4)];
}

void Fun_0x8xy2(CPU* cpu) // 8XY2 - set VX = VX & VY
{
	V[((opcode & 0x0F00) >> 8)] &= V[((opcode & 0x00F0) >> 4)];
}

void Fun_0x8xy3(CPU* cpu) // 8XY3 - set VX = VX ^ VY
{
	V[((opcode & 0x0F00) >> 8)] ^= V[((opcode & 0x00F0) >> 4)];
}

void Fun_0x8xy4(CPU* cpu) // 8XY4 - set VX = VX + VY, VF = carry
{
	int i;
	i = (int)(V[((opcode & 0x0F00) >> 8)]) + (int)(V[((opcode & 0x00F0) >> 4)]);

	if (i > 255)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[((opcode & 0x0F00) >> 8)] = i;
}

void Fun_0x8xy5(CPU* cpu) // 8XY5 - set VX = VX - VY, VF = NOT borrow
{
	if (V[((opcode & 0x0F00) >> 8)] > V[((opcode & 0x00F0) >> 4)]) //Vx > Vy
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[((opcode & 0x0F00) >> 8)] -= V[((opcode & 0x00F0) >> 4)]; //Vx - Vy
}

void Fun_0x8xy6(CPU* cpu) // 8XY6 - set VX = VX >> 1, VF = least-significant bit
{
	V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
	V[(opcode & 0x0F00) >> 8] >>= 1;
}

void Fun_0x8xy7(CPU* cpu) // 8XY7 - set VX = VY - VX, VF = NOT borrow
{
	if (V[((opcode & 0x00F0) >> 4)] > V[((opcode & 0x0F00) >> 8)]) //Vy > Vx
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)] - V[((opcode & 0x0F00) >> 8)]; //Vy - Vx
}

void Fun_0x8xyE(CPU* cpu) // 8XYE - set VX = VX << 1, VF = most-significant bit
{
	V[0xF] = (V[((opcode & 0x0F00) >> 8)] >> 7) & 0x1;
	V[((opcode & 0x0F00) >> 8)] <<= 1;
}

void Fun_0x9xy0(CPU* cpu) // 9XY0 - skip next instruction if VX != VY
{
	if (V[((opcode & 0x0F00) >> 8)] != V[((opcode & 0x00F0) >> 4)]) PC += 2;
}

void Fun_0xAnnn(CPU* cpu) // ANNN - set I = Addr
{
	I = opcode & 0x0FFF;
}

void Fun_0xBnnn(CPU* cpu) // BNNN - jump to Addr + V0
{
	PC = (opcode & 0x0FFF) + V[0];
}

void Fun_0xCxkk(CPU* cpu) // CXKK - set Vx = random & kk
{
	V[((opcode & 0x0F00) >> 8)] = (rand() % 255) & (opcode & 0x00FF);
}

void Fun_0xDxyn(CPU* cpu) // DXYN - Draw sprite
{
	unsigned short x = V[(opcode & 0x0F00) >> 8];
	unsigned short y = V[(opcode & 0x00F0) >> 4];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;

	V[0xF] = 0;
	for (int yline = 0; yline < height; yline++)
	{
		pixel = memory[I + yline];
		for (int xline = 0; xline < 8; xline++)
		{
			if ((pixel & (0x80 >> xline)) != 0)
			{
				if (gfx[(x + xline) % 64 + (((y + yline) % 128) * 128)] == 1)
					V[0xF] = 1;

				gfx[(x + xline) % 64 + (((y + yline) % 128) * 128)] ^= 1;
			}
		}
	}
}

void Fun_0xEx9E(CPU* cpu) // EX9E - skip next instruction if key VX down
{
	if (key[V[((opcode & 0x0F00) >> 8)]] == 1)
		PC += 2;
}

void Fun_0xExA1(CPU* cpu) // EXA1 - skip next instruction if key VX up
{
	if (key[V[((opcode & 0x0F00) >> 8)]] == 0)
		PC += 2;
}

void Fun_0xFx07(CPU* cpu) // FX07 - set VX = delaytimer
{
	V[((opcode & 0x0F00) >> 8)] = delay_timer;
}

void Fun_0xFx0A(CPU* cpu) // FX0A - set VX = key, wait for keypress
{
	PC -= 2;
	for (unsigned char n = 0; n < 16; n++)
	{
		if (key[n] == 1)
		{
			V[((opcode & 0x0F00) >> 8)] = n;
			PC += 2;
			break;
		}
	}
}

void Fun_0xFx15(CPU* cpu) // FX15 - set delaytimer = VX
{
	delay_timer = V[((opcode & 0x0F00) >> 8)];
}

void Fun_0xFx18(CPU* cpu) // FX18 - set soundtimer = VX
{
	sound_timer = V[((opcode & 0x0F00) >> 8)];
}

void Fun_0xFx1E(CPU* cpu) // FX1E - set I = I + VX; set VF if buffer overflow
{
	if ((I += V[((opcode & 0x0F00) >> 8)]) > 0xFFF)
		V[0xF] = 1;
	else
		V[0xF] = 0;
}

void Fun_0xFx29(CPU* cpu) // FX29 - point I to 5 byte numeric sprite for value in VX
{
	I = V[((opcode & 0x0F00) >> 8)] * 0x5;
}

void Fun_0xFx33(CPU* cpu) // FX33 - store BCD of VX in [I], [I+1], [I+2]
{
	memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
	memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
	memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
}

void Fun_0xFx55(CPU* cpu) // FX55 - store V0 .. VX in [I] .. [I+X]
{
	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
		memory[I + i] = V[i];
}

void Fun_0xFx65(CPU* cpu) // FX65 - read V0 ..VX from[I] ..[I + X]
{
	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
		V[i] = memory[I + i];
}

#endif /* Instructions_h */
