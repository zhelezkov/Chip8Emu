//
//  CPU.hpp
//  Chip8Emulator
//

#ifndef CPU_hpp
#define CPU_hpp

#include "CHIP.h"

#define STACK_SIZE 16
#define V_REGISTERS_COUNT 16
#define R_REGISTERS_COUNT 8

class CPU {
    friend class Keyboard;
    
public:
    bool exit;
    
    CPU(Memory* mem, GPU* gpu, TimersManager* timers, Keyboard* keyboard, APU* apu);
    CPU();
    ~CPU();
    
    static CPU* getInstance();
    
    void reset();
    
    void tick();
    
    Memory& getMemory() const;
    GPU& getGpu() const;
    TimersManager& getTimersManager() const;
    Keyboard& getKeyboard() const;
    APU& getApu() const;
    
    byte getRegisterV(byte index) const;
    void setRegisterV(byte index, byte val);
    
    ushort getAddrRegister() const;
    void setAddrRegister(ushort val);
    
    ushort getPC() const;
    void setPC(ushort val);
    
    byte getSP() const;
    void setSP(byte val);
    
    ushort popStack();
    void pushStack();
    
    byte getRegisterR(byte index) const;
    void setRegisterR(byte index, byte val);
    
    void waitForKey(byte index);
private:
    static CPU* instance;
	byte V[V_REGISTERS_COUNT];
    byte R[R_REGISTERS_COUNT];
    
	ushort I;
	ushort PC;
	byte SP;

	ushort stack[STACK_SIZE];

	Memory* memory;
	GPU* gpu;
	TimersManager* timersManager;
	Keyboard* keyboard;
    APU* apu;
    
    bool waitingForKey;
    byte registerWaitingKey;
};

#endif /* CPU_hpp */
