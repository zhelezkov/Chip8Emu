//
//  Disassembler.hpp
//  Chip8Asm
//

#ifndef Disassembler_hpp
#define Disassembler_hpp

#include "Opcodes.h"
#include <fstream>

class Disassembler
{
    const char* in;
    const char* out;
    const char* log = "Disassembler.log";

public:
    Disassembler(const char *in, const char* out);
    bool make();
};

#endif /* Disassembler_hpp */
