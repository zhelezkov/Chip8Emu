//
//  Assembler.cpp
//  Chip8Asm
//

#include "Assembler.hpp"
//#include <regex>
#include <ctime>
#include <pcre.h>
#include <pcreposix.h>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <set>

std::string StringNumber(int i)
{
	return "String " + std::to_string(i) + ".";
}

std::set<char> errorSymbols = { '-', ':', '#', '%', '.', '[', ']' };
bool checkForValidStr(std::string s, int start, int end)
{
	for (int i = start; i < end; i++)
		if (errorSymbols.count(s[i]))
			return false;

	return true;
}

bool checkStrForHexNumber(std::string& s, int start, int end)
{
	for (int i = start; i < end; i++)
		if (!(s[i] >= '0' && s[i] <= '9' || s[i] >= 'A' && s[i] <= 'F' || s[i] >= 'a' && s[i] <= 'f'))
			return false;

	return true;
}

bool checkStrForDecNumber(std::string& s, int start, int end)
{
	for (int i = start; i < end; i++)
		if (s[i] < '0' || s[i] > '9')
			return false;

	return true;
}

bool checkStrForBinNumber(std::string& s, int start, int end)
{
	for (int i = start; i < end; i++)
		if (!(s[i] == '.' || s[i] == '1'))
			return false;

	return true;
}

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
	clock_t start = clock();

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
	const int lengthToken = 15;
	const std::pair<int, int> NIL = { -1, -1 };

	struct StringToken
	{
		int arr[lengthToken];
		std::string parsedStr;

		std::pair<int, int> allInd;
		std::string allStr;

		std::pair<int, int> commnetInd;
		std::string commentStr;

		std::pair<int, int> commandInd;
		std::string commandStr;

		std::pair<int, int> errorInd;
		std::string errorStr;

		void Parse(std::string& s)
		{
			allStr = "";
			commentStr = "";
			commandStr = "";
			errorStr = "";

			allInd = std::make_pair(arr[0], arr[1]);
			for (int i = allInd.first; i < allInd.second; i++)
				allStr += s[i];

			commnetInd = std::make_pair(arr[2], arr[3]);
			for (int i = commnetInd.first; i < commnetInd.second; i++)
				commentStr += s[i];

			commandInd = std::make_pair(arr[4], arr[5]);
			for (int i = commandInd.first; i < commandInd.second; i++)
				commandStr += s[i];

			errorInd = std::make_pair(arr[6], arr[7]);
			for (int i = errorInd.first; i < errorInd.second; i++)
				errorStr += s[i];
		}

		std::string getAll()
		{
			parsedStr = "";
			parsedStr += commentStr;
			if (commandStr != "" && parsedStr != "") parsedStr += " ";

			parsedStr += commandStr;
			if (errorStr != "" && parsedStr != "") parsedStr += " ";

			parsedStr += errorStr;

			return parsedStr;
		}
	};
	std::vector<StringToken> ovector;

	char *regex = "(;+.*)?[\\s|,]*([\\w-\\[\\]:#%\\.]+)?[\\s|,]*([^\\w-\\[\\]:#%\\. ,\\s;]+)?";

	re = pcre_compile(regex,          /* the pattern */
		PCRE_MULTILINE,				  /* options */
		&error,						  /* for error message */
		&erroffset,				      /* for error offset */
		NULL);                        /* use default character tables */

	if (!re)
	{
		printf("pcre_compile failed (offset: %d), %s\n", erroffset, error);
		return -1;
	}

	std::vector <std::string> file;
	std::vector <std::string> errors;
	std::map<std::string, ushort> labels;

	std::string s;
	while (getline(inFile, s))
	{
		int offset = 0;
		int ind;
		ovector.clear();

		while (offset < s.length())
		{
			ovector.push_back(StringToken());
			ind = ovector.size() - 1;
			if (pcre_exec(re, 0, s.c_str(), s.length(), offset, 0, ovector[ind].arr, lengthToken) < 0)
				break;

			ovector[ind].Parse(s);
			offset = ovector[ind].arr[1];
		}

		bool ER = false;
		s = "";	// rewrite parsed string
		for (int i = 0; i < ovector.size(); i++)
		{
			if (ovector[i].errorInd != NIL)  // invalid symbol
			{
				errors.push_back(StringNumber(file.size()) + " Illegal symbol: " + ovector[i].errorStr);
				s += " " + ovector[i].getAll();
				ER = true;
				continue;
			}

			if (ovector[i].commandStr == "")
			{
				s += " " + ovector[i].getAll();
				continue;
			}

			if (ovector[i].commandStr.back() == ':') // check for valid label
			{
				if (ovector[i].commandStr.length() == 1)
				{
					errors.push_back(StringNumber(file.size()) + " Empty label: " + ovector[i].commandStr);
					s += " " + ovector[i].getAll();
					ER = true;
					continue;
				}
				
				if (!checkForValidStr(ovector[i].commandStr, 0, ovector[i].commandStr.length() - 1))	// label has invalid symbols
				{
					errors.push_back(StringNumber(file.size()) + " Invalid label: " + ovector[i].commandStr);
					ER = true;
				}
				else labels[ovector[i].commandStr] = file.size();	// add label

				s += " " + ovector[i].getAll();
				continue;
			}

			if (ovector[i].commandStr.front() == '#') // check for valid hex number
			{
				if (!checkStrForHexNumber(ovector[i].commandStr, 1, ovector[i].commandStr.length()))
				{
					errors.push_back(StringNumber(file.size()) + " Invalid hex number: " + ovector[i].commandStr);
					ER = true;
				}

				s += " " + ovector[i].getAll();
				continue;
			}

			if (ovector[i].commandStr.front() == '%') // check for valid bin number
			{
				if (!checkStrForBinNumber(ovector[i].commandStr, 1, ovector[i].commandStr.length()))	// invalid bin number
				{
					errors.push_back(StringNumber(file.size()) + " Invalid bin number: " + ovector[i].commandStr);
					ER = true;
				}

				s += " " + ovector[i].getAll();
				continue;
			}

			if (ovector[i].commandStr.front() == '-') // check for valid negative number
			{
				if (!checkStrForDecNumber(ovector[i].commandStr, 1, ovector[i].commandStr.length()))	// invalid bin number
				{
					errors.push_back(StringNumber(file.size()) + " Invalid dec number: " + ovector[i].commandStr);
					ER = true;
				}

				s += " " + ovector[i].getAll();
				continue;
			}

			if (ovector[i].commandStr.front() == '[') // check for instruction [I] / [i]
			{
				if (ovector[i].commandStr != "[I]" && ovector[i].commandStr != "[i]")
				{
					errors.push_back(StringNumber(file.size()) + " Unknown command: " + ovector[i].commandStr);
					ER = true;
				}

				s += " " + ovector[i].getAll();
				continue;
			}

			// default
			if (!checkForValidStr(ovector[i].commandStr, 0, ovector[i].commandStr.length()))
			{
				errors.push_back(StringNumber(file.size()) + " Invalid symbols: " + ovector[i].commandStr);
			}
			s += " " + ovector[i].getAll();
		}

		// push in file's vector
		if (!ER) errors.push_back(StringNumber(file.size()) + " OK");
		file.push_back(s);
	}

	// print parsed file
	for (int i = 0; i < file.size(); i++)
		outFile << i << " " << file[i] << "\n";

	// print our log
	for (int i = 0; i < errors.size(); i++)
		logFile << errors[i] << "\n";

	for (auto i = labels.begin(); i != labels.end(); i++)
		logFile << (*i).first << " " << (*i).second << "\n";

    logFile << clock() - start;
    return true;
}

int main() 
{
    Assembler Asm("in.c8", "out.txt");
    Asm.Assemble();
    return 0;
}
