//
//  Opcode.hpp
//  Chip8
//

#ifndef Opcode_hpp
#define Opcode_hpp

#include <fstream>
#include "CHIP.h"

enum OpMask {
    None = 0,
    Addr = 1, // address
    VxByte = 2, //Vx, byte
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
	N = 1	 // N
};

struct OpcodeData {
    OpcodeData(ushort val) {
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

#ifdef CHIP_EMU
#define EXEC_DESCR CPU* const, const OpcodeData
#elif CHIP_ASM
#define EXEC_DESCR
#elif CHIP_DISASM
#define EXEC_DESCR std::ofstream&, const OpcodeData
#endif

class CPU;

class Opcode final {
public:
    Opcode() : Opcode("", None, "") {
        
    }
    
    Opcode(const char* name, const OpMask mask, const char* description, void (*exec)(EXEC_DESCR) = nullptr);
    
    void (*exec)(EXEC_DESCR);
    const char* getName() const { return name; };
private:
    const char* name;
    const OpMask mask;
    const char* description;
};

#endif /* Opcode_hpp */
