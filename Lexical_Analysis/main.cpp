#include <iostream>
#include <fstream>
#include  <string>
#include "DFA.h"
#include "NFA.h"
#include "Lexer.h"
#include "NFATests.h"
using namespace std;

int main()
{
	nfa_test_1();
	nfa_test_2();
	nfa_test_3();
	system("pause");
}

/*
stupid scanner
//Scanner scanner("sample.cpp");
//scanner.StartScan();
//scanner.OutputResult();
*/

/*
Read File sample code:
ifstream infile;
infile.open("sample.cpp");

if (!infile) {
cout << "Open Failed" << endl;
return 0;
}
char ch;
while (!infile.eof()) {
infile.get(ch);
if (ch == '\n') break;
cout << ch;
}
ch = infile.peek();
cout << ch;
ch = infile.get();
cout << ch;
*/