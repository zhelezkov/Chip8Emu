//
//  CPU.hpp
//  Chip8Emulator
//

#ifndef CPU_hpp
#define CPU_hpp

#include "Memory.hpp"
#include "GPU.hpp"
#include "TimersManager.hpp"
//#include "Keyboard.hpp"
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
	//Keyboard board;

	CHIPMode mode;
    
    static CPU* instance;
public:
    CPU(CHIPMode mode, Memory& mem, GPU& gpu, TimersManager& timers);
	void cycle();
    //do we need getters??
	/*const Memory& getMemory();
	const GPU& getGpu();
	const Keyboard& getBoard();
	const Timers& getTimers();*/
    static CPU* getInstance() { return CPU::instance; };
};

#endif /* CPU_hpp */
