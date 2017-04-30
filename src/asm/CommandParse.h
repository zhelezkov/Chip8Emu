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
#include <string>
#include <algorithm>

extern std::vector <std::pair<int, bool>> file;		  // in Assembler.cpp
extern std::vector <std::string> errors;			  // in Assembler.cpp
extern std::map<std::string, ushort> labels;		  // in Assembler.cpp
extern std::map<std::string, byte> var;				  // in Assembler.cpp					
extern std::map<std::string, byte> equ;				  // in Assembler.cpp	
extern bool ERROR;									  // in Assembler.cpp
extern std::vector<std::vector<StringToken>> ovector; // in Assembler.cpp

bool CheckArg(StringToken str, typeArg _correctType)
{
	typeToken type = str.type;
	if (_correctType == name)
	{
		if (type == NAME) return true;
		else return false;
	}

	if (_correctType == number)
	{
		// + equ + label
		if (type == BIN || type == HEX || type == DEC || equ.count(str.commandStr) || labels.count(str.commandStr)) return true;
		else return false;
	}

	if (_correctType == reg)
	{
		// + var
		if (type == REG || var.count(str.commandStr)) return true;
		else return false;
	}

	if (_correctType == addr)
	{
		if (type == ADDR) return true;
		else return false;
	}

	if (_correctType == addr)
	{
		if (type == ADDR) return true;
		else return false;
	}

	if (_correctType == i)
	{
		if (type == I) return true;
		else return false;
	}

	if (_correctType == b)
	{
		if (type == B) return true;
		else return false;
	}

	if (_correctType == v0)
	{
		if (str.commandStr == "v0" || str.commandStr == "V0") return true;
		else return false;
	}

	if (_correctType == f)
	{
		if (type == F) return true;
		else return false;
	}

	if (_correctType == hf)
	{
		if (type == HF) return true;
		else return false;
	}

	if (_correctType == r)
	{
		if (type == R) return true;
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

bool Check(int strNum, const std::pair<int, int>& CommandToken, const std::vector<typeArg>& arguments, bool debug)
{
	std::string cmd = ovector[strNum][CommandToken.first].commandStr;
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);

	/**************************************** check count of arguments *****************************/
	if (CommandToken.second - CommandToken.first != arguments.size())
	{
		if(debug) errors.push_back(StringNumber(strNum) + " Error count of arguments.");
		return false;
	}

	/******************************************** Redefinition *************************************/
	// ONLY FOR VAR, EQU
	if(cmd == "EQU" || cmd == "VAR")
		if (Redefinition(ovector[strNum][CommandToken.first + 1].commandStr))
		{
			errors.push_back(StringNumber(strNum) + " Redefinition: " + ovector[strNum][CommandToken.first + 1].commandStr);
			return false;
		}

	/********************************* Correct type of arguments **********************************/
	for(int i = 0; i < arguments.size(); i++)
		if (!CheckArg(ovector[strNum][CommandToken.first + 1 + i], arguments[i]))
		{
			if (debug) errors.push_back(StringNumber(strNum) + ' ' + std::to_string(i + 1) + "th argument should be the " + typeArgStr[arguments[i]] + ": " + ovector[strNum][CommandToken.first + 1].commandStr);
			return false;
		}

	return true;
}

bool CheckCommand(int strNum, const std::pair<int, int>& CommandToken, int& curMem)
{
	std::vector<StringToken>& str = ovector[strNum];
	std::string cmd = ovector[strNum][CommandToken.first].commandStr;
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
	std::vector<typeArg> arguments;
	
	if (cmd == "EQU")
	{
		str[CommandToken.first].type = CMD;
		arguments = { name, number };
		file[strNum].first = -1;

		if (Check(strNum, CommandToken, arguments, true))
		{
			equ[str[CommandToken.first + 1].commandStr] = strToNumber(str[CommandToken.first + 2].commandStr);
			file[strNum].second = false;
			return true;
		}
		else
		{
			file[strNum].second = true;
			ERROR = true;
			return false;
		}
	}

	if (cmd == "VAR")
	{
		str[CommandToken.first].type = CMD;
		arguments = { name, reg };
		file[strNum].first = -1;
		
		if (Check(strNum, CommandToken, arguments, true))
		{
			var[str[CommandToken.first + 1].commandStr] = strToNumber(str[CommandToken.first + 2].commandStr);
			file[strNum].second = false;
			return true;
		}
		else
		{
			file[strNum].second = true;
			ERROR = true;
			return false;
		}
	}

	if (cmd == "BYTE")
	{
		str[CommandToken.first].type = CMD;
		arguments = { number };
		file[strNum].first = curMem;
		curMem += 2;

		if (Check(strNum, CommandToken, arguments, true))
		{
			file[strNum].second = false;
			return true;
		}
		else
		{
			file[strNum].second = true;
			ERROR = true;
			return false;
		}
	}

	for (auto it = getBeginOps(); it != getEndOps(); it++)
	{
		arguments = (*it).second.getArgs();
		if (cmd == (*it).second.getName())
		{
			bool debug = true;
			if (cmd != "LD" || cmd != "SE" || cmd != "SNE" || cmd != "JP" || cmd != "ADD") debug = false;

			str[CommandToken.first].type = CMD;
			file[strNum].first = curMem;
			curMem += 2;

			if (Check(strNum, CommandToken, arguments, debug))
			{
				file[strNum].second = false;
				return true;
			}
			else
			{
				file[strNum].second = true;
				ERROR = true;
				if (debug) return false;
			}
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