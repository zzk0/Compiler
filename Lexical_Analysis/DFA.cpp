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


void DFA::addAcceptState(int x)
{
	acceptStates.push_back(x);
}


void DFA::addState(translate state)
{
	//states.push_back(state);
	states[index] = state;
	index++;
}


void DFA::addState(translate state, string stateID)
{
	//states.push_back(state);
	states[index] = state;
	index++;
	stateIDs[states.size() - 1] = stateID;
	//stateIDs.push_back(stateID);
}

void DFA::addStateID(int x, string y)
{
	stateIDs[x] = y;
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
	for (int i = 0; i < acceptStates.size(); i++)
	{
		if (nextState == acceptStates[i])
		{
			return true;
		}
	}
	return false;
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
	for (int i = 0; i < acceptStates.size(); i++)
	{
		if (nextState == acceptStates[i])
		{
			return true;
		}
	}
	return false;
}


void DFA::runOneStep(char x, bool &accepted, int &currentStateID)
{
	int nextState = states[currentState].table[int(x)];
	currentStateID = nextState;
	if (nextState == -1) {
		accepted = false;
	}
	else {
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


void DFA::reset()
{
	currentState = startState;
}


int DFA::getLastAcceptState()
{
	return lastAcceptState;
}


void DFA::removeUnreachableStates()
{
	set<int> reachableStates = { startState };
	set<int> newStates = { startState };
	set <int> difference;

	do
	{
		set<int> temp;
		for (int x : newStates) 
		{
			for (int y = 0; y < 128; y++)
			{
				int nextState = states[x].table[y];
				if (nextState != -1) {
					temp.insert(nextState);
				}
			}
		}
		newStates.clear();
		set_difference(temp.begin(), temp.end(), reachableStates.begin(), reachableStates.end(), inserter(newStates, newStates.begin()));
		set_union(temp.begin(), temp.end(), reachableStates.begin(), reachableStates.end(), inserter(reachableStates, reachableStates.begin()));

	} while (newStates.size() != 0);

	set<int> allStates;
	for(int i=0; i<states.size(); i++) {
		allStates.insert(i);
	}

	set<int> unreachableStates;
	set_difference(allStates.begin(), allStates.end(), reachableStates.begin(), reachableStates.end(), inserter(unreachableStates, unreachableStates.begin()));

	for (auto x : unreachableStates)
	{
		states.erase(x);
	}
}


void DFA::mergeNondistinguishableStates()
{
}

void DFA::minized()
{
	removeUnreachableStates();
	mergeNondistinguishableStates();
}