//
//  Opcodes.h
//  Chip8
//

#ifndef Opcodes_h
#define Opcodes_h

#include <map>
#include "Opcode.hpp"

#define op(n1, n2, n3, n4) (n1 << 12 | n2 << 8 | n3 << 4 | n4)

#define BEGIN_OPS static std::map<ushort, Opcode> ops = {
#define OP(addr, name, arg, descr, exec) {addr, Opcode(name, arg, descr, exec) }
#define END_OPS };

#define fn(name) fn_##name

/*#ifdef CHIP_EMU

#elif CHIP_DISASM

#elif CHIP_ASM

#endif*/

#define args(...) {__VA_ARGS__}

#define INIT_OPS \
BEGIN_OPS \
    OP(op(0,n,n,n), "SYS", args(), "[Deprecated] Jump to a machine code routine at nnn.", fn(nop)), \
    OP(op(0,0,E,0), "CLS", args(), "Clear the display.", fn(0x00E0)), \
    OP(op(0,0,E,E), "RET", args(), "Return from a subroutine.", fn(0x00EE)), \
	OP(op(1,n,n,n), "JP", args(number), "Jump to addr nnn.", fn(0x1nnn)), \
    OP(op(2,n,n,n), "CALL", args(number), "Call subroutine at nnn.", fn(0x2nnn)), \
    OP(op(3,x,k,k), "SE", args(reg, number), "Skip next instruction if Vx = kk.", fn(0x3xkk)), \
    OP(op(4,x,k,k), "SNE", args(reg, number), "Skip next instruction if Vx != kk.", fn(0x4xkk)), \
    OP(op(5,x,y,0), "SE", args(reg, reg), "Skip next instruction if Vx = Vy.", fn(0x5xy0)), \
    OP(op(6,x,k,k), "LD", args(reg, number), "Set Vx = kk.", fn(0x6xkk)), \
    OP(op(7,x,k,k), "ADD", args(reg, number), "Set Vx = Vx + kk.", fn(0x7xkk)), \
    OP(op(8,x,y,0), "LD", args(reg, reg), "Set Vx = Vy.", fn(0x8xy0)), \
    OP(op(8,x,y,1), "OR", args(reg, reg), "Set Vx = Vx OR Vy.", fn(0x8xy1)), \
    OP(op(8,x,y,2), "AND", args(reg, reg), "Set Vx = Vx AND Vy.", fn(0x8xy2)), \
    OP(op(8,x,y,3), "XOR", args(reg, reg), "Set Vx = Vx XOR Vy.", fn(0x8xy3)), \
    OP(op(8,x,y,4), "ADD", args(reg, reg), "Set Vx = Vx + Vy, VF = 1 if overflow else 0.", fn(0x8xy4)), \
    OP(op(8,x,y,5), "SUB", args(reg, reg), "Set Vx = Vx - Vy, set VF = 1 if not borrow else 0.", fn(0x8xy5)), \
    OP(op(8,x,y,6), "SHR", args(reg), "Set Vx = Vx SHR 1.", fn(0x8xy6)), \
    OP(op(8,x,y,7), "SUBN", args(reg, reg), "Set Vx = Vy - Vx, set VF = 1 if not borrow else 0.", fn(0x8xy7)), \
    OP(op(8,x,y,E), "SHL", args(reg), "Set Vx = Vx SHL 1.", fn(0x8xyE)), \
    OP(op(9,x,y,0), "SNE", args(reg, reg), "Skip next instruction if Vx != Vy.", fn(0x9xy0)), \
    OP(op(A,n,n,n), "LD", args(i, number), "Set I = nnn.", fn(0xAnnn)), \
    OP(op(B,n,n,n), "JP", args(v0, number), "Jump to location nnn + V0.", fn(0xBnnn)), \
    OP(op(C,x,k,k), "RND", args(reg, number), "Set Vx = random byte AND kk.", fn(0xCxkk)), \
    OP(op(D,x,y,n), "DRW", args(reg, reg, number), "Display n-byte sprite starting at memory location I at (Vx, Vy)), set VF = collision.", fn(0xDxyn)), \
    OP(op(E,x,9,E), "SKP", args(reg), "Skip next instruction if key with the value of Vx is pressed.", fn(0xEx9E)), \
    OP(op(E,x,A,1), "SKNP", args(reg), "Skip next instruction if key with the value of Vx is not pressed.", fn(0xExA1)), \
    OP(op(F,x,0,7), "LD", args(reg, dt), "Set Vx = delay timer value.", fn(0xFx07)), \
    OP(op(F,x,0,A), "LD", args(reg), "Wait for a key press, store the value of the key in Vx.", fn(0xFx0A)), \
    OP(op(F,x,1,5), "LD", args(dt, reg), "Set delay timer = Vx.", fn(0xFx15)), \
    OP(op(F,x,1,8), "LD", args(st, reg), "Set sound timer = Vx.", fn(0xFx18)), \
    OP(op(F,x,1,E), "ADD", args(i, reg), "Set I = I + Vx.", fn(0xFx1E)), \
    OP(op(F,x,2,9), "LD", args(f, reg), "Set I = location of sprite for digit Vx.", fn(0xFx29)), \
    OP(op(F,x,3,3), "LD", args(b, reg), "Store BCD representation of Vx in memory locations I, I+1, and I+2.", fn(0xFx33)), \
    OP(op(F,x,5,5), "LD", args(addr, reg), "Store registers V0 through Vx in memory starting at location I.", fn(0xFx55)), \
    OP(op(F,x,6,5), "LD", args(reg, addr), "Read registers V0 through Vx from memory starting at location I.", fn(0xFx65)), \
    OP(op(F,F,F,F), "NOP", args(), "Unknown command", fn(nop)), \
    /************ Super CHIP-8 functions ************/ \
    OP(op(0,0,B,n), "SCU", args(number), "Scroll display N lines up.(N/2 in low res)", fn(0x00Bn)), \
    OP(op(0,0,C,n), "SCD", args(number), "Scroll display N lines down.(N/2 in low res)", fn(0x00Cn)), \
    OP(op(0,0,F,B), "SCR", args(), "Scroll display 4 pixels right.(2 pix in low res)", fn(0x00FB)), \
    OP(op(0,0,F,C), "SCL", args(), "Scroll display 4 pixels left.(2 pix in low res)", fn(0x00FC)), \
    OP(op(0,0,F,D), "EXIT", args(), "Quit the emulator.", fn(0x00FD)), \
    OP(op(0,0,F,E), "LOW", args(), "Enter low res screen mode", fn(0x00FE)), \
    OP(op(0,0,F,F), "HIGH", args(), "Enter high res screen mode", fn(0x00FF)), \
    OP(op(F,x,3,0), "LD", args(hf, reg), "Point I to 10 byte numeric sprite for value in VX.", fn(0xFx30)), \
    OP(op(F,x,7,5), "LD", args(r, reg), "Save V0...VX (X<8) in the RPL flags.", fn(0xFx75)), \
    OP(op(F,x,8,5), "LD", args(reg, r), "Load V0...VX (X<8) from the RPL flags.", fn(0xFx85)) \
	/************ Extended CHIP-8 functions ************/ \
END_OPS

extern const Opcode& getOpcode(const OpcodeData opcode);
extern std::map<ushort, Opcode>::iterator getBeginOps();
extern std::map<ushort, Opcode>::iterator getEndOps();
#endif /* OpCodes_h */
