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
extern std::map<std::string, int> labels;			  // in Assembler.cpp
extern std::map<std::string, byte> var;				  // in Assembler.cpp					
extern std::map<std::string, byte> equ;				  // in Assembler.cpp	
extern bool ERROR;									  // in Assembler.cpp
extern std::vector<VECTOR_TYPE> ovector; // in Assembler.cpp

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

bool Check(int strNum, const std::vector<typeArg>& arguments, bool debug)
{
	std::string cmd = CMD_STR(strNum, CMD_IND(strNum).first);
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);

	/**************************************** check count of arguments *****************************/
	if (CMD_IND(strNum).second - CMD_IND(strNum).first != arguments.size())
	{
		if(debug) errors.push_back(StringNumber(strNum) + " Error count of arguments.");
		return false;
	}

	/******************************************** Redefinition *************************************/
	// ONLY FOR VAR, EQU
	if(cmd == "EQU" || cmd == "VAR")
		if (Redefinition(CMD_STR(strNum, CMD_IND(strNum).first + 1)))
		{
			errors.push_back(StringNumber(strNum) + " Redefinition: " + CMD_STR(strNum, CMD_IND(strNum).first + 1));
			return false;
		}

	/********************************* Correct type of arguments **********************************/
	for(int i = 0; i < arguments.size(); i++)
		if (!CheckArg(TOKEN(strNum, CMD_IND(strNum).first + 1 + i), arguments[i]))
		{
			if (debug) errors.push_back(StringNumber(strNum) + ' ' + std::to_string(i + 1) + "th argument should be the " + typeArgStr[arguments[i]] + ": " + CMD_STR(strNum, CMD_IND(strNum).first + 1 + i));
			return false;
		}

	return true;
}

bool CheckCommand(int strNum, int& curMem)
{
	std::string cmd = CMD_STR(strNum, CMD_IND(strNum).first);
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
	std::vector<typeArg> arguments;
	
	if (cmd == "EQU")
	{
		TOKEN(strNum, CMD_IND(strNum).first).type = CMD;
		arguments = { name, number };
		file[strNum].first = -1;

		if (Check(strNum, arguments, true))
		{
			ARGS(strNum) = arguments;
			equ[CMD_STR(strNum, CMD_IND(strNum).first + 1)] = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2));
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
		TOKEN(strNum, CMD_IND(strNum).first).type = CMD;
		arguments = { name, reg };
		file[strNum].first = -1;
		
		if (Check(strNum, arguments, true))
		{
			ARGS(strNum) = arguments;
			var[CMD_STR(strNum, CMD_IND(strNum).first + 1)] = strToNumber(CMD_STR(strNum, CMD_IND(strNum).first + 2));
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
		TOKEN(strNum, CMD_IND(strNum).first).type = CMD;
		arguments = { number };
		file[strNum].first = curMem;
		curMem += 2;

		if (Check(strNum, arguments, true))
		{
			ARGS(strNum) = arguments;
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

			if (Check(strNum, arguments, debug))
			{
				TOKEN(strNum, CMD_IND(strNum).first).type = CMD;
				file[strNum].first = curMem;
				curMem += 2;

				ARGS(strNum) = arguments;
				file[strNum].second = false;
				return true;
			}
			else
			{
				if (debug)
				{
					file[strNum].second = true;
					ERROR = true;
					return false;
				}
			}
		}
	}

	std::string errorStr = "";
	errorStr += StringNumber(strNum) + " Unknown command \"" + cmd + "\" with arguments: ";
	for (int i = CMD_IND(strNum).first + 1; i <= CMD_IND(strNum).second; i++)
		errorStr += CMD_STR(strNum, i) + ' ';

	errors.push_back(errorStr);
	file[strNum].first = curMem;
	file[strNum].second = true;
	ERROR = true;
	curMem += 2;

	return false;
}

void ParseCommand(std::ofstream& out, int strNum)
{
	std::string cmd = CMD_STR(strNum, CMD_IND(strNum).first);
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);

	if (cmd == "BYTE")
	{
		fn_nop(out, strNum);
		return;
	}

	for (auto it = getBeginOps(); it != getEndOps(); it++)
	{
		if (cmd == (*it).second.getName() && (*it).second.getArgs() == ARGS(strNum))
			(*it).second.exec(out, strNum);
	}
}

#endif /* CommandParse.h */