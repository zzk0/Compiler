#include <iostream>
#include <fstream>
#include  <string>
#include "DFA.h"
#include "NFA.h"
#include "Lexer.h"
using namespace std;

int main()
{
	DFA dfa;
	dfa.addState(translate());
	for (int i = 0; i < 256; i++) {
		cout << dfa.states[0].table[i];
	}

	NFA nfa(4);
	nfa.setStartState(1);
	nfa.addAcceptState(1);
	nfa.addEdge(1, 2, 'b');
	nfa.addEdge(1, 3, '\0');
	nfa.addEdge(2, 2, 'a');
	nfa.addEdge(2, 3, 'a');
	nfa.addEdge(2, 3, 'b');
	nfa.addEdge(3, 1, 'a');

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