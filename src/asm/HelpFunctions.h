//
//  HelpFunctions.h
//  Chip8Asm
//

#ifndef HelpFunctions_h
#define HelpFunctions_h

#include <tuple>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include "CHIP.h"
#include "typeToken.h"

extern std::map<std::string, int> labels;	 // in Assembler.cpp
extern std::map<std::string, byte> var;		 // in Assembler.cpp	
extern std::map<std::string, byte> equ;		 // in Assembler.cpp 

std::string StringNumber(int i);

static const std::set<char> errorSymbols = { '-', ':', '#', '%', '.', '[', ']' };
bool checkForValidStr(const std::string s, int start = -1, int end = -1);

bool checkStrForHexNumber(const std::string&, int start = -1, int end = -1);

bool checkStrForDecNumber(const std::string& s, int start = -1, int end = -1);

bool checkStrForBinNumber(const std::string& s, int start = -1, int end = -1);

bool checkStrForRegister(const std::string& s);

bool checkStrForCommand(std::string s);

bool checkStrForDT(std::string s);

bool checkStrForST(std::string s);

bool checkStrForHF(std::string s);

bool checkTypeForArg(typeToken type);

ushort strToNumber(std::string str);

bool Redefinition(const std::string& str);

#endif /* HelpFunctions.h */