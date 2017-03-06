//
//  Opcode.cpp
//  Chip8
//

#include "Opcode.hpp"
#include "Instructions.h"

Opcode::Opcode(const char* name, const OpMask mask, const char* description, void (*exec)(CPU* const, const OpcodeData), void (*write)()) : exec(exec), write(write), name(name), mask(mask), description(description) {
}

#define nnn 000
#define kk 00
#define x 0
#define y 0
#define n 0
#define E 0xE
#define A 0xA

const Opcode& getOpcode(const OpcodeData opcode) {
    INIT_OPS
    switch (opcode.n1) {
        case 0x0:
            return ops[opcode.rawCode];
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

#undef nnn
#undef kk
#undef x
#undef y
#undef E
#undef A
