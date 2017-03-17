//
//  Opcodes.h
//  Chip8
//

#ifndef Opcodes_h
#define Opcodes_h

#include <map>
#include "Opcode.hpp"

struct OpcodeData {
    OpcodeData(int val) {
        rawCode = val;
    }
    
    union {
        struct {
            ushort n4 : 4;
            ushort n3 : 4;
            ushort n2 : 4;
            ushort n1 : 4;
        };
        ushort rawCode;
    };
};

#define BEGIN_OPS static std::map<unsigned char, Opcode> ops = {
#define OP(addr, name, mask, descr, exec) {addr, Opcode(name, mask, descr, exec) }
#define END_OPS };

#define INIT_OPS \
BEGIN_OPS \
    OP(0x0|nnn, "SYS", None, "[Deprecated] Jump to a machine code routine at nnn.", fn_nop), \
    OP(0x00E0, "CLS", None, "Clear the display.", fn_0x00E0), \
    OP(0x00EE, "RET", None, "Return from a subroutine.", fn_0x00EE), \
	OP(0x00B|n, "SCU", N, "Scroll display N lines up.", fn_0x00Bn), \
	OP(0x00C|n, "SCD", N, "Scroll display N lines down.", fn_0x00Cn), \
	OP(0x00FB, "SCR", None, "Scroll display 4 pixels right.", fn_0x00FB), \
	OP(0x00FC, "SCL", None, "Scroll display 4 pixels left.", fn_0x00FC), \
	OP(0x00FD, "EXIT", None, "Quit the emulator.", fn_0x00FD), \
	OP(0x00FE, "LOW", None, "Enable extended screen mode *CHIP48*.", fn_0x00FE), \
	OP(0x00FF, "HIGH", None, "Enable extended screen mode *SUPER CHIP*.", fn_0x00FF), \
    OP(0x1|nnn, "JP", Addr, "Jump to location nnn.", fn_0x1nnn), \
    OP(0x2|nnn, "CALL", Addr, "Call subroutine at nnn.", fn_0x2nnn), \
    OP(0x3|x|kk, "SE", VxByte, "Skip next instruction if Vx = kk.", fn_0x3xkk), \
    OP(0x4|x|kk, "SNE", VxByte, "Skip next instruction if Vx != kk.", fn_0x4xkk), \
    OP(0x5|x|y|0, "SE", VxVy, "Skip next instruction if Vx = Vy.", fn_0x5xy0), \
    OP(0x6|x|kk, "LD", VxByte, "Set Vx = kk.", fn_0x6xkk), \
    OP(0x7|x|kk, "ADD", VxByte, "Set Vx = Vx + kk.", fn_0x7xkk), \
    OP(0x8|x|y|0, "LD", VxVy, "Set Vx = Vy.", fn_0x8xy0), \
    OP(0x8|x|y|1, "OR", VxVy, "Set Vx = Vx OR Vy.", fn_0x8xy1), \
    OP(0x8|x|y|2, "AND", VxVy, "Set Vx = Vx AND Vy.", fn_0x8xy2), \
    OP(0x8|x|y|3, "XOR", VxVy, "Set Vx = Vx XOR Vy.", fn_0x8xy3), \
    OP(0x8|x|y|4, "ADD", VxVy, "Set Vx = Vx + Vy, set VF = carry.", fn_0x8xy4), \
    OP(0x8|x|y|5, "SUB", VxVy, "Set Vx = Vx - Vy, set VF = NOT borrow.", fn_0x8xy5), \
    OP(0x8|x|y|6, "SHR", Vx, "Set Vx = Vx SHR 1.", fn_0x8xy6), \
    OP(0x8|x|y|7, "SUBN", VxVy, "Set Vx = Vy - Vx, set VF = NOT borrow.", fn_0x8xy7), \
    OP(0x8|x|y|E, "SHL", Vx, "Set Vx = Vx SHL 1.", fn_0x8xyE), \
    OP(0x9|x|y|0, "SNE", VxVy, "Skip next instruction if Vx != Vy.", fn_0x9xy0), \
    OP(0xA|nnn, "LD", IAddr, "Set I = nnn.", fn_0xAnnn), \
    OP(0xB|nnn, "JP", V0Addr, "Jump to location nnn + V0.", fn_0xBnnn), \
    OP(0xC|x|kk, "RND", VxByte, "Set Vx = random byte AND kk.", fn_0xCxkk), \
    OP(0xD|x|y|n, "DRW", VxVyNib, "Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.", fn_0xDxyn), \
    OP(0xE|x|9|E, "SKP", Vx, "Skip next instruction if key with the value of Vx is pressed.", fn_0xEx9E), \
    OP(0xE|x|A|1, "SKNP", Vx, "Skip next instruction if key with the value of Vx is not pressed.", fn_0xExA1), \
    OP(0xF|x|0|7, "LD", VxDT, "Set Vx = delay timer value.", fn_0xFx07), \
    OP(0xF|x|0|A, "LD", VxK, "Wait for a key press, store the value of the key in Vx.", fn_0xFx0A), \
    OP(0xF|x|1|5, "LD", DTVx, "Set delay timer = Vx.", fn_0xFx15), \
    OP(0xF|x|1|8, "LD", STVx, "Set sound timer = Vx.", fn_0xFx18), \
    OP(0xF|x|1|E, "ADD", IVx, "Set I = I + Vx.", fn_0xFx1E), \
    OP(0xF|x|2|9, "LD", FVx, "Set I = location of sprite for digit Vx.", fn_0xFx29), \
    OP(0xF|x|3|3, "LD", BVx, "Store BCD representation of Vx in memory locations I, I+1, and I+2.", fn_0xFx33), \
    OP(0xF|x|5|5, "LD", IVx, "Store registers V0 through Vx in memory starting at location I.", fn_0xFx55), \
    OP(0xF|x|6|5, "LD", VxI, "Read registers V0 through Vx from memory starting at location I.", fn_0xFx65), \
	OP(0xF|x|3|0, "LD", Vx, "Point I to 10 byte numeric sprite for value in VX.", fn_0xFx30), \
	OP(0xF|x|7|5, "LD", Vx, "Save V0...VX (X<8) in the RPL flags.", fn_0xFx75), \
	OP(0xF|x|8|5, "LD", Vx, "Load V0...VX (X<8) from the HP48 flags.", fn_0xFx85), \
    OP(0xFFFF, "Unknown command", None, "Unknown command", fn_nop) \
END_OPS

#define op(n1, n2, n3, n4) (n1 << 12 | n2 << 8 | n3 << 4 | n4)

extern const Opcode& getOpcode(const OpcodeData opcode);

#endif /* OpCodes_h */
