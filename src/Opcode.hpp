//
//  Opcode.hpp
//  Chip8
//

#ifndef Opcode_hpp
#define Opcode_hpp

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

class CPU;
struct OpcodeData;

class Opcode final {
public:
    Opcode() : Opcode("", None, "") {
        
    }
    
    Opcode(const char* name, const OpMask mask, const char* description, void (*exec)(CPU* const, const OpcodeData) = nullptr, void (*write)() = nullptr);
    
    void (*exec)(CPU* const, const OpcodeData);
    void (*write)();
private:
    const char* name;
    const OpMask mask;
    const char* description;
};

#endif /* Opcode_hpp */
