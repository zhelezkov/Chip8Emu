//
//  CPU.hpp
//  Chip8Emulator
//

#ifndef CPU_hpp
#define CPU_hpp

//#include "Memory.hpp"
//#include "GPU.hpp"
//#include "Timers.hpp"
//#include "Keyboard.hpp"
#include "Opcodes.h"

class Opcode;

class CPU
{

private:
	const Opcode& opcode;
	unsigned char V[16];
	unsigned short I;
	unsigned short PC;
	unsigned char SP;

	unsigned short stack[16];

	//Memory memory;
	//GPU gpu;
	//Timers timer;
	//Keyboard board;

	int mode;						// 0 - chip48, 1 - superChip8

public:
    CPU();
	void cycle();
	/*const Memory& getMemory();
	const GPU& getGpu();
	const Keyboard& getBoard();
	const Timers& getTimers();*/
    static CPU* instance;
    static CPU* getInstance() { return CPU::instance; };
};

#endif /* CPU_hpp */
