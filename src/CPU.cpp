//
//  CPU.cpp
//  Chip8Emulator
//

#include "CPU.hpp"

CPU::CPU() : opcode(getOpcode({0xffff})) {
    //PC = START_MEM;
}

/*void CPU::init()
{
							// Ô‡ÏˇÚ¸ Ì‡˜ËÌ‡ÂÚÒˇ Ò 0x200
	//opcode = ops[0xFFFF];					// Ò·‡Ò˚‚‡ÂÏ ÓÔÍÓ‰
	I = 0;									// Ò·‡Ò˚‚‡ÂÏ Â„ËÒÚ
	SP = 0;									// Ò·‡Ò˚‚‡ÂÏ stack pointer

	for(int i = 0; i < 16; i++)				// Ó·ÌÛÎˇÂÏ ‚ÒÂ Â„ËÒÚ˚
		V[i] = 0;

	for (int i = 0; i < 16; i++)			// Ó˜Ë˘‡ÂÏ ÒÚÂÍ
		stack[i] = 0;

	//memory.init();							// ËÌËˆË‡ÎËÁËÛÂÏ Ô‡ÏˇÚ¸
	//gpu.init();								// ËÌËˆË‡ÎËÁËÛÂÏ GPU
	//timer.init();							// ËÌËˆË‡ÎËÁËÛÂÏ Ú‡ÈÏÂ
	//board.init();							// ËÌËˆË‡ÎËÁËÛÂÏ ÍÎ‡‚Ë‡ÚÛÛ

	mode = 0;								// ÔÓ ÛÏÓÎ˜‡ÌË˛ chip48
}*/

void CPU::cycle()
{
	//static unsigned short rawCode = memory[PC] | memory[PC + 1];						// ÔÓÎÛ˜‡ÂÏ ÓÔÍÓ‰
	PC += 2;																			// ÔÂÂıÓ‰ËÏ Í ÒÎÂ‰Û˛˘ÂÈ ËÌÒÚÛÛˆËË

	//opcode = getOpCode({rawCode});														// ÌÛÊÌ˚È ÓÔÍÓ‰
	//opcode.exec(this);																	// ‚˚ÔÓÎÌËÎË ÓÔÍÓ‰

	//timer.update();																		// Ó·ÌÓ‚ËÎË Ú‡ÈÏÂ

}

/*const Memory& CPU::getMemory()
{
	//return memory;
}

const GPU& CPU::getGpu()
{
	//return gpu;
}

const Keyboard& CPU::getBoard()
{
	//return board;
}

const Timers& CPU::getTimers()
{
	//return timer;
}*/
