//
//  CPU.cpp
//  Chip8Emulator
//

#include "CPU.hpp"

CPU* CPU::instance = nullptr;

CPU::CPU(CHIPMode mode, Memory& mem, GPU& gpu, TimersManager& timers, Keyboard& keyboard) :memory(mem), gpu(gpu), timersManager(timers), keyboard(keyboard), mode(mode){
    instance = this;
    PC = MEM_START;
    I = 0;
    SP = 0;
    for (int i = 0; i < REGISTERS_COUNT; i++)
        V[i] = 0;
    for (int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;
    
	for (int i = 0; i < RPL_COUNT; i++)
		RPL_flags[i] = 0;

    gpu.setMode(mode);

	exit = false;
}

void CPU::cycle()
{
    const ushort rawCode = memory[PC] | memory[PC + 1];
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

Keyboard& CPU::getKeyboard() const
{
	return keyboard;
}

byte CPU::getRegister(byte index) const
{
	if(index < 0 || index >= REGISTERS_COUNT) throw "Bad index";
	return V[index];
}

void CPU::setRegister(byte index, byte val)
{
	if (index < 0 || index >= REGISTERS_COUNT) throw "Bad index";
	V[index] = val;
}

ushort CPU::getAddrRegister() const
{
	return I;
}

void CPU::setAddrRegister(ushort val)
{
	I = val;
}

ushort CPU::getPC() const
{
	return PC;
}

void CPU::setPC(ushort val)
{
	PC = val;
}

byte CPU::getSP() const
{
	return SP;
}

void CPU::setSP(byte val)
{
	SP = val;
}


ushort CPU::getStack(byte index) const
{
	if (index < 0 || index >= STACK_SIZE) throw "Bad index";
	return stack[index];
}

void CPU::setStack(byte index, ushort val)
{
	if (index < 0 || index >= STACK_SIZE) throw "Bad index";
	stack[index] = val;
}

ushort CPU::getRPL(byte index) const
{
	if (index < 0 || index >= RPL_COUNT) throw "Bad index";
	return RPL_flags[index];
}

void CPU::setRPL(byte index, byte val)
{
	if (index < 0 || index >= RPL_COUNT) throw "Bad index";
	RPL_flags[index] = val;
}