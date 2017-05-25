//
//  Disassembler.cpp
//  Chip8Asm
//

#define LOGURU_IMPLEMENTATION 1
#include "Disassembler.hpp"

void Disassembler::disassembleFile(const char* in, const char* out) {
    std::ifstream inFile(in, std::ios::binary | std::ios::in);
    std::ofstream outFile(out, std::ios::out);

    // set output flags
    outFile.setf(std::ios::hex, std::ios::basefield);
    outFile.setf(std::ios::uppercase);
    
    CHECK_F(inFile.is_open(), "Error: unable to open file '%s'", in);

    LOG_F(INFO, "OK: file is opened.");
    // Check file size
    inFile.seekg(0, inFile.end);
    int size = inFile.tellg();
    LOG_F(INFO, "File size: %d bytes.", size);
    
    CHECK_F(size <= (0xFFF - 0x200), "Error: file '%s' is too large.", in);
    
    inFile.seekg(0, inFile.beg);

    // Parsing
    ushort ch;
    byte n1, n2;
    for (int i = 0; i < size; i+=2)
    {
        n1 = inFile.get();
        n2 = inFile.get();

        ch = (n1 << 8) + n2;
        const Opcode op = getOpcode(ch);
        op.exec(outFile, OpcodeData(ch));
    }

    LOG_F(INFO, "Build successful.");
}

std::string Disassembler::disasmBytecode(ushort bytecode) {
    std::stringstream str;
    str.setf(std::ios::hex, std::ios::basefield);
    str.setf(std::ios::uppercase);
    
    const Opcode op = getOpcode(bytecode);
    op.exec(str, OpcodeData(bytecode));
    return str.str();
}

#ifdef CHIP_DISASM_STANDALONE
int main(int argc, char* argv[]) {
    CHECK_F(argc >= 2, "Not enough arguments.");
    int inLen = strlen(argv[1]);
    char* outFile = (char*) malloc((inLen + 5) * sizeof(char));
    strcpy(outFile, argv[1]);
    strcat(outFile, ".out");
    Disassembler::disassembleFile(argv[1], outFile);
    
    free(outFile);
    return 0;
}
#endif
