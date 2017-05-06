//
//  Disassembler.hpp
//  Chip8Asm
//

#ifndef Disassembler_hpp
#define Disassembler_hpp

#include "Opcodes.h"
#include "CHIP.h"
#include <fstream>
#include <string>
#include <sstream>

class Disassembler {
public:
    static void disassembleFile(const char* in, const char* out);
    static std::string disasmBytecode(ushort bytecode);
};

#endif /* Disassembler_hpp */
