#pragma once

#include "RegularExpression.h"

void re_test1()
{
	string x = "a*b";
	int index = 0;
	NFA nfa = re_to_NFA(x);

	DFA dfa = nfa.convertToDFA();

	x = "";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "ab";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "b";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "aaaab";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "aaaaaaaaaaab";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;

	cout << "Regular Expression Test End" << endl;
}

void re_test2()
{
	string x = "(xy)*";
	int index = 0;
	NFA nfa = re_to_NFA(x);

	DFA dfa = nfa.convertToDFA();

	x = "";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "xyxyxyxy";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "x";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "xyy";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "xxy";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;

	cout << "Regular Expression Test End" << endl;
}

void re_test3()
{
	string x = "ab(ab|cd)*";
	int index = 0;
	NFA nfa = re_to_NFA(x);

	DFA dfa = nfa.convertToDFA();

	x = "";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "ab";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abcd";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abcdab";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "ababababababcd";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;

	cout << "Regular Expression Test End" << endl;
}

void re_test4()
{
	string x = "ab(ab|cd)*(x)*";
	int index = 0;
	NFA nfa = re_to_NFA(x);

	DFA dfa = nfa.convertToDFA();

	x = "";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abcd";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abcdab";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "ababababababcd";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "ababababababcdx";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "ababababababcdxxx";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abx";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "aba";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;

	cout << "Regular Expression Test End" << endl;
}

void re_test5()
{
	string x = "ab(ab|(cd)*)x";
	int index = 0;
	NFA nfa = re_to_NFA(x);

	DFA dfa = nfa.convertToDFA();

	x = "";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abcdx";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abcdcdx";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "ababx";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "ababcdx";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abcdcdcdx";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abx";
	if (!dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "aba";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;

	cout << "Regular Expression Test End" << endl;
}