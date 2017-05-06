//
//  Disassembler.hpp
//  Chip8Asm
//

#ifndef Disassembler_hpp
#define Disassembler_hpp

#if defined (_WIN32) 
	#if defined (chip8disasmLib_EXPORTS)
		#define EXPORT __declspec(dllexport)
	#else
		#define EXPORT __declspec(dllimport)
	#endif 
#else
	#define EXPORT
#endif

#include "Opcodes.h"
#include "CHIP.h"
#include <fstream>
#include <string>
#include <sstream>

class Disassembler {
public:
    static void disassembleFile(const char* in, const char* out);
	EXPORT static std::string disasmBytecode(ushort bytecode);
};

#endif /* Disassembler_hpp */
