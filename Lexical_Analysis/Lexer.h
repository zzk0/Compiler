/*
参考 Modern Compiler Implementation in C P24 figure 2.5
保存两个状态变量，一个表示上一次进入accept state时，state上的标号。一个表示当前进入的状态的标号。
扫描的过程，保存三个位置变量，一个变量在不断的前进，一个变量保存这一轮扫描开始时的位置，
还有一个变量保存这一轮上一次前进成功的位置。如果可以前进，要更新第三个变量。
如果不能前进，那么输出第二个变量到第三个变量这个位置范围的字符，并且输出上一次进入accpet state的状态的标号。
然后更新三个位置变量为第二位置变量的数值。
*/

#pragma once
#include "DFA.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
class Lexer
{
public:
	Lexer();
	~Lexer();

	vector<string> patterns;
	DFA dfa;
	string text;
	int textPos;

	void readText(const char *fileName);
	void addPattern(string pattern);
	void convertToDFA();
	vector<pair<string, string>> scan();
};

