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

/// 0x00E0: Clears the screen
void fn_0x00E0(CPU* const cpu, const OpcodeData data) {
	cpu->getGpu().clearScreen();
}

/// 0x00EE: Return from a subroutine
void fn_0x00EE(CPU* const cpu, const OpcodeData data) {
	byte index = cpu->getSP();
	ushort newPc = cpu->getStack(index - 1);
	cpu->setPC(newPc);
	cpu->setSP(index - 1);
}

/// 0x00Bn: - scroll display N lines up (N/2 in low res mode)
void fn_0x00Bn(CPU* const cpu, const OpcodeData data) {
	// CHECK IT!
	byte N = data.n4;
	GPU& gpu = cpu->getGpu();
	int height = gpu.getHeight();
	int width = gpu.getWidth();

	N /= gpu.getResolutionMode();
    for (int y = 0; y < height - N; y++) {
        for (int x = 0; x < width; x++) {
            byte pix = gpu.getPixel(x, y + N);
            gpu.setPixel(pix, x, y);
        }
    }

	// wipe the bottom-most pixels
    for (int y = height - N; y < height; y++)
        for (int x = 0; x < width; x++)
            gpu.setPixel(0, x, y);

}

/// 0x00Cn - scroll display N lines down (N/2 in low res mode)
void fn_0x00Cn(CPU* const cpu, const OpcodeData data) {
	// CHECK IT!
	byte N = data.n4;
	GPU& gpu = cpu->getGpu();
	int height = gpu.getHeight();
	int width = gpu.getWidth();

	N /= gpu.getResolutionMode();
    for (int y = height - 1; y >= N; y--) {
        for (int x = 0; x < width; x++) {
            byte pix = gpu.getPixel(x, y - N);
            gpu.setPixel(pix, x, y);
        }
    }

	// wipe the top-most pixels
	for (int y = 0; y < N; y++)
		for (int x = 0; x < width; x++)
            gpu.setPixel(0, x, y);
}

/// 0x00FB - scroll right 4 pixels (2 pixels in low res mode)
void fn_0x00FB(CPU* const cpu, const OpcodeData data) {
	// CHECK IT!
	GPU& gpu = cpu->getGpu();
	int height = gpu.getHeight();
	int width = gpu.getWidth();
	byte N = 4 / gpu.getResolutionMode();

	for (int y = 0; y < height; y++) {
        for (int x = width; x >= N; x--) {
            byte pix = gpu.getPixel(x - N, y);
            gpu.setPixel(pix, x, y);
        }

		// wipe the first N pixels
		for(int x = 0; x < N; x++)
            gpu.setPixel(0, x, y);
	}
}

/// 0x00FC - scroll left 4 pixels (2 pixels in low res mode)
void fn_0x00FC(CPU* const cpu, const OpcodeData data) {
	// CHECK IT!
	GPU& gpu = cpu->getGpu();
	int height = gpu.getHeight();
	int width = gpu.getWidth();
	byte N = 4 / gpu.getResolutionMode();

	for (int y = 0; y < height; y++) {
        for (int x = 0; x < width - N; x--) {
            byte pix = gpu.getPixel(x + N, y);
            gpu.setPixel(pix, x, y);
        }

		// wipe the last N pixels
		for (int x = width - N; x < width; x++)
            gpu.setPixel(0, x, y);
	}
}

/// 0x00FD - Quit the emulator
void fn_0x00FD(CPU* const cpu, const OpcodeData data) {
	cpu->exit = true;
}

/// 0x00FE -  enter low res mode
void fn_0x00FE(CPU* const cpu, const OpcodeData data) {
    cpu->getGpu().setResolutionMode(ResolutionMode::LOW);
}

/// 0x00FE -  enter high res mode
void fn_0x00FF(CPU* const cpu, const OpcodeData data) {
    cpu->getGpu().setResolutionMode(ResolutionMode::HIGH);
}

/// 0x1nnn - jump to addr nnn
void fn_0x1nnn(CPU* const cpu, const OpcodeData data) {
	cpu->setPC(op(0, data.n2, data.n3, data.n4));
}

