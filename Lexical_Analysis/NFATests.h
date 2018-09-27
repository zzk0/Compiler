#pragma once

#include "NFA.h"

void nfa_test_1()
{
	NFA nfa(4);
	nfa.setStartState(1);
	nfa.addAcceptState(1);
	nfa.addEdge(1, 2, 'b');
	nfa.addEdge(1, 3, '\0');
	nfa.addEdge(2, 2, 'a');
	nfa.addEdge(2, 3, 'a');
	nfa.addEdge(2, 3, 'b');
	nfa.addEdge(3, 1, 'a');

	DFA dfa = nfa.convertToDFA();
	string x = "aaaaaaaaaaaaa";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "baa";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "bbaaaaa";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "baaaaa";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "aaaxaaaaaaaaa";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "bab";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "bba";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "aaab";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "bbb";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	x = "baaaaaaaaaaaaaaaaaaaaaba";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
	
	cout << "Test End" << endl;
}

void nfa_test_2()
{
	NFA nfa(5);
	nfa.setStartState(0);
	nfa.addAcceptState(3);
	nfa.addAcceptState(4);
	nfa.addEdge(0, 1, '\0');
	nfa.addEdge(0, 2, '\0');
	nfa.addEdge(1, 3, 'a');
	nfa.addEdge(1, 4, 'b');
	nfa.addEdge(2, 4, 'c');
	nfa.addEdge(3, 4, 'd');
	nfa.addEdge(4, 4, 'e');

	DFA dfa = nfa.convertToDFA();
	string x = "a";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "ad";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "adeeeee";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "b";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "caaa";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "ab";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "ceeeee";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "de";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "adec";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;
	x = "ade";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;

	cout << "Test End" << endl;
}

/*
ITOC example 1.30 P51
*/
void nfa_test_3()
{
	NFA nfa(4);
	nfa.setStartState(0);
	nfa.addAcceptState(3);
	nfa.addEdge(0, 0, '0');
	nfa.addEdge(0, 0, '1');
	nfa.addEdge(0, 1, '1');
	nfa.addEdge(1, 2, '0');
	nfa.addEdge(1, 2, '1');
	nfa.addEdge(2, 3, '0');
	nfa.addEdge(2, 3, '1');

	DFA dfa = nfa.convertToDFA();
	string x = "000100";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "01010101100";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "111";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "101";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "0011";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "01011";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "111111111";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "101010";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "010";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;
	x = "1100";
	if (!dfa.IsAccepted(x, x.length())) cout << "Failed 3" << endl;

	cout << "Test End" << endl;
}


