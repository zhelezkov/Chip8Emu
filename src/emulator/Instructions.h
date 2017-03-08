//
//  Instructions.h
//  Chip8Emulator
//

#ifndef Instructions_h
#define Instructions_h

#include "CPU.hpp"
#include <cstdlib>

void fn_nop(CPU* const cpu, const OpcodeData data) {
    throw "Unknown op";
}

void fn_0x00E0(CPU* const cpu, const OpcodeData data) // 0x00E0: Clears the screen
{
	cpu->getGpu().clearScreen();
}

void fn_0x00EE(CPU* const cpu, const OpcodeData data) // Return from a subroutine.
{
	//PC = stack[--SP];

	byte index = cpu->getSP();
	ushort newPc = cpu->getStack(index - 1);
	cpu->setPC(newPc);
	cpu->setSP(index - 1);
}

// 00BN - scroll display N lines up *SUPER CHIP8*
// 00BN - scroll display N/2 lines up  *CHIP48*
void fn_0x00BN(CPU* const cpu, const OpcodeData data)
{
	// CHECK IT!
	byte N = data.n4;
	GPU& gpu = cpu->getGpu();
	byte height = gpu.getHeight();
	byte width = gpu.getWidth();

	N /= gpu.getResolutionMode();
	for (byte y = 0; y < height - N; y++)
		for (byte x = 0; x < width; x++)
			gpu.gfx[x][y] = gpu.gfx[x][y + N];

	// wipe the bottom-most pixels
	for (byte y = height - N; y < height; y++)
		for (byte x = 0; x < width; x++)
			gpu.gfx[x][y] = 0;
}

// 00CN - scroll display N lines down *SUPER CHIP8* 
// 00CN - scroll display N/2 lines down  *CHIP48*
void fn_0x00CN(CPU* const cpu, const OpcodeData data)
{
	// CHECK IT!
	byte N = data.n4;
	GPU& gpu = cpu->getGpu();
	byte height = gpu.getHeight();
	byte width = gpu.getWidth();

	N /= gpu.getResolutionMode();
	for (byte y = height - 1; y >= N; y--)
		for (byte x = 0; x < width; x++)
			gpu.gfx[x][y] = gpu.gfx[x][y - N];

	// wipe the top-most pixels
	for (byte y = 0; y < N; y++)
		for (byte x = 0; x < width; x++)
			gpu.gfx[x][y] = 0;
}

// 00FB - scroll display 4 pixels right *SUPER CHIP8*
// 00FB - scroll display 2 pixels right *CHIP48*
void fn_0x00FB(CPU* const cpu, const OpcodeData data) 
{
	// CHECK IT!
	GPU& gpu = cpu->getGpu();
	byte height = gpu.getHeight();
	byte width = gpu.getWidth();
	byte N = 4 / gpu.getResolutionMode();

	for (byte y = 0; y < height; y++)
	{
		for (byte x = width; x >= N; x--)
			gpu.gfx[x][y] = gpu.gfx[x - N][y];

		// wipe the first N pixels
		for(byte x = 0; x < N; x++)
			gpu.gfx[x][y] = 0;
	}
}

// 00FC - scroll display 4 pixels left *SUPER CHIP8*
// 00FC - scroll display 2 pixels left *CHIP48*
void fn_0x00FC(CPU* const cpu, const OpcodeData data)
{
	// CHECK IT!
	GPU& gpu = cpu->getGpu();
	byte height = gpu.getHeight();
	byte width = gpu.getWidth();
	byte N = 4 / gpu.getResolutionMode();

	for (byte y = 0; y < height; y++)
	{
		for (byte x = 0; x < width - N; x--)
			gpu.gfx[x][y] = gpu.gfx[x + N][y];

		// wipe the last N pixels
		for (byte x = width - N; x < width; x++)
			gpu.gfx[x][y] = 0;
	}
}

void fn_0x00FD(CPU* const cpu, const OpcodeData data) // 00FD - Quit the emulator *SUPER CHIP*
{
	cpu->exit = true;
}

void fn_0x00FE(CPU* const cpu, const OpcodeData data) // 00FE -  enable extended screen mode *CHIP48*
{
    cpu->getGpu().setResolutionMode(ResolutionMode::LOW);
}

void fn_0x00FF(CPU* const cpu, const OpcodeData data) // 00FE -  enable extended screen mode *SUPER CHIP*
{
    cpu->getGpu().setResolutionMode(ResolutionMode::HIGH);
}

