//
//  Opcodes.h
//  Chip8
//

#ifndef Opcodes_h
#define Opcodes_h

#include <map>
#include "Opcode.hpp"

#define BEGIN_OPS static std::map<ushort, Opcode> ops = {
#define OP(addr, name, mask, descr, exec) {addr, Opcode(name, mask, descr, exec) }
#define END_OPS };

#ifdef CHIP_EMU
#define fn(name) fn_##name
#elif CHIP_DISASM
#define fn(name) fn_##name
#elif CHIP_ASM
#define fn(name) nullptr
#endif

#define INIT_OPS \
BEGIN_OPS \
    OP(0x0|nnn, "SYS", None, "[Deprecated] Jump to a machine code routine at nnn.", fn(nop)), \
    OP(0x00E0, "CLS", None, "Clear the display.", fn(0x00E0)), \
    OP(0x00EE, "RET", None, "Return from a subroutine.", fn(0x00EE)), \
	OP(0x1|nnn, "JP", Addr, "Jump to location nnn.", fn(0x1nnn)), \
    OP(0x2|nnn, "CALL", Addr, "Call subroutine at nnn.", fn(0x2nnn)), \
    OP(0x3|x|kk, "SE", VxByte, "Skip next instruction if Vx = kk.", fn(0x3xkk)), \
    OP(0x4|x|kk, "SNE", VxByte, "Skip next instruction if Vx != kk.", fn(0x4xkk)), \
    OP(0x5|x|y|0, "SE", VxVy, "Skip next instruction if Vx = Vy.", fn(0x5xy0)), \
    OP(0x6|x|kk, "LD", VxByte, "Set Vx = kk.", fn(0x6xkk)), \
    OP(0x7|x|kk, "ADD", VxByte, "Set Vx = Vx + kk.", fn(0x7xkk)), \
    OP(0x8|x|y|0, "LD", VxVy, "Set Vx = Vy.", fn(0x8xy0)), \
    OP(0x8|x|y|1, "OR", VxVy, "Set Vx = Vx OR Vy.", fn(0x8xy1)), \
    OP(0x8|x|y|2, "AND", VxVy, "Set Vx = Vx AND Vy.", fn(0x8xy2)), \
    OP(0x8|x|y|3, "XOR", VxVy, "Set Vx = Vx XOR Vy.", fn(0x8xy3)), \
    OP(0x8|x|y|4, "ADD", VxVy, "Set Vx = Vx + Vy, set VF = carry.", fn(0x8xy4)), \
    OP(0x8|x|y|5, "SUB", VxVy, "Set Vx = Vx - Vy, set VF = NOT borrow.", fn(0x8xy5)), \
    OP(0x8|x|y|6, "SHR", Vx, "Set Vx = Vx SHR 1.", fn(0x8xy6)), \
    OP(0x8|x|y|7, "SUBN", VxVy, "Set Vx = Vy - Vx, set VF = NOT borrow.", fn(0x8xy7)), \
    OP(0x8|x|y|E, "SHL", Vx, "Set Vx = Vx SHL 1.", fn(0x8xyE)), \
    OP(0x9|x|y|0, "SNE", VxVy, "Skip next instruction if Vx != Vy.", fn(0x9xy0)), \
    OP(0xA|nnn, "LD", IAddr, "Set I = nnn.", fn(0xAnnn)), \
    OP(0xB|nnn, "JP", V0Addr, "Jump to location nnn + V0.", fn(0xBnnn)), \
    OP(0xC|x|kk, "RND", VxByte, "Set Vx = random byte AND kk.", fn(0xCxkk)), \
    OP(0xD|x|y|n, "DRW", VxVyNib, "Display n-byte sprite starting at memory location I at (Vx, Vy)), set VF = collision.", fn(0xDxyn)), \
    OP(0xE|x|9|E, "SKP", Vx, "Skip next instruction if key with the value of Vx is pressed.", fn(0xEx9E)), \
    OP(0xE|x|A|1, "SKNP", Vx, "Skip next instruction if key with the value of Vx is not pressed.", fn(0xExA1)), \
    OP(0xF|x|0|7, "LD", VxDT, "Set Vx = delay timer value.", fn(0xFx07)), \
    OP(0xF|x|0|A, "LD", VxK, "Wait for a key press, store the value of the key in Vx.", fn(0xFx0A)), \
    OP(0xF|x|1|5, "LD", DTVx, "Set delay timer = Vx.", fn(0xFx15)), \
    OP(0xF|x|1|8, "LD", STVx, "Set sound timer = Vx.", fn(0xFx18)), \
    OP(0xF|x|1|E, "ADD", IVx, "Set I = I + Vx.", fn(0xFx1E)), \
    OP(0xF|x|2|9, "LD", FVx, "Set I = location of sprite for digit Vx.", fn(0xFx29)), \
    OP(0xF|x|3|3, "LD", BVx, "Store BCD representation of Vx in memory locations I, I+1, and I+2.", fn(0xFx33)), \
    OP(0xF|x|5|5, "LD", IVx, "Store registers V0 through Vx in memory starting at location I.", fn(0xFx55)), \
    OP(0xF|x|6|5, "LD", VxI, "Read registers V0 through Vx from memory starting at location I.", fn(0xFx65)), \
    OP(0xFFFF, "NOP", None, "Unknown command", fn(nop)), \
    /************ Super CHIP-8 functions ************/ \
    OP(0x00B|n, "SCU", N, "Scroll display N lines up.", fn(0x00Bn)), \
    OP(0x00C|n, "SCD", N, "Scroll display N lines down.", fn(0x00Cn)), \
    OP(0x00FB, "SCR", None, "Scroll display 4 pixels right.", fn(0x00FB)), \
    OP(0x00FC, "SCL", None, "Scroll display 4 pixels left.", fn(0x00FC)), \
    OP(0x00FD, "EXIT", None, "Quit the emulator.", fn(0x00FD)), \
    OP(0x00FE, "LOW", None, "Enable extended screen mode *CHIP48*.", fn(0x00FE)), \
    OP(0x00FF, "HIGH", None, "Enable extended screen mode *SUPER CHIP*.", fn(0x00FF)), \
    OP(0xF|x|3|0, "LD", Vx, "Point I to 10 byte numeric sprite for value in VX.", fn(0xFx30)), \
    OP(0xF|x|7|5, "LD", Vx, "Save V0...VX (X<8) in the RPL flags.", fn(0xFx75)), \
    OP(0xF|x|8|5, "LD", Vx, "Load V0...VX (X<8) from the HP48 flags.", fn(0xFx85)) \
END_OPS

#define op(n1, n2, n3, n4) (n1 << 12 | n2 << 8 | n3 << 4 | n4)

extern const Opcode& getOpcode(const OpcodeData opcode);

#endif /* OpCodes_h */
