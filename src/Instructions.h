//
//  Instructions.h
//  Chip8Emulator
//

#ifndef Instructions_h
#define Instructions_h

//#include "Opcodes.hpp"
#include "CPU.hpp"

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

	unsigned char index = cpu->getSP();
	unsigned short newPc = cpu->getStack(index - 1);
	cpu->setPC(newPc);
	cpu->setSP(index - 1);
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

	unsigned char index = cpu->getSP();
	cpu->setStack(index, cpu->getPC());
	cpu->setSP(index + 1);

	cpu->setPC(op(0, data.n2, data.n3, data.n4));
}

void fn_0x3xkk(CPU* const cpu, const OpcodeData data) // 3XKK - skip next instruction if VX == Byte
{
//	if (V[((opcode & 0x0F00) >> 8)] == (opcode & 0x00FF)) PC += 2;

	unsigned char Vx = cpu->getRegister(op(0, data.n2, 0, 0));

	if (Vx == op(0, 0, data.n3, data.n4)) 
		cpu->setPC(cpu->getPC() + 2);
}

void fn_0x4xkk(CPU* const cpu, const OpcodeData data) // 4XKK - skip next instruction if VX != Byte
{
//	if (V[((opcode & 0x0F00) >> 8)] != (opcode & 0x00FF)) PC += 2;

	unsigned char Vx = cpu->getRegister(op(0, data.n2, 0, 0));

	if (Vx != op(0, 0, data.n3, data.n4))
		cpu->setPC(cpu->getPC() + 2);
}

void fn_0x5xy0(CPU* const cpu, const OpcodeData data) // 5XY0 - skip next instruction if VX == VY
{
//	if (V[((opcode & 0x0F00) >> 8)] == V[((opcode & 0x00F0) >> 4)]) PC += 2;

	unsigned char Vx = cpu->getRegister(op(0, data.n2, 0, 0));
	unsigned char Vy = cpu->getRegister(op(0, 0, data.n3, 0));

	if (Vx == Vy) cpu->setPC(cpu->getPC() + 2);
}

void fn_0x6xkk(CPU* const cpu, const OpcodeData data) // 6XKK - set VX = Byte
{
//	V[((opcode & 0x0F00) >> 8)] = opcode & 0x00FF;

	unsigned char index = op(0, data.n1, 0, 0);
	unsigned char val = op(0, 0, data.n3, data.n4);

	cpu->setRegister(index, val);
}

void fn_0x7xkk(CPU* const cpu, const OpcodeData data) // 7XKK - set VX = VX + Byte
{
//	V[((opcode & 0x0F00) >> 8)] += opcode & 0x00FF;

	unsigned char index = op(0, data.n1, 0, 0);
	unsigned char val = op(0, 0, data.n3, data.n4) + cpu->getRegister(index);
	cpu->setRegister(index, val);
}

void fn_0x8xy0(CPU* const cpu, const OpcodeData data) // 8XY0 - set VX = VY
{
//	V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)];

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char index_Y = op(0, 0, data.n3, 0);
	unsigned char val_Y = cpu->getRegister(index_Y);

	cpu->setRegister(index_X, val_Y);
}

void fn_0x8xy1(CPU* const cpu, const OpcodeData data) // 8XY1 - set VX = VX | VY
{
//	V[((opcode & 0x0F00) >> 8)] |= V[((opcode & 0x00F0) >> 4)];

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	unsigned char index_Y = op(0, 0, data.n3, 0);
	unsigned char val_Y = cpu->getRegister(index_Y);

	cpu->setRegister(index_X, val_X | val_Y);
}

void fn_0x8xy2(CPU* const cpu, const OpcodeData data) // 8XY2 - set VX = VX & VY
{
//	V[((opcode & 0x0F00) >> 8)] &= V[((opcode & 0x00F0) >> 4)];

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	unsigned char index_Y = op(0, 0, data.n3, 0);
	unsigned char val_Y = cpu->getRegister(index_Y);

	cpu->setRegister(index_X, val_X & val_Y);
}

void fn_0x8xy3(CPU* const cpu, const OpcodeData data) // 8XY3 - set VX = VX ^ VY
{
//	V[((opcode & 0x0F00) >> 8)] ^= V[((opcode & 0x00F0) >> 4)];

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	unsigned char index_Y = op(0, 0, data.n3, 0);
	unsigned char val_Y = cpu->getRegister(index_Y);

	cpu->setRegister(index_X, val_X ^ val_Y);
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

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	unsigned char index_Y = op(0, 0, data.n3, 0);
	unsigned char val_Y = cpu->getRegister(index_Y);

	int i = (int)val_X + (int)val_Y;

	if (i > 255)
		cpu->setRegister(0xF, 1);
	else
		cpu->setRegister(0xF, 0);

	cpu->setRegister(index_X, i); // unsigned char to int ? check
}