void fn_0x1nnn(CPU* const cpu, const OpcodeData data) // 1NNN - jump to addr
{
	//PC = opcode & 0x0FFF;

	cpu->setPC(op(0, data.n2, data.n3, data.n4));
}

void fn_0x2nnn(CPU* const cpu, const OpcodeData data) // 2NNN - call subroutine
{
//	stack[SP++] = PC;
//	PC = opcode & 0x0FFF;

	byte index = cpu->getSP();
	cpu->setStack(index, cpu->getPC());
	cpu->setSP(index + 1);

	cpu->setPC(op(0, data.n2, data.n3, data.n4));
}

void fn_0x3xkk(CPU* const cpu, const OpcodeData data) // 3XKK - skip next instruction if VX == Byte
{
//	if (V[((opcode & 0x0F00) >> 8)] == (opcode & 0x00FF)) PC += 2;

	byte Vx = cpu->getRegisterV(op(0, data.n2, 0, 0));

	if (Vx == op(0, 0, data.n3, data.n4)) 
		cpu->setPC(cpu->getPC() + 2);
}

void fn_0x4xkk(CPU* const cpu, const OpcodeData data) // 4XKK - skip next instruction if VX != Byte
{
//	if (V[((opcode & 0x0F00) >> 8)] != (opcode & 0x00FF)) PC += 2;

	byte Vx = cpu->getRegisterV(op(0, data.n2, 0, 0));

	if (Vx != op(0, 0, data.n3, data.n4))
		cpu->setPC(cpu->getPC() + 2);
}

void fn_0x5xy0(CPU* const cpu, const OpcodeData data) // 5XY0 - skip next instruction if VX == VY
{
//	if (V[((opcode & 0x0F00) >> 8)] == V[((opcode & 0x00F0) >> 4)]) PC += 2;

	byte Vx = cpu->getRegisterV(op(0, data.n2, 0, 0));
	byte Vy = cpu->getRegisterV(op(0, 0, data.n3, 0));

	if (Vx == Vy) cpu->setPC(cpu->getPC() + 2);
}

void fn_0x6xkk(CPU* const cpu, const OpcodeData data) // 6XKK - set VX = Byte
{
//	V[((opcode & 0x0F00) >> 8)] = opcode & 0x00FF;

	byte index = op(0, data.n1, 0, 0);
	byte val = op(0, 0, data.n3, data.n4);

	cpu->setRegisterV(index, val);
}

void fn_0x7xkk(CPU* const cpu, const OpcodeData data) // 7XKK - set VX = VX + Byte
{
//	V[((opcode & 0x0F00) >> 8)] += opcode & 0x00FF;

	byte index = op(0, data.n1, 0, 0);
	byte val = op(0, 0, data.n3, data.n4) + cpu->getRegisterV(index);
	cpu->setRegisterV(index, val);
}

void fn_0x8xy0(CPU* const cpu, const OpcodeData data) // 8XY0 - set VX = VY
{
//	V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)];

	byte index_X = op(0, data.n1, 0, 0);
	byte index_Y = op(0, 0, data.n3, 0);
	byte val_Y = cpu->getRegisterV(index_Y);

	cpu->setRegisterV(index_X, val_Y);
}

void fn_0x8xy1(CPU* const cpu, const OpcodeData data) // 8XY1 - set VX = VX | VY
{
//	V[((opcode & 0x0F00) >> 8)] |= V[((opcode & 0x00F0) >> 4)];

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	byte index_Y = op(0, 0, data.n3, 0);
	byte val_Y = cpu->getRegisterV(index_Y);

	cpu->setRegisterV(index_X, val_X | val_Y);
}

void fn_0x8xy2(CPU* const cpu, const OpcodeData data) // 8XY2 - set VX = VX & VY
{
//	V[((opcode & 0x0F00) >> 8)] &= V[((opcode & 0x00F0) >> 4)];

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	byte index_Y = op(0, 0, data.n3, 0);
	byte val_Y = cpu->getRegisterV(index_Y);

	cpu->setRegisterV(index_X, val_X & val_Y);
}

void fn_0x8xy3(CPU* const cpu, const OpcodeData data) // 8XY3 - set VX = VX ^ VY
{
//	V[((opcode & 0x0F00) >> 8)] ^= V[((opcode & 0x00F0) >> 4)];

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	byte index_Y = op(0, 0, data.n3, 0);
	byte val_Y = cpu->getRegisterV(index_Y);

	cpu->setRegisterV(index_X, val_X ^ val_Y);
}