/// 0x2nnn - call subroutine at nnn
void fn_0x2nnn(CPU* const cpu, const OpcodeData data) {
    cpu->pushStack();
	cpu->setPC(op(0, data.n2, data.n3, data.n4));
}

/// 0x3xkk - skip next instruction if VX == kk
void fn_0x3xkk(CPU* const cpu, const OpcodeData data) {
	byte Vx = cpu->getRegisterV(data.n2);

	if (Vx == op(0, 0, data.n3, data.n4)) 
		cpu->setPC(cpu->getPC() + 2);
}

/// 0x4xkk - skip next instruction if VX != kk
void fn_0x4xkk(CPU* const cpu, const OpcodeData data) {
	byte Vx = cpu->getRegisterV(op(0, data.n2, 0, 0));

	if (Vx != op(0, 0, data.n3, data.n4))
		cpu->setPC(cpu->getPC() + 2);
}

/// 0x5xy0 - skip next instruction if VX == VY
void fn_0x5xy0(CPU* const cpu, const OpcodeData data) {
	byte Vx = cpu->getRegisterV(data.n2);
	byte Vy = cpu->getRegisterV(data.n3);

	if (Vx == Vy) cpu->setPC(cpu->getPC() + 2);
}

/// 0x6xkk - set Vx = kk
void fn_0x6xkk(CPU* const cpu, const OpcodeData data) {
	byte val = op(0, 0, data.n3, data.n4);

	cpu->setRegisterV(data.n2, val);
}

/// 0x7xkk - set VX += kk
void fn_0x7xkk(CPU* const cpu, const OpcodeData data) {
	byte val = op(0, 0, data.n3, data.n4) + cpu->getRegisterV(data.n2);
    
	cpu->setRegisterV(data.n2, val);
}

/// 0x8xy0 - set Vx = Vy
void fn_0x8xy0(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte indexY = data.n3;
	byte valY = cpu->getRegisterV(indexY);

	cpu->setRegisterV(indexX, valY);
}

/// 0x8xy1 - set Vx = Vx | Vy
void fn_0x8xy1(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	byte indexY = data.n3;
	byte valY = cpu->getRegisterV(indexY);

	cpu->setRegisterV(indexX, valX | valY);
}

/// 0x8xy2 - set Vx = Vx & Vy
void fn_0x8xy2(CPU* const cpu, const OpcodeData data) {
    byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	byte indexY = data.n3;
	byte valY = cpu->getRegisterV(indexY);

	cpu->setRegisterV(indexX, valX & valY);
}

/// 8xy3 - set Vx = Vx ^ Vy
void fn_0x8xy3(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	byte indexY = data.n3;
	byte valY = cpu->getRegisterV(indexY);

	cpu->setRegisterV(indexX, valX ^ valY);
}

/// 0x8xy4 - set Vx = Vx + Vy, VF = 1 if overflow else VF = 0
void fn_0x8xy4(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);
    
	byte indexY = data.n3;
	byte valY = cpu->getRegisterV(indexY);

	int res = (int)valX + (int)valY;

	if (res > 255)
		cpu->setRegisterV(0xF, 1);
	else
		cpu->setRegisterV(0xF, 0);

	cpu->setRegisterV(indexX, res); // byte to int ? check
}

/// 0x8xy5 - set Vx = Vx - Vy, VF = 1 if not borrow else 0
void fn_0x8xy5(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	byte indexY = data.n3;
	byte valY = cpu->getRegisterV(indexY);

	if (valX > valY)
		cpu->setRegisterV(0xF, 1);
	else
		cpu->setRegisterV(0xF, 0);

	cpu->setRegisterV(indexX, valX - valY);
}

/// 8xy6 - set Vx = Vx >> 1, VF = least-significant bit
void fn_0x8xy6(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	cpu->setRegisterV(0xF, valX & 1);
	cpu->setRegisterV(indexX, valX >> 1);
}

/// 0x8xy7 - set Vx = Vy - Vx, VF = 1 if not borrow else 0
void fn_0x8xy7(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	byte indexY = data.n3;
	byte valY = cpu->getRegisterV(indexY);

	if (valY > valX)
		cpu->setRegisterV(0xF, 1);
	else
		cpu->setRegisterV(0xF, 0);

	cpu->setRegisterV(indexX, valY - valX);
}