void fn_0x8xy5(CPU* const cpu, const OpcodeData data) // 8XY5 - set VX = VX - VY, VF = NOT borrow
{
//	if (V[((opcode & 0x0F00) >> 8)] > V[((opcode & 0x00F0) >> 4)]) //Vx > Vy
//		V[0xF] = 1;
//	else
//		V[0xF] = 0;
//
//	V[((opcode & 0x0F00) >> 8)] -= V[((opcode & 0x00F0) >> 4)]; //Vx - Vy

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	unsigned char index_Y = op(0, 0, data.n3, 0);
	unsigned char val_Y = cpu->getRegister(index_Y);

	if (val_X > val_Y)
		cpu->setRegister(0xF, 1);
	else
		cpu->setRegister(0xF, 0);

	cpu->setRegister(index_X, val_X - val_Y);
}

void fn_0x8xy6(CPU* const cpu, const OpcodeData data) // 8XY6 - set VX = VX >> 1, VF = least-significant bit
{
//	V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
//	V[(opcode & 0x0F00) >> 8] >>= 1;

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	cpu->setRegister(0xF, val_X & 0x1);
	cpu->setRegister(index_X, val_X >> 1);
}

void fn_0x8xy7(CPU* const cpu, const OpcodeData data) // 8XY7 - set VX = VY - VX, VF = NOT borrow
{
//	if (V[((opcode & 0x00F0) >> 4)] > V[((opcode & 0x0F00) >> 8)]) //Vy > Vx
//		V[0xF] = 1;
//	else
//		V[0xF] = 0;
//
//	V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)] - V[((opcode & 0x0F00) >> 8)]; //Vy - Vx

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	unsigned char index_Y = op(0, 0, data.n3, 0);
	unsigned char val_Y = cpu->getRegister(index_Y);

	if (val_Y > val_X)
		cpu->setRegister(0xF, 1);
	else
		cpu->setRegister(0xF, 0);

	cpu->setRegister(index_X, val_Y - val_X);
}

void fn_0x8xyE(CPU* const cpu, const OpcodeData data) // 8XYE - set VX = VX << 1, VF = most-significant bit
{
//	V[0xF] = (V[((opcode & 0x0F00) >> 8)] >> 7) & 0x1;
//	V[((opcode & 0x0F00) >> 8)] <<= 1;

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	cpu->setRegister(0xF, (val_X >> 7) & 0x1);
	cpu->setRegister(index_X, val_X << 1);
}

void fn_0x9xy0(CPU* const cpu, const OpcodeData data) // 9XY0 - skip next instruction if VX != VY
{
//	if (V[((opcode & 0x0F00) >> 8)] != V[((opcode & 0x00F0) >> 4)]) PC += 2;

	unsigned char Vx = cpu->getRegister(op(0, data.n2, 0, 0));
	unsigned char Vy = cpu->getRegister(op(0, 0, data.n3, 0));

	if (Vx != Vy) cpu->setPC(cpu->getPC() + 2);
}

void fn_0xAnnn(CPU* const cpu, const OpcodeData data) // ANNN - set I = Addr
{
//	I = opcode & 0x0FFF;
	
	unsigned short newAddr = op(0, data.n2, data.n3, data.n4);
	cpu->setAddrRegister(newAddr);
}

void fn_0xBnnn(CPU* const cpu, const OpcodeData data) // BNNN - jump to Addr + V0
{
//	PC = (opcode & 0x0FFF) + V[0];
	
	unsigned short newAddr = op(0, data.n2, data.n3, data.n4) + cpu->getRegister(0x0);
	cpu->setPC(newAddr);
}

void fn_0xCxkk(CPU* const cpu, const OpcodeData data) // CXKK - set Vx = random & kk
{
//	V[((opcode & 0x0F00) >> 8)] = (rand() % 255) & (opcode & 0x00FF);

	unsigned short b = op(0, 0, data.n3, data.n4);
	unsigned char val = (rand() % 255) & b;
}

void fn_0xDxyn(CPU* const cpu, const OpcodeData data) // DXYN - Draw sprite
{
//	unsigned short x = V[(opcode & 0x0F00) >> 8];
//	unsigned short y = V[(opcode & 0x00F0) >> 4];
//	unsigned short height = opcode & 0x000F;
//	unsigned short pixel;
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

	cpu->setRegister(0xF, 0);
	for(int y = 0; y < cpu->getGpu().getHeight(); y++)
	{
		for (int x = 0; x < cpu->getGpu().getWidth(); x++)
		{
			
		}
	}
}

void fn_0xEx9E(CPU* const cpu, const OpcodeData data) // EX9E - skip next instruction if key VX down
{
//	if (key[V[((opcode & 0x0F00) >> 8)]] == 1)
//		PC += 2;

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	if (cpu->getBoard().isPressed(val_X) == 1)
		cpu->setPC(cpu->getPC() + 2);
}