void fn_0x8xy4(CPU* const cpu, const OpcodeData data) // 8XY4 - set VX = VX + VY, VF = carry
{
//	int i;
//	i = (int)(V[((opcode & 0x0F00) >> 8)]) + (int)(V[((opcode & 0x00F0) >> 4)]);
//
//	if (i > 255)
//		V[0xF] = 1;
//	else
//		V[0xF] = 0;
//
//	V[((opcode & 0x0F00) >> 8)] = i;

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	byte index_Y = op(0, 0, data.n3, 0);
	byte val_Y = cpu->getRegisterV(index_Y);

	int i = (int)val_X + (int)val_Y;

	if (i > 255)
		cpu->setRegisterV(0xF, 1);
	else
		cpu->setRegisterV(0xF, 0);

	cpu->setRegisterV(index_X, i); // byte to int ? check
}

void fn_0x8xy5(CPU* const cpu, const OpcodeData data) // 8XY5 - set VX = VX - VY, VF = NOT borrow
{
//	if (V[((opcode & 0x0F00) >> 8)] > V[((opcode & 0x00F0) >> 4)]) //Vx > Vy
//		V[0xF] = 1;
//	else
//		V[0xF] = 0;
//
//	V[((opcode & 0x0F00) >> 8)] -= V[((opcode & 0x00F0) >> 4)]; //Vx - Vy

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	byte index_Y = op(0, 0, data.n3, 0);
	byte val_Y = cpu->getRegisterV(index_Y);

	if (val_X > val_Y)
		cpu->setRegisterV(0xF, 1);
	else
		cpu->setRegisterV(0xF, 0);

	cpu->setRegisterV(index_X, val_X - val_Y);
}

void fn_0x8xy6(CPU* const cpu, const OpcodeData data) // 8XY6 - set VX = VX >> 1, VF = least-significant bit
{
//	V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
//	V[(opcode & 0x0F00) >> 8] >>= 1;

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	cpu->setRegisterV(0xF, val_X & 0x1);
	cpu->setRegisterV(index_X, val_X >> 1);
}

void fn_0x8xy7(CPU* const cpu, const OpcodeData data) // 8XY7 - set VX = VY - VX, VF = NOT borrow
{
//	if (V[((opcode & 0x00F0) >> 4)] > V[((opcode & 0x0F00) >> 8)]) //Vy > Vx
//		V[0xF] = 1;
//	else
//		V[0xF] = 0;
//
//	V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)] - V[((opcode & 0x0F00) >> 8)]; //Vy - Vx

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	byte index_Y = op(0, 0, data.n3, 0);
	byte val_Y = cpu->getRegisterV(index_Y);

	if (val_Y > val_X)
		cpu->setRegisterV(0xF, 1);
	else
		cpu->setRegisterV(0xF, 0);

	cpu->setRegisterV(index_X, val_Y - val_X);
}

void fn_0x8xyE(CPU* const cpu, const OpcodeData data) // 8XYE - set VX = VX << 1, VF = most-significant bit
{
//	V[0xF] = (V[((opcode & 0x0F00) >> 8)] >> 7) & 0x1;
//	V[((opcode & 0x0F00) >> 8)] <<= 1;

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	cpu->setRegisterV(0xF, (val_X >> 7) & 0x1);
	cpu->setRegisterV(index_X, val_X << 1);
}

void fn_0x9xy0(CPU* const cpu, const OpcodeData data) // 9XY0 - skip next instruction if VX != VY
{
//	if (V[((opcode & 0x0F00) >> 8)] != V[((opcode & 0x00F0) >> 4)]) PC += 2;

	byte Vx = cpu->getRegisterV(op(0, data.n2, 0, 0));
	byte Vy = cpu->getRegisterV(op(0, 0, data.n3, 0));

	if (Vx != Vy) cpu->setPC(cpu->getPC() + 2);
}

void fn_0xAnnn(CPU* const cpu, const OpcodeData data) // ANNN - set I = Addr
{
//	I = opcode & 0x0FFF;
	
	ushort newAddr = op(0, data.n2, data.n3, data.n4);
	cpu->setAddrRegister(newAddr);
}

void fn_0xBnnn(CPU* const cpu, const OpcodeData data) // BNNN - jump to Addr + V0
{
//	PC = (opcode & 0x0FFF) + V[0];
	
	ushort newAddr = op(0, data.n2, data.n3, data.n4) + cpu->getRegisterV(0x0);
	cpu->setPC(newAddr);
}

