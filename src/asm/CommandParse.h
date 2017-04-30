//
//  CommandParse.h
//  Chip8Asm
//

#ifndef CommandParse_h
#define CommandParse_h

#include "Opcodes.h"

#include "CHIP.h"
#include "StringToken.hpp"
#include "HelpFunctions.h"
#include <vector>

extern std::vector <std::pair<int, bool>> file;		  // in Assembler.cpp
extern std::vector <std::string> errors;			  // in Assembler.cpp
extern std::map<std::string, ushort> labels;		  // in Assembler.cpp
extern std::map<std::string, byte> var;				  // in Assembler.cpp					
extern std::map<std::string, byte> equ;				  // in Assembler.cpp	
extern bool ERROR;									  // in Assembler.cpp
extern std::vector<std::vector<StringToken>> ovector; // in Assembler.cpp

bool CheckArg(typeToken type, typeArg _correctType)
{
	if (_correctType == name)
	{
		if (type == UNK) return true;
		else return false;
	}

	if (_correctType == number)
	{
		if (type == BIN || type == HEX || type == DEC) return true;
		else return false;
	}

	if (_correctType == reg)
	{
		if (type == REG) return true;
		else return false;
	}

	if (_correctType == addr)
	{
		if (type == ADDR) return true;
		else return false;
	}

	if (_correctType == dt)
	{
		if (type == DT) return true;
		else return false;
	}

	if (_correctType == st)
	{
		if (type == ST) return true;
		else return false;
	}

	return false;
}

typeToken typeArgTotypeToken(typeArg type)
{

}

bool Check(int strNum, const std::pair<int, int>& CommandToken, const std::vector<typeArg>& arguments)
{
	/**************************************** check count of arguments *****************************/
	if (CommandToken.second - CommandToken.first != arguments.size())
	{
		errors.push_back(StringNumber(strNum) + " Error count of arguments.");
		return false;
	}

	/******************************************** Redefinition *************************************/
	for(int i = 0; i < arguments.size(); i++)
		if(arguments[i] != reg)
			if (Redefinition(ovector[strNum][CommandToken.first + 1 + i].commandStr))
			{
				errors.push_back(StringNumber(strNum) + " Redefinition: " + ovector[strNum][CommandToken.first + 1].commandStr);
				return false;
			}

	/********************************* Correct type of arguments **********************************/
	for(int i = 0; i < arguments.size(); i++)
		if (!CheckArg(ovector[strNum][CommandToken.first + 1 + i].type, arguments[i]))
		{
			errors.push_back(StringNumber(strNum) + ' ' + std::to_string(i + 1) + "th argument should be the " + typeArgStr[arguments[i]] + ": " + ovector[strNum][CommandToken.first + 1].commandStr);
			return false;
		}
}

bool CheckCommand(int strNum, const std::pair<int, int>& CommandToken, int& curMem)
{
	std::vector<StringToken>& str = ovector[strNum];
	std::string cmd = ovector[strNum][CommandToken.first].commandStr;
	std::vector<typeArg> arguments;
	
	for (auto it = getBeginOps(); it != getEndOps(); it++)
	{
		arguments = (*it).second.getArgs();
		if (Check(strNum, CommandToken, arguments))
		{
			str[CommandToken.first].type = CMD;
			for (int i = 0; i < arguments.size(); i++)
			{
				str[CommandToken.first + 1 + i].type = typeArgTotypeToken(arguments[i]);
			}

			file[strNum].first = curMem;
			file[strNum].second = false;

			if (cmd == "equ")
			{
				curMem -= 2;
				file[strNum].first = -1;
				equ[str[CommandToken.first + 1].commandStr] = strToNumber(str[CommandToken.first + 2].commandStr);
			}

			if (cmd == "var")
			{
				curMem -= 2;
				file[strNum].first = -1;
				var[str[CommandToken.first + 1].commandStr] = strToNumber(str[CommandToken.first + 2].commandStr);
			}

			curMem += 2;
			return true;
		}
	}

	std::string errorStr = "";
	errorStr += StringNumber(strNum) + " Unknown command \"" + cmd + "\" with arguments: ";
	for (int i = CommandToken.first + 1; i <= CommandToken.second; i++)
		errorStr += str[i].commandStr + ' ';

	errors.push_back(errorStr);
	file[strNum].first = curMem;
	file[strNum].second = true;
	ERROR = true;
	curMem += 2;

	return false;
}

void ParseCommand()
{
	
}

#endif CommandParse_h /* CommandParse.h */