//
//  Disassembler.cpp
//  Chip8Asm
//

#include "Disassembler.hpp"

Disassembler::Disassembler(const char *in, const char* out) : in(in), out(out)
{

}

bool Disassembler::disassemble()
{
    std::ifstream inFile(in, std::ios::binary | std::ios::in);
    std::ofstream outFile(out, std::ios::out);
    std::ofstream logFile(this->log, std::ios::out);

    // set output flags
    outFile.setf(std::ios::hex, std::ios::basefield);
    outFile.setf(std::ios::uppercase);

    // check file for opened
    if (!inFile.is_open())
    {
        logFile << "Error: unable to open file '" << in << "'" << std::endl;
        return false;
    }

    logFile << "OK: file is opened." << std::endl;
    // Check file size
    inFile.seekg(0, inFile.end);
    int size = inFile.tellg();
    logFile << "File size: " << size << "bytes." << std::endl;
    if (size > 0x0FFF - 0x200)
    {
        logFile << "Error: file '" << in << "' is too large." << std::endl;
        return false;
    }
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
        //if(op.exec) op.exec(outFile, OpcodeData(ch));
        op.exec(outFile, OpcodeData(ch));
    }

    logFile << "Build successful." << std::endl;

    return true;
}

int main()
{
    Disassembler disAsm("BLINKY", "out.txt");
    disAsm.disassemble();
    return 0;
}
