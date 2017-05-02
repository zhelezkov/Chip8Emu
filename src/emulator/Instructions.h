//
//  Instructions.h
//  Chip8Emulator
//

#ifndef Instructions_h
#define Instructions_h

//TODO make instructions.cpp and keep it as header only

#include <cstdlib>
#include "CPU.hpp"
#include "GPU.hpp"
#include "Memory.hpp"
#include "Keyboard.hpp"
#include "TimersManager.hpp"

void fn_nop(CPU* const cpu, const OpcodeData data) {
    //throw "Unknown op";
}

/// 0x00E0 - clears the screen
void fn_0x00E0(CPU* const cpu, const OpcodeData data) {
	cpu->getGpu().clearScreen();
}

/// 0x00EE - return from a subroutine
void fn_0x00EE(CPU* const cpu, const OpcodeData data) {
    cpu->setPC(cpu->popStack());
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
	byte Vx = cpu->getRegisterV(data.n2);

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

    cpu->setRegisterV(0xF, valX > ((valX + valY) & 0xFF));

	cpu->setRegisterV(indexX, valX + valY);
}

/// 0x8xy5 - set Vx = Vx - Vy, VF = 1 if not borrow else 0
void fn_0x8xy5(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	byte indexY = data.n3;
	byte valY = cpu->getRegisterV(indexY);

    cpu->setRegisterV(0xF, valX > valY);

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

    cpu->setRegisterV(0xF, valY > valX);

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
	/*if (n == 0) // 0xDXY0 - Draw a 16x16 sprite at I to VX, VY (8x16 in low res mode) *SUPER CHIP*
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
                    else
                        cpu->setRegisterV(0xF, 0);
					//gpu.xorPixel((x + xline + 8) % gpu.getWidth(), (y + yline) % gpu.getHeight()) ^= 1;
				}
			}
		}
	}
	else {*/
		for(byte yLine = 0; yLine < n; yLine++) {
			pixel = memory[I + yLine];
			for (byte xLine = 0; xLine < 8; xLine++) {
				if ((pixel & (0x80 >> xLine)) != 0) {
                    if(gpu.getPixel((x + xLine) % (gpu.getWidth()), (y + yLine) % (gpu.getHeight())) == 1)
                        cpu->setRegisterV(0xF, 1);
                    //else
                    //    cpu->setRegisterV(0xF, 0);
                    
                    //printf("XORING: %d %d", (x + xLine) % gpu.getWidth(), (y + yLine) % gpu.getHeight());
					gpu.xorPixel((x + xLine) % (gpu.getWidth()), (y + yLine) % (gpu.getHeight()));
				}
			}
		}
	//}
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

	cpu->setRegisterV(indexX, cpu->getTimersManager().getDelayTimer());
}

/// 0xFx0A - set Vx = key, wait for keypress
void fn_0xFx0A(CPU* const cpu, const OpcodeData data) {
    cpu->waitForKey(data.n2);
}

/// 0xFx15 - set delayTimer = Vx
void fn_0xFx15(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	cpu->getTimersManager().setDelayTimer(valX);
}

/// 0xFx18 - set soundTimer = Vx
void fn_0xFx18(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	byte valX = cpu->getRegisterV(indexX);

	cpu->getTimersManager().setSoundTimer(valX);
}

/// 0xFx1E - I = I + VX; VF = 1 if I > 0xFFF else 0
void fn_0xFx1E(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	ushort i = cpu->getAddrRegister() + cpu->getRegisterV(indexX);

	if (i > 0xFFF)
		cpu->setRegisterV(0xF, 1);
	else
		cpu->setRegisterV(0xF, 0);

	cpu->setAddrRegister(i);
}

/// 0xFx29 - point I to 5 byte numeric sprite for value in VX
void fn_0xFx29(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	ushort Vx = cpu->getRegisterV(indexX);

	cpu->setAddrRegister(Vx * 0x5);
}

/// 0xFx33 - store BCD of VX in [I], [I+1], [I+2]
void fn_0xFx33(CPU* const cpu, const OpcodeData data) {
	ushort I = cpu->getAddrRegister();
	byte indexX = data.n2;
	byte Vx = cpu->getRegisterV(indexX);
    Memory& mem = cpu->getMemory();

	mem[I] = Vx / 100;
	mem[I + 1] = (Vx / 10) % 10;
	mem[I + 2] = Vx % 10;
}

/// 0xFx55 - store V0 .. VX (inclusive) in [I] .. [I + X]
void fn_0xFx55(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	ushort I = cpu->getAddrRegister();
    Memory& mem = cpu->getMemory();
    
	for (byte i = 0; i <= indexX; i++)
		mem[I + i] = cpu->getRegisterV(i);
}

/// 0xFx65 - read V0 .. VX (inclusive) from [I] .. [I + X]
void fn_0xFx65(CPU* const cpu, const OpcodeData data) {
	byte indexX = data.n2;
	ushort I = cpu->getAddrRegister();
    Memory& mem = cpu->getMemory();

	for (byte i = 0; i <= indexX; i++)
		cpu->setRegisterV(i, mem[I + i]);
}

/************ Super CHIP-8 functions ************/

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

/// 0x00FF -  enter high res mode
void fn_0x00FF(CPU* const cpu, const OpcodeData data) {
    cpu->getGpu().setResolutionMode(ResolutionMode::HIGH);
}

/// 0xFx30 - point I to 10 byte numeric sprite for value in VX
void fn_0xFx30(CPU* const cpu, const OpcodeData data) {
    byte indexX = data.n2;
    byte Vx = cpu->getRegisterV(indexX);
    
    cpu->setAddrRegister(Vx * 10 + 80); //TODO CHECK
}

/// 0xFx75 - save V0...VX (X<8) in the RPL flags
void fn_0xFx75(CPU* const cpu, const OpcodeData data) {
    byte indexX = data.n2;
    //TODO assert x < 8
    for (byte i = 0; i <= indexX; i++)
        cpu->setRegisterR(i, cpu->getRegisterV(i));
}

/// FX85 - load V0...VX (X<8) from the RPL flags
void fn_0xFx85(CPU* const cpu, const OpcodeData data) {
    byte indexX = data.n2;
    
    for (byte i = 0; i <= indexX; i++)
        cpu->setRegisterV(i, cpu->getRegisterR(i));
}

/************ Extended CHIP-8 functions ************/
//TODO

#endif /* Instructions_h */
