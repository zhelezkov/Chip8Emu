//
//  CPU.cpp
//  Chip8Emulator
//

#include "Memory.hpp"
#include "GPU.hpp"
#include "TimersManager.hpp"
#include "Keyboard.hpp"
#include "Opcodes.h"

#include "CPU.hpp"

CPU::CPU(Memory* mem, GPU* gpu, TimersManager* timers, Keyboard* keyboard) :memory(mem), gpu(gpu), timersManager(timers), keyboard(keyboard) {
}

CPU::CPU() : memory(new Memory()), gpu(new GPU()), timersManager(new TimersManager()), keyboard(new Keyboard()) {
}

CPU::~CPU() {
    delete memory;
    delete gpu;
    delete timersManager;
    delete keyboard;
}

void CPU::reset() {
    exit = false;
    PC = MEM_START;
    I = 0;
    SP = 0;
    for (int i = 0; i < V_REGISTERS_COUNT; i++)
        V[i] = 0;
    for (int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;
    for (int i = 0; i < R_REGISTERS_COUNT; i++)
        R[i] = 0;
    
    gpu->reset();
    memory->reset();
    timersManager->reset();
    keyboard->reset();
    
    LOG_F(INFO, "CPU reseted");
}

void CPU::tick() {
    const ushort rawCode = getMemory()[PC] << 8 | getMemory()[PC + 1];
	PC += 2;

	const Opcode& opcode = getOpcode(rawCode);
    //printf("PC: %d, OP: %s RawCode: %d\n", PC, opcode.getName(), rawCode);
    opcode.exec(this, rawCode);

	//timersManager->update();

}

Memory& CPU::getMemory() const {
	return *memory;
}

GPU& CPU::getGpu() const {
	return *gpu;
}

TimersManager& CPU::getTimersManager() const {
	return *timersManager;
}

Keyboard& CPU::getKeyboard() const {
	return *keyboard;
}

byte CPU::getRegisterV(byte index) const {
	assert(index >= 0 && index < V_REGISTERS_COUNT);
	return V[index];
}

void CPU::setRegisterV(byte index, byte val)
{
	assert(index >= 0 && index < V_REGISTERS_COUNT);
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
	return stack[--SP];
}

void CPU::pushStack() {
    stack[SP++] = PC;
}

byte CPU::getRegisterR(byte index) const {
	assert(index >= 0 && index < R_REGISTERS_COUNT);
	return R[index];
}

void CPU::setRegisterR(byte index, byte val) {
	assert(index >= 0 && index < R_REGISTERS_COUNT);
	R[index] = val;
}
