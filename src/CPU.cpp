//
//  CPU.cpp
//  Chip8Emulator
//

#include "CPU.hpp"

CPU* CPU::instance = nullptr;

CPU::CPU(CHIPMode mode, Memory& mem, GPU& gpu, TimersManager& timers) :memory(mem), gpu(gpu), timersManager(timers), mode(mode){
    instance = this;
    PC = MEM_START;
    I = 0;
    SP = 0;
    for (int i = 0; i < REGISTERS_COUNT; i++)
        V[i] = 0;
    for (int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;
    
    gpu.setMode(mode);
}

void CPU::cycle()
{
    const unsigned short rawCode = memory[PC] | memory[PC + 1];
	PC += 2;

	const Opcode& opcode = getOpcode(rawCode);
    
	opcode.exec(this, rawCode);

	//timer.update();

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
