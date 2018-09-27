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


void DFA::addState(translate state, string stateID)
{
	states.push_back(state);
	stateIDs.push_back(stateID);
}


int DFA::statesPos(string stateID)
{
	for (int i = 0; i < stateIDs.size(); i++) {
		if (stateIDs[i] == stateID) {
			return i;
		}
	}
	return -1;
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


exDFA::exDFA()
{
}


exDFA::~exDFA()
{
}


void exDFA::resetToStart()
{
	currentState = startState;
	lastAcceptState = -1;
}


void exDFA::setStartState(int x)
{
	startState = x;
}


void exDFA::addAcceptState(int x)
{
	acceptStates.push_back(x);
}


void exDFA::addState(translate state)
{
	states.push_back(state);
}


void exDFA::runOneStep(char x, bool &accepted, int &currentStateID)
{
	int nextState = states[currentState].table[int(x)];
	if (nextState == -1) {
		accepted = false;
	}
	else {
		currentStateID = nextState;
		currentState = nextState;
		accepted = false;
		for (unsigned int i = 0; i < acceptStates.size(); i++) {
			if (acceptStates[i] == currentState) {
				accepted = true;
				lastAcceptState = currentState;
			}
		}
	}
}


void exDFA::convertFromDFAs(vector<DFA> DFAs)
{
}


int exDFA::getLastAcceptState()
{
	return lastAcceptState;
}