//
//  DisAssembler.hpp
//  Chip8Asm
//

#ifndef DisAssembler_hpp
#define DisAssembler_hpp

#include "Opcodes.h"
#include <fstream>

class DisAssembler
{
    const char* in;
    const char* out;
    const char* log = "DisAssembler.log";

public:
    DisAssembler(const char *in, const char* out);
    bool make();
};

#endif /* DisAssembler_hpp */