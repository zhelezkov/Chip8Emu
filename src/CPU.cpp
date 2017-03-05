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

Memory& CPU::getMemory() const
{
	return memory;
}

GPU& CPU::getGpu() const
{
	return gpu;
}

TimersManager& CPU::getTimers() const
{
	return timersManager;
}

Keyboard& CPU::getBoard() const
{
	return board;
}

unsigned char CPU::getRegister(unsigned char index) const
{
	if(index < 0 || index >= REGISTERS_COUNT) throw "Bad index";
	return V[index];
}

void CPU::setRegister(unsigned char index, unsigned char val)
{
	if (index < 0 || index >= REGISTERS_COUNT) throw "Bad index";
	V[index] = val;
}

unsigned short CPU::getAddrRegister() const
{
	return I;
}

void CPU::setAddrRegister(unsigned short val)
{
	I = val;
}

unsigned short CPU::getPC() const
{
	return PC;
}

void CPU::setPC(unsigned short val)
{
	PC = val;
}

unsigned char CPU::getSP() const
{
	return SP;
}

void CPU::setSP(unsigned char val)
{
	SP = val;
}


unsigned short CPU::getStack(unsigned char index) const
{
	if (index < 0 || index >= STACK_SIZE) throw "Bad index";
	return stack[index];
}

unsigned short CPU::setStack(unsigned char index, unsigned short val)
{
	if (index < 0 || index >= STACK_SIZE) throw "Bad index";
	stack[index] = val;
}