void fn_0xExA1(CPU* const cpu, const OpcodeData data) // EXA1 - skip next instruction if key VX up
{
//	if (key[V[((opcode & 0x0F00) >> 8)]] == 0)
//		PC += 2;

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	if (cpu->getBoard().isPressed(val_X) == 0)
		cpu->setPC(cpu->getPC() + 2);
}

void fn_0xFx07(CPU* const cpu, const OpcodeData data) // FX07 - set VX = delaytimer
{
//	V[((opcode & 0x0F00) >> 8)] = delay_timer;

	unsigned char index_X = op(0, data.n1, 0, 0);

	cpu->setRegister(index_X, cpu->getTimers().getDelayTimer());
}

void fn_0xFx0A(CPU* const cpu, const OpcodeData data) // FX0A - set VX = key, wait for keypress
{
//	PC -= 2;
//	for (unsigned char n = 0; n < 16; n++)
//	{
//		if (key[n] == 1)
//		{
//			V[((opcode & 0x0F00) >> 8)] = n;
//			PC += 2;
//			break;
//		}
//	}

	cpu->setPC(cpu->getPC() - 2);
	for (unsigned char n = 0; n < 16; n++)
	{
		if (cpu->getBoard().isPressed(n) == 1)
		{
			unsigned char index_X = op(0, data.n1, 0, 0);
			cpu->setRegister(index_X, n);

			cpu->setPC(cpu->getPC() + 2);
			break;
		}
	}
}

void fn_0xFx15(CPU* const cpu, const OpcodeData data) // FX15 - set delaytimer = VX
{
//	delay_timer = V[((opcode & 0x0F00) >> 8)];

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	cpu->getTimers().setDelayTimer(val_X);
}

void fn_0xFx18(CPU* const cpu, const OpcodeData data) // FX18 - set soundtimer = VX
{
//	sound_timer = V[((opcode & 0x0F00) >> 8)];

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned char val_X = cpu->getRegister(index_X);

	cpu->getTimers().setSoundTimer(val_X);
}

void fn_0xFx1E(CPU* const cpu, const OpcodeData data) // FX1E - set I = I + VX; set VF if buffer overflow
{
//	if ((I += V[((opcode & 0x0F00) >> 8)]) > 0xFFF)
//		V[0xF] = 1;
//	else
//		V[0xF] = 0;

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned short i = cpu->getAddrRegister() + cpu->getRegister(index_X);

	if (i > 0xFFF)
		cpu->setRegister(0xF, 1);
	else
		cpu->setRegister(0xF, 0);

	cpu->setAddrRegister(i);
}

void fn_0xFx29(CPU* const cpu, const OpcodeData data) // FX29 - point I to 5 byte numeric sprite for value in VX
{
//	I = V[((opcode & 0x0F00) >> 8)] * 0x5;

	unsigned char index_X = op(0, data.n1, 0, 0);
	unsigned short i = cpu->getRegister(index_X);

	cpu->setAddrRegister(i * 0x5);
}

void fn_0xFx33(CPU* const cpu, const OpcodeData data) // FX33 - store BCD of VX in [I], [I+1], [I+2]
{
//	memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
//	memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
//	memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;

	unsigned short I = cpu->getAddrRegister();
	unsigned char index_X = op(0, data.n2, 0, 0);
	unsigned char Vx = cpu->getRegister(index_X);

	cpu->getMemory()[I] = Vx  / 100;
	cpu->getMemory()[I + 1] = (Vx / 10) % 10;
	cpu->getMemory()[I + 2] = (Vx % 100) % 10; //CHECK!
}

void fn_0xFx55(CPU* const cpu, const OpcodeData data) // FX55 - store V0 .. VX in [I] .. [I+X]
{
//	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
//		memory[I + i] = V[i];

	unsigned char index_X = op(0, data.n2, 0, 0);
	unsigned char Vx = cpu->getRegister(index_X);
	unsigned short I = cpu->getAddrRegister();

	for (unsigned char i = 0; i <= index_X; i++)
		cpu->getMemory()[I + i] = cpu->getRegister(i);
}

void fn_0xFx65(CPU* const cpu, const OpcodeData data) // FX65 - read V0 ..VX from[I] ..[I + X]
{
//	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
//		V[i] = memory[I + i];

	unsigned char index_X = op(0, data.n2, 0, 0);
	unsigned char Vx = cpu->getRegister(index_X);
	unsigned short I = cpu->getAddrRegister();

	for (unsigned char i = 0; i <= index_X; i++)
		cpu->setRegister(i, cpu->getMemory()[I + i]);
}

#endif /* Instructions_h */
