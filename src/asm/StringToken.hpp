//
//  StringToken.hpp
//  Chip8Asm
//

#ifndef StringToken_hpp
#define StringToken_hpp

#include <string>
#include <tuple>
#include "typeToken.h"
#include "HelpFunctions.h"

const int lengthToken = 15;

struct StringToken
{
	int arr[lengthToken];
	std::string parsedStr;	// comments + cmd + error
	typeToken type = NIL;
	bool error = false;

	std::pair<int, int> allInd;
	std::string allStr;

	std::pair<int, int> commnetInd;
	std::string commentStr;

	std::pair<int, int> commandInd;
	std::string commandStr;

	std::pair<int, int> errorInd;
	std::string errorStr;

	void Parse(const std::string& s)
	{
		allStr = "";
		commentStr = "";
		commandStr = "";
		errorStr = "";

		// all string
		allInd = std::make_pair(arr[0], arr[1]);
		for (int i = allInd.first; i < allInd.second; i++)
			allStr += s[i];

		// commnet string
		commnetInd = std::make_pair(arr[2], arr[3]);
		for (int i = commnetInd.first; i < commnetInd.second; i++)
			commentStr += s[i];

		// command string
		commandInd = std::make_pair(arr[4], arr[5]);
		for (int i = commandInd.first; i < commandInd.second; i++)
			commandStr += s[i];

		// error string
		errorInd = std::make_pair(arr[6], arr[7]);
		for (int i = errorInd.first; i < errorInd.second; i++)
			errorStr += s[i];

		if (errorStr == "") error = false;
		else error = true;

		// type
		if (!error)
		{
			if (commentStr != "") type = COMMENT;
			if (commandStr.length() && type == NIL)
			{
				if (commandStr.front() == '#') type = HEX;
				if (commandStr.front() == '%') type = BIN;
				if (commandStr.front() >= '0' && commandStr.front() <= '9' || commandStr.front() == '-') type = DEC;
				if (commandStr.back() == ':') type = LABEL;
				if (commandStr.front() == '[') type = ADDR;
				if (checkStrForRegister(commandStr)) type = REG;
				if (checkStrForDT(commandStr)) type = DT;
				if (checkStrForST(commandStr)) type = ST;
				if (checkStrForHF(commandStr)) type = HF;
				if (commandStr == "I" || commandStr == "i") type = I;
				if (commandStr == "B" || commandStr == "b") type = B;
				if (commandStr == "R" || commandStr == "r") type = R;
				if (commandStr == "F" || commandStr == "f") type = F;

				if (type == NIL) type = NAME;
			}
		}
		else type = NIL;

		// write parsed str
		parsedStr = "";
		parsedStr += commentStr;
		if (commandStr != "" && parsedStr != "") parsedStr += " ";

		parsedStr += commandStr;
		if (errorStr != "" && parsedStr != "") parsedStr += " ";

		parsedStr += errorStr;
	}
};

#endif /* StringToken.hpp */