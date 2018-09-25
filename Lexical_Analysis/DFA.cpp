#include "DFA.h"



DFA::DFA()
{
}


DFA::~DFA()
{
}

void DFA::setStartState(int x)
{
	startState = x;
}

void DFA::setAcceptState(int x)
{
	acceptState = x;
}

void DFA::addState(translate state)
{
	states.push_back(state);
}

bool DFA::IsAccepted(const char *str, int length)
{
	int nextState = startState;
	for (int i = 0; i < length; i++) {
		nextState = states[nextState].table[int(str[i])];
		if (nextState == -1) {
			return false;
		}
	}
	if (nextState == acceptState) {
		return true;
	}
	else {
		return false;
	}
}

bool DFA::IsAccepted(string str, int length)
{
	int nextState = startState;
	for (int i = 0; i < length; i++) {
		nextState = states[nextState].table[int(str[i])];
		if (nextState == -1) {
			return false;
		}
	}
	if (nextState == acceptState) {
		return true;
	}
	else {
		return false;
	}
}