void fn_0xCxkk(CPU* const cpu, const OpcodeData data) // CXKK - set Vx = random & kk
{
//	V[((opcode & 0x0F00) >> 8)] = (rand() % 255) & (opcode & 0x00FF);

	ushort b = op(0, 0, data.n3, data.n4);
    byte val = (std::rand() % 255) & b;
}

void fn_0xDxyn(CPU* const cpu, const OpcodeData data) // DXYN - Draw sprite
{
//	ushort x = V[(opcode & 0x0F00) >> 8];
//	ushort y = V[(opcode & 0x00F0) >> 4];
//	ushort height = opcode & 0x000F;
//	ushort pixel;
//
//	V[0xF] = 0;
//	for (int yline = 0; yline < height; yline++)
//	{
//		pixel = memory[I + yline];
//		for (int xline = 0; xline < 8; xline++)
//		{
//			if ((pixel & (0x80 >> xline)) != 0)
//			{
//				if (gfx[(x + xline) % 64 + (((y + yline) % 128) * 128)] == 1)
//					V[0xF] = 1;
//
//				gfx[(x + xline) % 64 + (((y + yline) % 128) * 128)] ^= 1;
//			}
//		}
//	}

	ushort x = cpu->getRegisterV(data.n2);
	ushort y = cpu->getRegisterV(data.n3);
	ushort n = data.n4;
	ushort I = cpu->getAddrRegister();
	ushort pixel;

	GPU& gpu = cpu->getGpu();
	Memory& memory = cpu->getMemory();

	cpu->setRegisterV(0xF, 0);
	if (n == 0) // 0xDXY0 - Draw a 16x16 sprite at I to VX, VY (8x16 in low res mode) *SUPER CHIP* 
	{
		for (byte yline = 0; yline < 16; yline++)
		{
			pixel = memory[I + 2 * yline];
			for (byte xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if (gpu.gfx[(x + xline) % gpu.getWidth()][(y + yline) % gpu.getHeight()] == 1)
						cpu->setRegisterV(0xF, 1);

					gpu.gfx[(x + xline) % gpu.getWidth()][(y + yline) % gpu.getHeight()] ^= 1;
				}
			}

			pixel = memory[I + 1 + 2 * yline];
			for (byte xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if (gpu.gfx[(x + xline + 8) % gpu.getWidth()][(y + yline) % gpu.getHeight()] == 1)
						cpu->setRegisterV(0xF, 1);

					gpu.gfx[(x + xline + 8) % gpu.getWidth()][(y + yline) % gpu.getHeight()] ^= 1;
				}
			}
		}
	}
	else
	{
		for(byte yline = 0; yline < n; yline++)
		{
			pixel = memory[I + yline];
			for (byte xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if(gpu.gfx[(x + xline) % gpu.getWidth()][(y + yline) % gpu.getHeight()] == 1)
						cpu->setRegisterV(0xF, 1);

					gpu.gfx[(x + xline) % gpu.getWidth()][(y + yline) % gpu.getHeight()] ^= 1;
				}
			}
		}
	}
}

void fn_0xEx9E(CPU* const cpu, const OpcodeData data) // EX9E - skip next instruction if key VX down
{
//	if (key[V[((opcode & 0x0F00) >> 8)]] == 1)
//		PC += 2;

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	if (cpu->getKeyboard().isPressed(val_X) == 1)
		cpu->setPC(cpu->getPC() + 2);
}

void fn_0xExA1(CPU* const cpu, const OpcodeData data) // EXA1 - skip next instruction if key VX up
{
//	if (key[V[((opcode & 0x0F00) >> 8)]] == 0)
//		PC += 2;

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	if (cpu->getKeyboard().isPressed(val_X) == 0)
		cpu->setPC(cpu->getPC() + 2);
}

void fn_0xFx07(CPU* const cpu, const OpcodeData data) // FX07 - set VX = delaytimer
{
//	V[((opcode & 0x0F00) >> 8)] = delay_timer;

	byte index_X = op(0, data.n1, 0, 0);

	cpu->setRegisterV(index_X, cpu->getTimers().getDelayTimer());
}

void fn_0xFx0A(CPU* const cpu, const OpcodeData data) // FX0A - set VX = key, wait for keypress
{
//	PC -= 2;
//	for (byte n = 0; n < 16; n++)
//	{
//		if (key[n] == 1)
//		{
//			V[((opcode & 0x0F00) >> 8)] = n;
//			PC += 2;
//			break;
//		}
//	}

	cpu->setPC(cpu->getPC() - 2);
	for (byte n = 0; n < 16; n++)
	{
		if (cpu->getKeyboard().isPressed(n) == 1)
		{
			byte index_X = op(0, data.n1, 0, 0);
			cpu->setRegisterV(index_X, n);

			cpu->setPC(cpu->getPC() + 2);
			break;
		}
	}
}

