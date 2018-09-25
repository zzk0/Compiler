#include <iostream>
#include <fstream>
#include  <string>
#include "DFA.h"
#include "Lexer.h"
using namespace std;

int main()
{
	Lexer lexer;
	lexer.readText("sample.cpp");
	cout << lexer.text << endl;

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