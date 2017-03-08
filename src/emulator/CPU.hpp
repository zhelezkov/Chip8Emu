//
//  CPU.hpp
//  Chip8Emulator
//

#ifndef CPU_hpp
#define CPU_hpp

#include "Memory.hpp"
#include "GPU.hpp"
#include "TimersManager.hpp"
#include "Keyboard.hpp"
#include "CHIP.h"
#include "Opcodes.h"

#define STACK_SIZE 16
#define REGISTERS_COUNT 16
#define RPL_COUNT 8

class CPU {
private:
	byte V[REGISTERS_COUNT];
	ushort I;
	ushort PC;
	byte SP;

	ushort stack[STACK_SIZE];
	byte RPL_flags[RPL_COUNT];

	Memory& memory;
	GPU& gpu;
	TimersManager& timersManager;
	Keyboard& keyboard;

	CHIPMode mode;
    
    static CPU* instance;
public:	
	bool exit;

    CPU(CHIPMode mode, Memory& mem, GPU& gpu, TimersManager& timers, Keyboard& keyboard);
	void cycle();
	Memory& getMemory() const;
	GPU& getGpu() const;
	TimersManager& getTimers() const;
	Keyboard& getKeyboard() const;

	byte getRegister(byte index) const;
	void setRegister(byte index, byte val);

	ushort getAddrRegister() const;
	void setAddrRegister(ushort val);

	ushort getPC() const;
	void setPC(ushort val);

	byte getSP() const;
	void setSP(byte val);

	ushort getStack(byte index) const;
	void setStack(byte index, ushort val);

	ushort getRPL(byte index) const;
	void setRPL(byte index, byte val);

    static CPU* getInstance() { return CPU::instance; };
};

#endif /* CPU_hpp */
