//
//  DisAssembler.cpp
//  Chip8Asm
//

#include "DisAssembler.hpp"

DisAssembler::DisAssembler(const char *in, const char* out) : in(in), out(out)
{

}

bool DisAssembler::make()
{
    std::ifstream inFILE(in, std::ios::binary | std::ios::in);
    std::ofstream outFILE(out, std::ios::out);
    std::ofstream logFILE(this->log, std::ios::out);

    if (inFILE.is_open())
    {
        logFILE << "OK: file is opened." << std::endl;

        // Check file size
        inFILE.seekg(0, inFILE.end);
        int size = inFILE.tellg();
        logFILE << "File size: " << size << std::endl;
        if (size > 0x0FFF - 0x200)
        {
            logFILE << "Error: file '" << in << "' is too large." << std::endl;
            inFILE.close();
            outFILE.close();
            logFILE.close();

            return false;
        }
        inFILE.seekg(0, inFILE.beg);

        // Parsing
        ushort ch;
        for (int i = 0; i < size; i++)
        {
            inFILE >> ch;
            const Opcode op = getOpcode(ch);
            /*if (op.exec != nullptr)
            {

            }
            else
            {
                inFILE.close();
                outFILE.close();
                logFILE.close();

                return false;
            }*/
        }

        logFILE << "Build successful." << std::endl;
        inFILE.close();
        outFILE.close();
        logFILE.close();

        return true;
    }
    else
    {
        logFILE << "Error: unable to open file '" << in << "'" << std::endl;

        inFILE.close();
        outFILE.close();
        logFILE.close();
        return false;
    }
}

int main()
{
    return 0;
}