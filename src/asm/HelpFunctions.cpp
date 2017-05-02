//
//  HelpFunctions.cpp
//  Chip8Asm
//

#include "HelpFunctions.h"

#include "Opcodes.h"

std::string StringNumber(int i)
{
	return "String " + std::to_string(i) + ".";
}

bool checkForValidStr(const std::string s, int start, int end)
{
	if (start == -1) start = 0;
	if (end == -1) end = s.length();

	for (int i = start; i < end; i++)
		if (errorSymbols.count(s[i]))
			return false;

	return true;
}

bool checkStrForHexNumber(const std::string& s, int start, int end)
{
	if (start == -1) start = 0;
	if (end == -1) end = s.length();

	if (s[start] == '-')
		if (s.length() != 1) start++;
		else return false;

		for (int i = start; i < end; i++)
			if (!(s[i] >= '0' && s[i] <= '9' || s[i] >= 'A' && s[i] <= 'F' || s[i] >= 'a' && s[i] <= 'f'))
				return false;

		return true;
}

bool checkStrForDecNumber(const std::string& s, int start, int end)
{
	if (start == -1) start = 0;
	if (end == -1) end = s.length();

	if (s[start] == '-')
		if (s.length() != 1) start++;
		else return false;

		for (int i = start; i < end; i++)
			if (s[i] < '0' || s[i] > '9')
				return false;

		return true;
}

bool checkStrForBinNumber(const std::string& s, int start, int end)
{
	if (start == -1) start = 0;
	if (end == -1) end = s.length();

	if (s[start] == '-')
		if (s.length() != 1) start++;
		else return false;

		for (int i = start; i < end; i++)
			if (!(s[i] == '0' || s[i] == '1'))
				return false;

		return true;
}

bool checkStrForRegister(const std::string& s)
{
	if (s.length() != 2) return false;

	if ((s[0] == 'v' || s[0] == 'V') && (s[1] >= '0' && s[1] <= '9' || s[1] >= 'A' && s[1] <= 'F' || s[1] >= 'a' && s[1] <= 'f'))
		return true;
	else return false;
}

bool checkStrForCommand(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	for (auto it = getBeginOps(); it != getEndOps(); it++)
	{
		if (s == (*it).second.getName())
			return true;
	}
	return false;
}

bool checkStrForDT(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	if (s == "DT")return true;
	else return false;
}

bool checkStrForST(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	if (s == "ST")return true;
	else return false;
}

bool checkStrForHF(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	if (s == "HF")return true;
	else return false;
}

bool checkTypeForArg(typeToken type)
{
	return type != LABEL && type != COMMENT && type != NIL;
}

ushort strToNumber(std::string str)
{
	if (str.length() == 0) return 0;

	if (str[0] == '%')
	{
		str.erase(str.begin());
		return std::stoi(str, nullptr, 2);
	}

	if (str[0] == '#')
	{
		str.erase(str.begin());
		return std::stoi(str, nullptr, 16);
	}

	if (str[0] == 'v' || str[0] == 'V')
	{
		str.erase(str.begin());
		return std::stoi(str, nullptr, 16);
	}

	return std::stoi(str, nullptr, 10);
}

bool Redefinition(const std::string& str)
{
	std::string temp = str;
	std::transform(temp.begin(), temp.end(), temp.begin(), toupper);
	if (temp == "EQU" || temp == "VAR" || temp == "BYTE") return true;

	return labels.count(str) || equ.count(str) || var.count(str) || checkStrForRegister(str) || checkStrForCommand(str);
}
