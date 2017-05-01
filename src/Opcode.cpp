//
//  Opcode.cpp
//  Chip8
//

#include "Opcodes.h"

#ifdef CHIP_EMU
#include "emulator/Instructions.h"
#elif CHIP_DISASM
#include "disasm/Instructions.h"
#elif CHIP_ASM
#include "asm/Instructions.h"
#endif

Opcode::Opcode(const char* name, const OpMask mask, const std::vector<typeArg> arguments, const char* description, void (*exec)(EXEC_DESCR)) : exec(exec), name(name), mask(mask), arguments(arguments), description(description) {
}

#define k 0
#define x 0
#define y 0
#define n 0
#define A 0xA
#define B 0xB
#define C 0xC
#define D 0xD
#define E 0xE
#define F 0xF

INIT_OPS
const Opcode& getOpcode(const OpcodeData opcode) {
    switch (opcode.n1) {
        case 0x0:
            switch (opcode.n3) {
                case 0xB:
                case 0xC:
                    return ops[op(0, 0, opcode.n3, 0)];
            }
            return ops[op(0, 0, opcode.n3, opcode.n4)];
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            return ops[op(opcode.n1, 0, 0, 0)];
        case 0x8:
            return ops[op(opcode.n1, 0, 0, opcode.n4)];
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
            return ops[op(opcode.n1, 0, 0, 0)];
        case 0xE:
        case 0xF:
            return ops[op(opcode.n1, 0, opcode.n3, opcode.n4)];
        default:
            return ops[0xFFFF];
    }
};

std::map<ushort, Opcode>::iterator getBeginOps()
{
	return ops.begin();
}

std::map<ushort, Opcode>::iterator getEndOps()
{
	return ops.end();
}

#undef n
#undef k
#undef x
#undef y
#undef A
#undef B
#undef C
#undef D
#undef E
#undef F