/// 0x8xyE - set VX = VX << 1, VF = most-significant bit
void fn_0x8xyE(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	cpu->setRegisterV(0xF, (valX >> 7) & 0x1);
	cpu->setRegisterV(indexX, valX << 1);
}

/// 0x9xy0 - skip next instruction if Vx != Vy
void fn_0x9xy0(CPU* const cpu, const OpcodeData data) {
	byte Vx = cpu->getRegisterV(data.n2);
	byte Vy = cpu->getRegisterV(data.n3);

	if (Vx != Vy) cpu->setPC(cpu->getPC() + 2);
}

/// 0xAnnn - set I = Addr
void fn_0xAnnn(CPU* const cpu, const OpcodeData data) {
	ushort newAddr = op(0, data.n2, data.n3, data.n4);
	cpu->setAddrRegister(newAddr);
}
/// 0xBnnn - jump to Addr + V0
void fn_0xBnnn(CPU* const cpu, const OpcodeData data) {
	ushort newAddr = op(0, data.n2, data.n3, data.n4) + cpu->getRegisterV(0x0);
	cpu->setPC(newAddr);
}

/// 0xCxkk - set Vx = random & kk
void fn_0xCxkk(CPU* const cpu, const OpcodeData data) {
	ushort b = op(0, 0, data.n3, data.n4);
    byte val = (std::rand() % 255) & b;
    cpu->setRegisterV(data.n2, val);
}

/// 0xDxyn - Draw 8xN sprite at I to VX, VY; VF = 1 if collision else 0
//TODO: CHECK THIS FCKING FUNCTION VERY CAREFULLY
void fn_0xDxyn(CPU* const cpu, const OpcodeData data) {
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
	byte pixel;

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
					if (gpu.getPixel((x + xline) % gpu.getWidth(), (y + yline) % gpu.getHeight()) == 1)
						cpu->setRegisterV(0xF, 1);

					//gpu.xorPixel((x + xline) % gpu.getWidth(), (y + yline) % gpu.getHeight());
				}
			}

			pixel = memory[I + 1 + 2 * yline];
			for (byte xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if (gpu.getPixel((x + xline + 8) % gpu.getWidth(), (y + yline) % gpu.getHeight()) == 1)
						cpu->setRegisterV(0xF, 1);

					//gpu.xorPixel((x + xline + 8) % gpu.getWidth(), (y + yline) % gpu.getHeight()) ^= 1;
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
					if(gpu.getPixel((x + xline) % gpu.getWidth(), (y + yline) % gpu.getHeight()) == 1)
						cpu->setRegisterV(0xF, 1);

					//gpu.xorPixel((x + xline) % gpu.getWidth(), (y + yline) % gpu.getHeight()) ^= 1;
				}
			}
		}
	}
}

/// 0xEx9E - skip next instruction if key Vx down
void fn_0xEx9E(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	if (cpu->getKeyboard().isPressed(valX))
		cpu->setPC(cpu->getPC() + 2);
}

/// 0xExA1 - skip next instruction if key Vx up
void fn_0xExA1(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	if (!cpu->getKeyboard().isPressed(valX))
		cpu->setPC(cpu->getPC() + 2);
}

/// 0xFx07 - set Vx = delayTimer
void fn_0xFx07(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;

	cpu->setRegisterV(indexX, cpu->getTimers().getDelayTimer());
}

/// 0xFx0A - set Vx = key, wait for keypress
void fn_0xFx0A(CPU* const cpu, const OpcodeData data) {
    //TODO make proper way to wait, check PC

	cpu->setPC(cpu->getPC() - 2);
	for (byte n = 0; n < 16; n++)
	{
		if (cpu->getKeyboard().isPressed(n))
		{
			byte indexX = data.n2;
			cpu->setRegisterV(indexX, n);

			cpu->setPC(cpu->getPC() + 2);
			break;
		}
	}
}

/// 0xFx15 - set delayTimer = Vx
void fn_0xFx15(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	cpu->getTimers().setDelayTimer(valX);
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
