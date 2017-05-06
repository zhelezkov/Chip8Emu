//
//  CPU.cpp
//  Chip8Emulator
//

#include "CPU.hpp"

#include "Memory.hpp"
#include "GPU.hpp"
#include "TimersManager.hpp"
#include "Keyboard.hpp"
#include "APU.hpp"
#include "Opcodes.h"

CPU* CPU::instance = NULL;

CPU::CPU(Memory* mem, GPU* gpu, TimersManager* timers, Keyboard* keyboard, APU* apu) : memory(mem), gpu(gpu), timersManager(timers), keyboard(keyboard), apu(apu) {
    instance = this;
}

CPU::CPU() : CPU(new Memory(), new GPU(), new TimersManager(), new Keyboard(), new APU()) {
    
}

CPU::~CPU() {
    delete memory;
    delete gpu;
    delete timersManager;
    delete keyboard;
}

CPU* CPU::getInstance() {
    return instance;
}

void CPU::reset(bool resetMemory) {
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
	if (resetMemory)
		memory->reset();
    timersManager->reset();
    keyboard->reset();
	waitingForKey = false;
    registerWaitingKey = -1;
    
    LOG_F(INFO, "CPU reseted");
}

void CPU::tick() {
    if (waitingForKey) {
        return;
    }
    
    const ushort rawCode = getMemory()[PC] << 8 | getMemory()[PC + 1];
	PC += 2;

	const Opcode& opcode = getOpcode(rawCode);
    opcode.exec(this, rawCode);
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

APU& CPU::getApu() const {
    return *apu;
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

void CPU::waitForKey(byte index) {
    waitingForKey = true;
    registerWaitingKey = index;
}
