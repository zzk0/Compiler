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
	NFA nfa = re_to_NFA(x);

	DFA dfa = nfa.convertToDFA();

	x = " ";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
	x = "abcdx ";
	if (dfa.IsAccepted(x, x.length())) cout << "Test Failed" << endl;
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


/*
我这里的实现最大的问题在于并联产生了很多的状态。这些其实可以化简成一个状态机的。
*/
void re_test6()
{
	string str = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_)(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9)*";
	
	NFA nfa = re_to_NFA(str);
	DFA dfa = nfa.convertToDFA();

 	str = "asdf1";
	if (!dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
	str = "a";
	if (!dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
	str = "a1";
	if (!dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
	str = "_a";
	if (!dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
	str = "_a1";
	if (!dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
	str = "1_a1";
	if (dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
}


/*
转义字符测试
*/
void re_test7()
{
	string str = R"(/\*(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9)*\*/)";

	NFA nfa = re_to_NFA(str);
	DFA dfa = nfa.convertToDFA();

	str = "/";
	if (dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
	str = "/*commnt*/";
	if (!dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
	str = "/**";
	if (dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
}


void re_test8()
{
	string str = R"(\()";

	NFA nfa = re_to_NFA(str);
	DFA dfa = nfa.convertToDFA();

	str = "(";
	if (!dfa.IsAccepted(str, str.length())) cout << "Test Failed" << endl;
}