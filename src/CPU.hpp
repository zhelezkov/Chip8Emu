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

class CPU {
private:
	unsigned char V[REGISTERS_COUNT];
	unsigned short I;
	unsigned short PC;
	unsigned char SP;

	unsigned short stack[STACK_SIZE];

	Memory& memory;
	GPU& gpu;
	TimersManager& timersManager;
	Keyboard& board;

	CHIPMode mode;
    
    static CPU* instance;
public:
    CPU(CHIPMode mode, Memory& mem, GPU& gpu, TimersManager& timers);
	void cycle();
	Memory& getMemory() const;
	GPU& getGpu() const;
	TimersManager& getTimers() const;
	Keyboard& getBoard() const;

	unsigned char getRegister(unsigned char index) const;
	void setRegister(unsigned char index, unsigned char val);

	unsigned short getAddrRegister() const;
	void setAddrRegister(unsigned short val);

	unsigned short getPC() const;
	void setPC(unsigned short val);

	unsigned char getSP() const;
	void setSP(unsigned char val);

	unsigned short getStack(unsigned char index) const;
	unsigned short setStack(unsigned char index, unsigned short val);

    static CPU* getInstance() { return CPU::instance; };
};

#endif /* CPU_hpp */
