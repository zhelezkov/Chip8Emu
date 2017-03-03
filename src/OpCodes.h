//
//  OpCodes.h
//  Chip8Emulator
//
//  Created by rsredsq on 02/03/2017.
//
//

#ifndef OpCodes_h
#define OpCodes_h

#include <map>

enum OpMask {
    None = 0,
    Addr = 1, // address
    VxByte = 1, //Vx, byte
    Vx = 1, // Vx
    VxVy = 2, // Vx, Vy
    IAddr = 2, // I, address
    V0Addr = 2, // V0, address
    VxVyNib = 3, // Vx, Vy, nibble
    VxDT = 2, // Vx, DT
    VxK = 2, // Vx, K
    DTVx = 2, // DT, Vx
    STVx = 2, // ST, Vx
    IVx = 2, // I, Vx
    FVx = 2, // F, Vx
    BVx = 2, // B, Vx
    VxI = 2, // Vx, I
};

class OpCode {
public:
    OpCode() : OpCode("", None, "") {
        
    }
    
    OpCode(const char* name, const OpMask mask, const char* description, const void (*exec)(void) = nullptr, const void (*write)(void) = nullptr, const void(*read)(void) = nullptr) : name(name), mask(mask), description(description), exec(exec), write(write), read(read) {
        bestVal = -999;
    }
    int bestVal;
public:
    const char* name;
    const OpMask mask;
    const char* description;
    const void (*exec)(void);
    const void (*write)(void);
    const void (*read)(void);
};

using address = unsigned short;
using reg = unsigned char;

#define BEGIN_OPS static std::map<unsigned char, OpCode> ops = {
#define OP(addr, name, mask, descr) {addr, OpCode(name, mask, descr) }
#define END_OPS };

#define nnn 000
#define kk 00
#define x 0
#define y 0
#define n 0
#define E 0xE
#define A 0xA

BEGIN_OPS
    OP(0x0|nnn, "SYS", None, "[Deprecated] Jump to a machine code routine at nnn."),
    OP(0x00E0, "CLS", None, "Clear the display."),
    OP(0x00EE, "RET", None, "Return from a subroutine."),
    OP(0x1|nnn, "JP", Addr, "Jump to location nnn."),
    OP(0x2|nnn, "CALL", Addr, "Call subroutine at nnn."),
    OP(0x3|x|kk, "SE", VxByte, "Skip next instruction if Vx = kk."),
    OP(0x4|x|kk, "SNE", VxByte, "Skip next instruction if Vx != kk."),
    OP(0x5|x|y|0, "SE", VxVy, "Skip next instruction if Vx = Vy."),
    OP(0x6|x|kk, "LD", VxByte, "Set Vx = kk."),
    OP(0x7|x|kk, "ADD", VxByte, "Set Vx = Vx + kk."),
    OP(0x8|x|y|0, "LD", VxVy, "Set Vx = Vy."),
    OP(0x8|x|y|1, "OR", VxVy, "Set Vx = Vx OR Vy."),
    OP(0x8|x|y|2, "AND", VxVy, "Set Vx = Vx AND Vy."),
    OP(0x8|x|y|3, "XOR", VxVy, "Set Vx = Vx XOR Vy."),
    OP(0x8|x|y|4, "ADD", VxVy, "Set Vx = Vx + Vy, set VF = carry."),
    OP(0x8|x|y|5, "SUB", VxVy, "Set Vx = Vx - Vy, set VF = NOT borrow."),
    OP(0x8|x|y|6, "SHR", VxVy, "Set Vx = Vx SHR 1."),
    OP(0x8|x|y|7, "SUBN", VxVy, "Set Vx = Vy - Vx, set VF = NOT borrow."),
    OP(0x8|x|y|E, "SHL", VxVy, "Set Vx = Vx SHL 1."),
    OP(0x9|x|y|0, "SNE", VxVy, "Skip next instruction if Vx != Vy."),
    OP(0xA|nnn, "LD", IAddr, "Set I = nnn."),
    OP(0xB|nnn, "JP", V0Addr, "Jump to location nnn + V0."),
    OP(0xC|x|kk, "RND", VxByte, "Set Vx = random byte AND kk."),
    OP(0xD|x|y|n, "DRW", VxVyNib, "Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision."),
    OP(0xE|x|9|E, "SKP", Vx, "Skip next instruction if key with the value of Vx is pressed."),
    OP(0xE|x|A|1, "SKNP", Vx, "Skip next instruction if key with the value of Vx is not pressed."),
    OP(0xF|x|0|7, "LD", VxDT, "Set Vx = delay timer value."),
    OP(0xF|x|0|A, "LD", VxK, "Wait for a key press, store the value of the key in Vx."),
    OP(0xF|x|1|5, "LD", DTVx, "Set delay timer = Vx."),
    OP(0xF|x|1|8, "LD", STVx, "Set sound timer = Vx."),
    OP(0xF|x|1|E, "ADD", IVx, "Set I = I + Vx."),
    OP(0XF|x|2|9, "LD", FVx, "Set I = location of sprite for digit Vx."),
    OP(0xF|x|3|3, "LD", BVx, "Store BCD representation of Vx in memory locations I, I+1, and I+2."),
    OP(0xF|x|5|5, "LD", IVx, "Store registers V0 through Vx in memory starting at location I."),
    OP(0xF|x|6|5, "LD", VxI, "Read registers V0 through Vx from memory starting at location I."),

    OP(0xFFFF, "Unknown command", None, "Unknown command")
END_OPS

#undef nnn
#undef kk
#undef x
#undef y
#undef E
#undef A

#define n1(val) (val & 0xF000) >> 12
#define n2(val) (val & 0x0F00) >> 8
#define n3(val) (val & 0x00F0) >> 4
#define n4(val) (val & 0x000F)

#define op1(n1) op(n1, 0, 0, 0)
#define op2(n1, n2) op(n1, n2, 0, 0)
#define op3(n1, n2, n3) op(n1, n2, n3, 0)
#define op(n1, n2, n3, n4) (n1 << 12 | n2 << 8 | n3 << 4 | n4)

const OpCode& getOpCode(const unsigned short rawCode) {
    unsigned char n1 = n1(rawCode);
    unsigned char n2 = n2(rawCode);
    unsigned char n3 = n3(rawCode);
    unsigned char n4 = n4(rawCode);

    switch (n1) {
        case 0x0:
            return ops[rawCode];
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            return ops[op1(n1)];
        case 0x8:
            return ops[op(n1, 0, 0, n4)];
        case 0xE:
            return ops[op(n1, 0, n3, n4)];
    }
    
    return ops[0xFFFF];
}

#endif /* OpCodes_h */
