//
//  Assembler.cpp
//  Chip8Asm
//

#include "Assembler.hpp"
#include "HelpFunctions.h"
#include "StringToken.hpp"
#include "CommandParse.h"

//#include <regex>
#include <ctime>
#include <pcre.h>
#include <pcreposix.h>
#include <map>
#include <set>

extern const int lengthToken;					// in StringToken.hpp
std::vector <std::pair<int, bool>> file;		// parsed file
std::vector <std::string> errors;				// errors
std::map<std::string, ushort> labels;			// labels 
std::map<std::string, byte> var;				// names of registers (var)					
std::map<std::string, byte> equ;				// const (equ)	
bool ERROR = false;								// global error
std::vector<std::vector<StringToken>> ovector;	// parsed file here

Assembler::Assembler(const char * in, const char * out) : in(in), out(out)
{

}

bool Assembler::Assemble()
{
    std::ifstream inFile(in, std::ios::binary | std::ios::in);
    std::ofstream outFile(out, std::ios::out);
    std::ofstream logFile(this->log, std::ios::out);

	std::ofstream temp("Assemble.txt", std::ios::out);

    /************************************ check file for opened ***************************/
    if (!inFile.is_open())
    {
        logFile << "Error: unable to open file '" << in << "'\n";
        return false;
    }
    logFile << "OK: file is opened.\n\n";
	clock_t start = clock();

	/*************** It's too long time ***************/
    /*std::string s;
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
    }*/

	const char *error;
	int   erroffset;
	pcre *re;

	char *regex = "(;+.*)?[\\s|,]*([\\w-\\[\\]:#%\\.]+)?[\\s|,]*([^\\w-\\[\\]:#%\\. ,\\s;]+)?";

	re = pcre_compile(regex,          /* the pattern */
		PCRE_MULTILINE,				  /* options */
		&error,						  /* for error message */
		&erroffset,				      /* for error offset */
		NULL);                        /* use default character tables */

	if (!re)
	{
		logFile << "pcre_compile failed (offset: " << erroffset << "), " << error << "\n";
		return -1;
	}

	std::string s;
	while (getline(inFile, s))
	{
		ovector.push_back(std::vector<StringToken>());
		int offset = 0;
		int strNum = ovector.size() - 1;
		int TokenNum;

		while (offset < s.length())
		{
			ovector[strNum].push_back(StringToken());
			TokenNum = ovector[strNum].size() - 1;
			if (pcre_exec(re, 0, s.c_str(), s.length(), offset, 0, ovector[strNum][TokenNum].arr, lengthToken) < 0)
				break;

			ovector[strNum][TokenNum].Parse(s);
			offset = ovector[strNum][TokenNum].arr[1];
		}
	}

	/*********************************** labels and syntaxis parse ************************/
	for (int strNum = 0; strNum < ovector.size(); strNum++)
	{
		bool errorStr = false;
		for (int TokenNum = 0; TokenNum < ovector[strNum].size(); TokenNum++)
		{
			/********************** invalid symbol **********************/
			if (ovector[strNum][TokenNum].error)
			{
				errors.push_back(StringNumber(strNum) + " Illegal symbol: " + ovector[strNum][TokenNum].errorStr);
				ERROR = true;
				errorStr = true;
				continue;
			}

			/***************** empty command - next string **************/
			if (ovector[strNum][TokenNum].commandStr == "")
			{
				continue;
			}

			/****************** check for valid label *******************/
			if (ovector[strNum][TokenNum].type == LABEL)
			{
				/******************** empty label ********************/
				if (ovector[strNum][TokenNum].commandStr.length() == 1)
				{
					errors.push_back(StringNumber(strNum) + " Empty label: " + ovector[strNum][TokenNum].commandStr);
					ERROR = true;
					errorStr = true;
					continue;
				}

				/********************* label has invalid symbols ******************/
				if (!checkForValidStr(ovector[strNum][TokenNum].commandStr, 0, ovector[strNum][TokenNum].commandStr.length() - 1))
				{
					errors.push_back(StringNumber(strNum) + " Invalid label: " + ovector[strNum][TokenNum].commandStr);
					ERROR = true;
					errorStr = true;
				}
				else
				{
					/********************* Redefinition **********************/
					if (Redefinition(ovector[strNum][TokenNum].commandStr))
					{
						errors.push_back(StringNumber(strNum) + " Redefinition: " + ovector[strNum][TokenNum].commandStr);
						ERROR = true;
						errorStr = true;
					}
					/******************** add label ******************/
					else
					{
						labels[ovector[strNum][TokenNum].commandStr] = strNum;
					}
				}
				continue;
			}

			/*************** check for valid hex number *****************/
			if (ovector[strNum][TokenNum].type == HEX)
			{
				if (!checkStrForHexNumber(ovector[strNum][TokenNum].commandStr, 1))
				{
					errors.push_back(StringNumber(strNum) + " Invalid hex number: " + ovector[strNum][TokenNum].commandStr);
					ERROR = true;
					errorStr = true;
				}
				continue;
			}

			/*************** check for valid bin number *****************/
			if (ovector[strNum][TokenNum].type == BIN)
			{
				if (!checkStrForBinNumber(ovector[strNum][TokenNum].commandStr, 1))
				{
					errors.push_back(StringNumber(strNum) + " Invalid bin number: " + ovector[strNum][TokenNum].commandStr);
					ERROR = true;
					errorStr = true;
				}

				continue;
			}

			/***************** check for valid dec number ***************/
			if (ovector[strNum][TokenNum].type == DEC)
			{
				if (!checkStrForDecNumber(ovector[strNum][TokenNum].commandStr))
				{
					errors.push_back(StringNumber(strNum) + " Invalid dec number: " + ovector[strNum][TokenNum].commandStr);
					ERROR = true;
					errorStr = true;
				}
				continue;
			}

			/*************** check for instruction [I] / [i] ************/
			if (ovector[strNum][TokenNum].commandStr.front() == '[')
			{
				if (ovector[strNum][TokenNum].commandStr != "[I]" && ovector[strNum][TokenNum].commandStr != "[i]")
				{
					errors.push_back(StringNumber(strNum) + " Unknown command: " + ovector[strNum][TokenNum].commandStr);
					ERROR = true;
					errorStr = true;
				}

				continue;
			}

			/****************************** default *********************/
			if (!checkForValidStr(ovector[strNum][TokenNum].commandStr))
			{
				errors.push_back(StringNumber(strNum) + " Invalid symbols: " + ovector[strNum][TokenNum].commandStr);
			}
		}

		file.push_back(std::make_pair(-1, errorStr));
	}
	if (ERROR) goto END;

	/*********************************** command and memory parse *************************/
	int curMem = 0x200;
	const std::pair<int, int> NILtoken = { -1, -1 };
	// string == label + cmd + comments
	for (int strNum = 0; strNum < ovector.size(); strNum++)
	{
		/******************** label ***************************/
		std::pair<int, int> LabelsToken = { 0, -1 };
		while (1)
		{
			int curInd = LabelsToken.second + 1;
			if(curInd < ovector[strNum].size())
				if(ovector[strNum][curInd].type == LABEL)
					LabelsToken.second++;
				else break;
			else break;
		}
		if (LabelsToken.first > LabelsToken.second) LabelsToken = NILtoken;

		/********************* cmd ****************************/
		std::pair<int, int> CommandToken = { LabelsToken.second + 1,  LabelsToken.second };
		while (1)
		{
			int curInd = CommandToken.second + 1;
			if (curInd < ovector[strNum].size())
				if (checkTypeForArg(ovector[strNum][curInd].type))
					CommandToken.second++;
				else break;
			else break;
		}
		if (CommandToken.first > CommandToken.second) CommandToken = NILtoken;

		/******************* comments *************************/
		std::pair<int, int> CommentToken = { CommandToken.second + 1, CommandToken.second };
		while (1)
		{
			if (CommentToken.second + 1 < ovector[strNum].size())
				if (ovector[strNum][CommentToken.second + 1].type == COMMENT || ovector[strNum][CommentToken.second + 1].type == NIL)
					CommentToken.second++;
				else break;
			else break;
			CommentToken.second++;
		}
		if (CommentToken.first > CommentToken.second) CommentToken = NILtoken;

		if (LabelsToken != NILtoken || CommandToken != NILtoken)
		{
			/******************** check for too much labels ***********************/
			if (LabelsToken != NILtoken && LabelsToken.second - LabelsToken.first > 0)
			{
				errors.push_back(StringNumber(strNum) + " Too much labels.");
				file[strNum].second = true;
				ERROR = true;
				continue;
			}

			/************************** parse command *****************************/
			if (CommandToken == NILtoken) continue;
			CheckCommand(strNum, CommandToken, curMem);
		}
		/******************** empty str or comments **************/
		else
		{
			file[strNum].first = -1;
		}
	}
	if (ERROR) goto END;


	/************************************ make bin file **********************************/


	END:
	// print parsed file
	for (int strNum = 0; strNum < file.size(); strNum++)
	{
		temp << '[' << std::dec << strNum;
		if (!file[strNum].second) temp << "+] ";
		else temp << "-] ";

		if(file[strNum].first != -1)
			temp << "[0x" << std::hex << file[strNum].first << "] ";
		
		for (int TokenNum = 0; TokenNum < ovector[strNum].size(); TokenNum++)
		{
			temp << ovector[strNum][TokenNum].parsedStr + "(" + typeTokenStr[ovector[strNum][TokenNum].type] + ") ";
		}
		temp << "\n";
	}

	// print our log
	logFile << "Errors: \n";
	for (int i = 0; i < errors.size(); i++)
		logFile << errors[i] << "\n";
	logFile << "\n";

	logFile << "Labels: \n";
	for (auto i = labels.begin(); i != labels.end(); i++)
		logFile << (*i).first << " " << std::hex << "[0x" <<file[(*i).second].first << "]\n";
	logFile << "\n";

	logFile << "Var: \n";
	for (auto i = var.begin(); i != var.end(); i++)
	{
		logFile << (*i).first << " = v" << std::hex << (int)(*i).second << "\n";
	}
	logFile << "\n";

	logFile << "Const: \n";
	for (auto i = equ.begin(); i != equ.end(); i++)
	{
		logFile << (*i).first << " = " << std::dec << (int)(*i).second << "\n";
	}
	logFile << "\n";

    logFile << "Time: " << clock() - start;
    return true;
}

int main() 
{
    Assembler Asm("in.c8", "out.txt");
    Asm.Assemble();
    return 0;
}
