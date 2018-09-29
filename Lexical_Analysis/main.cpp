#include <iostream>
#include <fstream>
#include  <string>
#include "DFA.h"
#include "NFA.h"
#include "Lexer.h"
#include "NFATests.h"
#include "RETests.h"
#include "DFATests.h"
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

	//string x
	//int index = 0;
	//NFA nfa = re_to_NFA(x);

	//DFA dfa = nfa.convertToDFA();
	//x;
	//while (cin >> x)
	//{
	//	cout << (dfa.IsAccepted(x, x.length()) ? "Accept" : "Reject") << endl;
	//}

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