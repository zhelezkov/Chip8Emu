//
//  CPU.cpp
//  Chip8Emulator
//

#include "CPU.hpp"

CPU::CPU(Memory& mem, GPU& gpu, TimersManager& timers, Keyboard& keyboard) :memory(mem), gpu(gpu), timersManager(timers), keyboard(keyboard) {
    
    reset();

	exit = false;
}

void CPU::reset() {
    PC = MEM_START;
    I = 0;
    SP = 0;
    for (int i = 0; i < V_REGISTERS_COUNT; i++)
        V[i] = 0;
    for (int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;
    for (int i = 0; i < R_REGISTERS_COUNT; i++)
        R[i] = 0;
    
    gpu.reset();
    memory.reset();
    timersManager.reset();
    keyboard.reset();
}

void CPU::tick()
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

byte CPU::getRegisterV(byte index) const
{
	if(index < 0 || index >= V_REGISTERS_COUNT) throw "Bad index";
	return V[index];
}

void CPU::setRegisterV(byte index, byte val)
{
	if (index < 0 || index >= V_REGISTERS_COUNT) throw "Bad index";
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


ushort CPU::popStack() {
	return stack[SP--];
}

void CPU::pushStack() {
    stack[SP++] = PC;
}

ushort CPU::getRegisterR(byte index) const
{
	if (index < 0 || index >= R_REGISTERS_COUNT) throw "Bad index";
	return R[index];
}

void CPU::setRegisterR(byte index, byte val)
{
	if (index < 0 || index >= R_REGISTERS_COUNT) throw "Bad index";
	R[index] = val;
}
