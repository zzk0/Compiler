#pragma once

#include "DFA.h"
#include "NFA.h"

void dfa_test_1()
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
	x = "";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 1" << endl;
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


void dfa_test_2()
{
	NFA nfa(4);
	nfa.setStartState(1);
	nfa.addAcceptState(2);
	nfa.addEdge(1, 2, 'b');
	nfa.addEdge(1, 3, '\0');
	nfa.addEdge(2, 2, 'a');
	nfa.addEdge(2, 3, 'a');
	nfa.addEdge(2, 3, 'b');
	nfa.addEdge(3, 1, 'a');

	DFA dfa = nfa.convertToDFA();
	string x = "";
	if (dfa.IsAccepted(x, x.length())) cout << "Failed 2" << endl;

	cout << "Test End" << endl;
}