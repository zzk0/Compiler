#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

enum TokenType {
	IF, ELSE,  FOR, DO, WHILE, // control
	OP, // operator + - * / += -= *= /= =  // < > == != << >>
	ASSIGN, MACRO, DOT, SEMICOLON,
	NUM, ID, STRING,// number and identifier
	BRACKETS,
	KEYWORD,
	UNKNOWN
};

struct Token {
	string str;
	TokenType type;
	int line_number;

	Token(string s, TokenType t, int x) : str(s), type(t), line_number(x)
	{

	}
};

class Scanner
{
private:
	const char *fileName;
	vector<Token> Tokens;
	ifstream infile;

	bool InAlphabet(char x);
	bool IsNumber(char x);
	bool IsKeyword(string str);
	bool IsIncludePath();

public:
	Scanner();
	Scanner(const char *name);
	~Scanner();

	void OpenFile();
	void StartScan();
	void OutputResult();
};

