//
//  Assembler.cpp
//  Chip8Asm
//

#include "Assembler.hpp"
#include <regex>

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
    std::cmatch result;
    std::regex regular("[\\s|,]*(;+.*)?[\\s|,]*([\\w-\\[\\]:#%\\.]+)?[\\s|,]*([^\\w-\\[\\]:#%\\. ,\\s;]+)?");
    while (getline(inFile, s))
    {
        while (std::regex_search(s.c_str(), result, regular) && s != "")
        {
            for (int i = 1; i < result.size(); i++)
                if (result[i] != "") outFile << result[i] << ' ';

            s = result.suffix().str();
        }

        outFile << "\n";
    }

    return true;
}

int main() {
    Assembler Asm("in.c8", "out.txt");
    Asm.Assemble();
    return 0;
}
