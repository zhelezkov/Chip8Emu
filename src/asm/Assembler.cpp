//
//  Assembler.cpp
//  Chip8Asm
//

#include "Assembler.hpp"


Assembler::Assembler(const char * in, const char * out) : in(in), out(out)
{

}

bool Assembler::Assemble()
{
    std::ifstream inFile(in, std::ios::binary | std::ios::in);
    std::ofstream outFile(out, std::ios::out);
    std::ofstream logFile(this->log, std::ios::out);

    // check file for opened
    if (!inFile.is_open())
    {
        logFile << "Error: unable to open file '" << in << "'" << std::endl;
        return false;
    }
    logFile << "OK: file is opened." << std::endl;

    std::string s;
    while (getline(inFile, s))
    {

    }

    return true;
}

int main() {
    Assembler Asm("in.asm", "out.txt");
    Asm.Assemble();
    return 0;
}
