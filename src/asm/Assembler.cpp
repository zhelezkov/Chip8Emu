//
//  Assembler.cpp
//  Chip8Asm
//

#define LOGURU_IMPLEMENTATION 1
#include "Assembler.hpp"
#include "HelpFunctions.h"
#include "StringToken.hpp"
#include "CommandParse.h"
#include "Opcodes.h"
#include "Defines.h"

//#include <regex>
#include <ctime>
#include <pcre.h>
#include <pcreposix.h>
#include <map>
#include <set>

extern const int lengthToken;					// in StringToken.hpp
std::vector <std::pair<int, bool>> file;		// parsed file
std::vector <std::string> errors;				// errors
std::map<std::string, int> labels;				// labels 
std::map<std::string, byte> var;				// names of registers (var)					
std::map<std::string, byte> equ;				// const (equ)	
bool ERROR = false;								// global error
std::vector<VECTOR_TYPE> ovector;				// parsed file here

Assembler::Assembler(const char * in, const char * out) : in(in), out(out)
{

}

bool Assembler::Assemble()
{
    loguru::add_file(log, loguru::FileMode::Truncate, loguru::Verbosity_MAX);
    std::ifstream inFile(in, std::ios::in);
    std::ofstream outFile(out, std::ios::binary | std::ios::out);

	std::ofstream temp("Assemble.txt", std::ios::out);

    /************************************ check file for opened ***************************/
    CHECK_F(inFile.is_open(), "Error: unable to open file '%s'", in);
    
    LOG_F(INFO, "OK: file is opened.");
	clock_t start = clock();

	const char *error;
	int   errorffset;
	pcre *re;

	const char* regex = "(;+.*)?[\\s|,]*([\\w-\\[\\]:#%\\.]+)?[\\s|,]*([^\\w-\\[\\]:#%\\. ,\\s;]+)?";

	re = pcre_compile(regex,          /* the pattern */
		PCRE_MULTILINE,				  /* options */
		&error,						  /* for error message */
		&errorffset,				      /* for error offset */
		NULL);                        /* use default character tables */

    
    CHECK_NOTNULL_F(re, "pcre_compile failed (offset: %d), %s", errorffset, error);

	std::string s;
	while (getline(inFile, s))
	{
		ovector.push_back(NEWSTR);
		int offset = 0;
		int strNum = ovector.size() - 1;
		int TokenNum;

		while (offset < s.length())
		{
			STRING(strNum).push_back(StringToken());
			TokenNum = STRING(strNum).size() - 1;
			if (pcre_exec(re, 0, s.c_str(), s.length(), offset, 0, TOKEN(strNum, TokenNum).arr, lengthToken) < 0)
				break;

			TOKEN(strNum, TokenNum).Parse(s);
			offset = TOKEN(strNum, TokenNum).arr[1];
		}
	}

	/*********************************** labels and syntaxis parse ************************/
	for (int strNum = 0; strNum < ovector.size(); strNum++)
	{
		bool errorStr = false;
		for (int TokenNum = 0; TokenNum < STRING(strNum).size(); TokenNum++)
		{
			/********************** invalid symbol **********************/
			if (TOKEN(strNum, TokenNum).error)
			{
				errors.push_back(StringNumber(strNum) + " Illegal symbol: " + ERROR_STR(strNum, TokenNum));
				ERROR = true;
				errorStr = true;
				continue;
			}

			/***************** empty command - next string **************/
			if (CMD_STR(strNum, TokenNum) == "")
			{
				continue;
			}

			/****************** check for valid label *******************/
			if (TOKEN(strNum, TokenNum).type == LABEL)
			{
				/******************** empty label ********************/
				if (CMD_STR(strNum, TokenNum).length() == 1)
				{
					errors.push_back(StringNumber(strNum) + " Empty label: " + CMD_STR(strNum, TokenNum));
					ERROR = true;
					errorStr = true;
					continue;
				}

				/********************* label has invalid symbols ******************/
				if (!checkForValidStr(CMD_STR(strNum, TokenNum), 0, CMD_STR(strNum, TokenNum).length() - 1))
				{
					errors.push_back(StringNumber(strNum) + " Invalid label: " + CMD_STR(strNum, TokenNum));
					ERROR = true;
					errorStr = true;
				}
				else
				{
					/********************* Redefinition **********************/
					if (Redefinition(CMD_STR(strNum, TokenNum)))
					{
						errors.push_back(StringNumber(strNum) + " Redefinition: " + CMD_STR(strNum, TokenNum));
						ERROR = true;
						errorStr = true;
					}
					/******************** add label ******************/
					else
					{
						std::string label = CMD_STR(strNum, TokenNum);
						label.erase(label.end() - 1);
						labels[label] = strNum;
					}
				}
				continue;
			}

			/*************** check for valid hex number *****************/
			if (TOKEN(strNum, TokenNum).type == HEX)
			{
				if (!checkStrForHexNumber(CMD_STR(strNum, TokenNum), 1))
				{
					errors.push_back(StringNumber(strNum) + " Invalid hex number: " + CMD_STR(strNum, TokenNum));
					ERROR = true;
					errorStr = true;
				}
				continue;
			}

			/*************** check for valid bin number *****************/
			if (TOKEN(strNum, TokenNum).type == BIN)
			{
				if (!checkStrForBinNumber(CMD_STR(strNum, TokenNum), 1))
				{
					errors.push_back(StringNumber(strNum) + " Invalid bin number: " + CMD_STR(strNum, TokenNum));
					ERROR = true;
					errorStr = true;
				}

				continue;
			}

			/***************** check for valid dec number ***************/
			if (TOKEN(strNum, TokenNum).type == DEC)
			{
				if (!checkStrForDecNumber(CMD_STR(strNum, TokenNum)))
				{
					errors.push_back(StringNumber(strNum) + " Invalid dec number: " + CMD_STR(strNum, TokenNum));
					ERROR = true;
					errorStr = true;
				}
				continue;
			}

			/*************** check for instruction [I] / [i] ************/
			if (CMD_STR(strNum, TokenNum).front() == '[')
			{
				if (CMD_STR(strNum, TokenNum) != "[I]" && CMD_STR(strNum, TokenNum) != "[i]")
				{
					errors.push_back(StringNumber(strNum) + " Unknown command: " + CMD_STR(strNum, TokenNum));
					ERROR = true;
					errorStr = true;
				}

				continue;
			}

			/****************************** default *********************/
			if (!checkForValidStr(CMD_STR(strNum, TokenNum)))
			{
				errors.push_back(StringNumber(strNum) + " Invalid symbols: " + CMD_STR(strNum, TokenNum));
			}
		}

		file.push_back(std::make_pair(-1, errorStr));
	}
	if (ERROR) goto END;

	/*********************************** command and memory parse *************************/
    {
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
			if(curInd < STRING(strNum).size())
				if(TOKEN(strNum, curInd).type == LABEL)
					LabelsToken.second++;
				else break;
			else break;
		}
		if (LabelsToken.first > LabelsToken.second) LabelsToken = NILtoken;
		LABEL_IND(strNum) = LabelsToken;

		/********************* cmd ****************************/
		std::pair<int, int> CommandToken = { LabelsToken.second + 1,  LabelsToken.second };
		while (1)
		{
			int curInd = CommandToken.second + 1;
			if (curInd < STRING(strNum).size())
				if (checkTypeForArg(TOKEN(strNum, curInd).type))
					CommandToken.second++;
				else break;
			else break;
		}
		if (CommandToken.first > CommandToken.second) CommandToken = NILtoken;
		CMD_IND(strNum) = CommandToken;

		/******************* comments *************************/
		std::pair<int, int> CommentToken = { CommandToken.second + 1, CommandToken.second };
		while (1)
		{
			int curInd = CommentToken.second + 1;
			if (curInd < STRING(strNum).size())
				if (TOKEN(strNum, curInd).type == COMMENT || TOKEN(strNum, curInd).type == NIL)
					CommentToken.second++;
				else break;
			else break;
			CommentToken.second++;
		}
		if (CommentToken.first > CommentToken.second) CommentToken = NILtoken;
		COMMENT_IND(strNum) = CommentToken;

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
			if (CommandToken == NILtoken)
				if(LabelsToken != NILtoken)
				{
					file[strNum].first = curMem;
					file[strNum].second = false;
					continue;
				}

			CheckCommand(strNum, curMem);
		}
		/******************** empty str or comments **************/
		else
		{
			file[strNum].first = -1;
		}
	}

	if (ERROR) goto END;


	/************************************ make bin file **********************************/
	for (int strNum = 0; strNum < ovector.size(); strNum++)
	{
		if(CMD_IND(strNum) != NILtoken) ParseCommand(outFile, strNum);
	}
    }

	END:
	// print parsed file
	for (int strNum = 0; strNum < file.size(); strNum++)
	{
		temp << '[' << std::dec << strNum;
		if (!file[strNum].second) temp << "+] ";
		else temp << "-] ";

		if(file[strNum].first != -1)
			temp << "[0x" << std::hex << file[strNum].first << "] ";
		
		for (int TokenNum = 0; TokenNum < STRING(strNum).size(); TokenNum++)
		{
			temp << TOKEN(strNum, TokenNum).parsedStr + "(" + TypeTokenStr[TOKEN(strNum, TokenNum).type] + ") ";
		}
		temp << "\n";
	}

	// print our log
    LOG_F(INFO, "Errors: %lu", errors.size());
    for (int i = 0; i < errors.size(); i++)
        LOG_F(INFO, "%s", errors[i].c_str());

    LOG_F(INFO, "Labels:");
	for (auto i = labels.begin(); i != labels.end(); i++)
        LOG_F(INFO, "%s [0x%x]", (*i).first.c_str(), file[(*i).second].first);

    LOG_F(INFO, "Var: ");
	for (auto i = var.begin(); i != var.end(); i++)
        LOG_F(INFO, "%s = v%x", (*i).first.c_str(), (*i).second);
	
    LOG_F(INFO, "Conts: ");
	for (auto i = equ.begin(); i != equ.end(); i++)
        LOG_F(INFO, "%s = %d", (*i).first.c_str(), (*i).second);
    
    LOG_F(INFO, "Time: %lu", clock() - start);
    return true;
}

int main(int argc, char* argv[]) {
    CHECK_F(argc >= 2, "Not enough arguments.");
    int inLen = strlen(argv[1]);
    char* outFile = (char*) malloc((inLen + 5) * sizeof(char));
    strcpy(outFile, argv[1]);
    strcat(outFile, ".bin");
    
    Assembler Asm(argv[1], outFile);
    Asm.Assemble();
    
    free(outFile);
    return 0;
}
