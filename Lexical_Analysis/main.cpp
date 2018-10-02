#include <iostream>
#include <fstream>
#include  <string>
#include "DFA.h"
#include "NFA.h"
#include "Lexer.h"
#include "DFATests.h"
#include "NFATests.h"
#include "RETests.h"
#include "Lexer.h"
#include "LexerTests.h"
using namespace std;

int main()
{
	dfa_test_2();

	nfa_test_1();
	nfa_test_2();
	nfa_test_3();
	nfa_test_4();
	nfa_test_5();
	nfa_test_6();

	re_test1();
	re_test2();
	re_test3();
	re_test4();
	re_test5();
	re_test6();

	lexer_test1();

	system("pause");
}

/*
stupid scanner
//Scanner scanner("sample.cpp");
//scanner.StartScan();
//scanner.OutputResult();
*/

/*
preprocess
string x = "(x*|xy)";
string y = preprocess(x);
cout << y << endl;

x = "x(y)*";
y = preprocess(x);
cout << y << endl;

x = "((xy)*|xyz*)";
y = preprocess(x);
cout << y << endl;
*/