void fn_0xFx15(CPU* const cpu, const OpcodeData data) // FX15 - set delaytimer = VX
{
//	delay_timer = V[((opcode & 0x0F00) >> 8)];

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	cpu->getTimers().setDelayTimer(val_X);
}

void fn_0xFx18(CPU* const cpu, const OpcodeData data) // FX18 - set soundtimer = VX
{
//	sound_timer = V[((opcode & 0x0F00) >> 8)];

	byte index_X = op(0, data.n1, 0, 0);
	byte val_X = cpu->getRegisterV(index_X);

	cpu->getTimers().setSoundTimer(val_X);
}

void fn_0xFx1E(CPU* const cpu, const OpcodeData data) // FX1E - set I = I + VX; set VF if buffer overflow
{
//	if ((I += V[((opcode & 0x0F00) >> 8)]) > 0xFFF)
//		V[0xF] = 1;
//	else
//		V[0xF] = 0;

	byte index_X = op(0, data.n1, 0, 0);
	ushort i = cpu->getAddrRegister() + cpu->getRegisterV(index_X);

	if (i > 0xFFF)
		cpu->setRegisterV(0xF, 1);
	else
		cpu->setRegisterV(0xF, 0);

	cpu->setAddrRegister(i);
}

void fn_0xFx29(CPU* const cpu, const OpcodeData data) // FX29 - point I to 5 byte numeric sprite for value in VX
{
//	I = V[((opcode & 0x0F00) >> 8)] * 0x5;

	byte index_X = op(0, data.n1, 0, 0);
	ushort Vx = cpu->getRegisterV(index_X);

	cpu->setAddrRegister(Vx * 0x5);
}

void fn_0xFx33(CPU* const cpu, const OpcodeData data) // FX33 - store BCD of VX in [I], [I+1], [I+2]
{
//	memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
//	memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
//	memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;

	ushort I = cpu->getAddrRegister();
	byte index_X = op(0, data.n2, 0, 0);
	byte Vx = cpu->getRegisterV(index_X);

	cpu->getMemory()[I] = Vx  / 100;
	cpu->getMemory()[I + 1] = (Vx / 10) % 10;
	cpu->getMemory()[I + 2] = (Vx % 100) % 10; //CHECK!
}

void fn_0xFx55(CPU* const cpu, const OpcodeData data) // FX55 - store V0 .. VX in [I] .. [I+X]
{
//	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
//		memory[I + i] = V[i];

	byte index_X = op(0, data.n2, 0, 0);
	ushort I = cpu->getAddrRegister();

	for (byte i = 0; i <= index_X; i++)
		cpu->getMemory()[I + i] = cpu->getRegisterV(i);
}

void fn_0xFx65(CPU* const cpu, const OpcodeData data) // FX65 - read V0 ..VX from[I] ..[I + X]
{
//	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
//		V[i] = memory[I + i];

	byte index_X = op(0, data.n2, 0, 0);
	ushort I = cpu->getAddrRegister();

	for (byte i = 0; i <= index_X; i++)
		cpu->setRegisterV(i, cpu->getMemory()[I + i]);
}

void fn_0xFx30(CPU* const cpu, const OpcodeData data) // FX30 - point I to 10 byte numeric sprite for value in VX *SUPER CHIP*
{
	byte index_X = op(0, data.n2, 0, 0);
	byte Vx = cpu->getRegisterV(index_X);

	cpu->setAddrRegister(Vx * 10 + 80);
}

void fn_0xFx75(CPU* const cpu, const OpcodeData data) // FX75 - save V0...VX (X<8) in the RPL flags *SUPER CHIP*
{
	byte index_X = op(0, data.n2, 0, 0);

	for (byte i = 0; i <= index_X; i++)
		cpu->setRegisterR(i, cpu->getRegisterV(i));
}

void fn_0xFx85(CPU* const cpu, const OpcodeData data) // FX85 - load V0...VX (X<8) from the HP48 flags *SUPER CHIP*
{
	byte index_X = op(0, data.n2, 0, 0);

	for (byte i = 0; i <= index_X; i++)
		cpu->setRegisterV(i, cpu->getRegisterR(i));
}

#endif /* Instructions_h */
