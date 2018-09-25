#include <iostream>
#include <fstream>
#include  <string>
#include "DFA.h"
using namespace std;

int main()
{
	translate a0, a1, a2;
	for (int i = 0; i < 256; i++) {
		a0.table[i] = -1;
		a1.table[i] = -1;
		a2.table[i] = -1;
	}

	a0.table[int('a')] = 1;
	a0.table[int('c')] = 2;
	a1.table[int('a')] = 2;
	a1.table[int('b')] = 1;
	a2.table[int('c')] = 2;

	DFA dfa;
	dfa.setAcceptState(2);
	dfa.setStartState(0);
	dfa.addState(a0);
	dfa.addState(a1);
	dfa.addState(a2);

	string str;
	while (cin >> str) {
		bool x = dfa.IsAccepted(str, str.length());
		cout << str << (x ? " accept" : " reject") << endl;
	}

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