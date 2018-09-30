#include "Lexer.h"



Lexer::Lexer()
{
}


Lexer::~Lexer()
{
}


/*
Reference:
https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
*/
void Lexer::readText(const char *fileName)
{
	ifstream t(fileName);
	stringstream buffer;
	buffer << t.rdbuf();
	text = buffer.str();
}


void Lexer::addPattern(string pattern)
{
	patterns.push_back(pattern);
}


void Lexer::convertToDFA()
{
	for (int i = 0; i < patterns.size(); i++) {
		//DFAs.push_back(re_to_NFA(patterns[i]).convertToDFA);
	}
}


vector<pair<string, string>> Lexer::scan()
{
	int textLength = text.length();
	return vector<pair<string, string>>();
}
