//
//  Assembler.hpp
//  Chip8Asm
//

#ifndef Assembler_hpp
#define Assembler_hpp

#include <fstream>

class Assembler
{
    const char* in;
    const char* out;
    const char* log = "Assembler.log";

public:
    Assembler(const char *in, const char* out);
    bool Assemble();
};
#endif /* Assembler_hpp */
