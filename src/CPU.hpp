//
//  CPU.hpp
//  Chip8Emulator
//
//  
//
//

#ifndef CPU_hpp
#define CPU_hpp

#include <stdio.h>
#include "Memory.hpp"
#include "GPU.hpp"
#include "Timers.hpp"
#include "Keyboard.hpp"
#include "OpCodes.hpp"

class CPU
{

private:
	OpCode opcode;					// двухбайтовый опкод
	unsigned char V[16];            // регистры общего назначения и флаг переноса VF  
	unsigned short I;               // адрессный регистр
	unsigned short PC;              // указатель кода
	unsigned char SP;               // указаель стека

	unsigned short stack[16];       // массив хранящий в себе стек

	Memory memory;			        // память
	GPU gpu;					    // графика
	Timers timer;					// таймеры
	Keyboard board;					// клавиатура


	int mode;						// 0 - chip48, 1 - superChip8

public:
	void init();					// инициализация переменных
	void cycle();					// один цикл

	const Memory& getMemory();
	const GPU& getGpu();
	const Keyboard& getBoard();
	const Timers& getTimers();
};
#endif /* CPU_hpp */
