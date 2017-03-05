//
//  CPU.cpp
//  Chip8Emulator
//
//
//
//

#include "CPU.hpp"

void CPU::init()
{
	PC = 0x200;								// память начинается с 0x200
	opcode = ops[0xFFFF];					// сбрасываем опкод
	I = 0;									// сбрасываем регистр
	SP = 0;									// сбрасываем stack pointer

	for(int i = 0; i < 16; i++)				// обнуляем все регистры
		V[i] = 0;

	for (int i = 0; i < 16; i++)			// очищаем стек
		stack[i] = 0;

	memory.init();							// инициализируем память
	gpu.init();								// инициализируем GPU
	timer.init();							// инициализируем таймер
	board.init();							// инициализируем клавиатуру

	mode = 0;								// по умолчанию chip48
}

void CPU::cycle()
{
	static unsigned short rawCode = memory[PC] | memory[PC + 1];						// получаем опкод
	PC += 2;																			// переходим к следующей инстуруции

	opcode = getOpCode({rawCode});														// нужный опкод
	opcode.exec(this);																	// выполнили опкод

	timer.update();																		// обновили таймер

}

const Memory& CPU::getMemory()
{
	return memory;
}

const GPU& CPU::getGpu()
{
	return gpu;
}

const Keyboard& CPU::getBoard()
{
	return board;
}

const Timers& CPU::getTimers()
{
	return timer;
}