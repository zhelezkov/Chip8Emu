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
public:
    bool exit;
    
    CPU(Memory* mem, GPU* gpu, TimersManager* timers, Keyboard* keyboard);
    CPU();
    ~CPU();
    
    void reset();
    
    void tick();
    
    Memory& getMemory() const;
    GPU& getGpu() const;
    TimersManager& getTimersManager() const;
    Keyboard& getKeyboard() const;
    
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
    bool waitingForKey() { return registerWaitingKey >= 0; };
    
    void keyPress(ushort);
private:
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
    
    int registerWaitingKey;
};

#endif /* CPU_hpp */
