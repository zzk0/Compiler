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
	set<int> finalStates;
	set<int> otherStates;
	set<int> allStates;
	for (int i = 0; i < acceptStates.size(); i++)
	{
		finalStates.insert(acceptStates[i]);
	}
	for (int i = 0; i < states.size(); i++)
	{
		allStates.insert(i);
	}
	set_difference(allStates.begin(), allStates.end(), finalStates.begin(), finalStates.end(), inserter(otherStates, otherStates.begin()));

	queue<set<int>> newAllStates;
	newAllStates.push(otherStates);
	newAllStates.push(finalStates);

	vector<set<int>> newStates;

	while (!newAllStates.empty())
	{
		set<int> stateSet = newAllStates.front();
		newAllStates.pop();
		if (stateSet.size() == 1) {
			newStates.push_back(stateSet);
			continue;
		}
		set<int> A, B;
		bool seperatable = false;
		for (int i = 0; i < 128; i++)
		{
			if (seperate(stateSet, char(i), A, B))
			{
				newAllStates.push(A);
				newAllStates.push(B);
				seperatable = true;
				break;
			}
		}
		if (!seperatable) {
			newStates.push_back(stateSet);
		}
	}

	DFA newDFA;
	
	for (int i = 0; i < newStates.size(); i++)
	{
		set<int> X = newStates[i];
		set<int>::iterator it = X.find(startState);
		if (it != X.end()) newDFA.setStartState(i);
		newDFA.addState(translate());
	}
	
	for (int i = 0; i < newStates.size(); i++)
	{
		for (int x : newStates[i])
		{
			for (int j = 0; j < 128; j++)
			{
				int nextState = states[x].table[j];
				if (nextState == -1) continue;
				//int newCurrentState = findOwner(newStates, x);
				int newNextState = findOwner(newStates, nextState); 
				bool isAcceptState = false;
				for (int state_i = 0; state_i < acceptStates.size(); state_i++)
				{
					if (acceptStates[state_i] == x) {
						isAcceptState = true;
						break;
					}
				}
				if (isAcceptState) newDFA.addAcceptState(i);
				newDFA.states[i].table[j] = newNextState;
			}
		}
	}
	
	*this = newDFA;
}


void DFA::minized()
{
	removeUnreachableStates();
	mergeNondistinguishableStates();
}


bool DFA::seperate(set<int> input, char x, set<int>& A, set<int>& B)
{
	A.clear();
	B.clear();
	bool seperatable = false;;
	for (auto state : input)
	{
		int nextState = states[state].table[x];
		if (nextState == -1) continue;

		set<int>::iterator it = input.find(nextState);
		if (it == input.end())
		{
			B.insert(state);
			seperatable = true;
		}
	}

	set_difference(input.begin(), input.end(), B.begin(), B.end(), inserter(A, A.begin()));
	if (A.size() == 0)  seperatable = false;
	return seperatable;
}


//set<int> DFA::translateToOneState(char c, set<int> theStates)
//{
//	set<int> reachableStates;
//	set<int> result;
//	for (auto x : theStates)
//	{
//		int nextStates = states[x].table[c];
//		if (nextStates != -1) {
//			reachableStates.insert(nextStates);
//			result.insert(x);
//		}
//	}
//
//	if (reachableStates.size() == 0) return set<int>();
//	set<int>::iterator it = theStates.find(*reachableStates.begin());
//	if (reachableStates.size() == 1 && it != theStates.end())
//	{
//		return result;
//	}
//	return set<int>();
//}


int DFA::findOwner(vector<set<int>> P, int x)
{
	for (int i = 0; i < P.size(); i++)
	{
		if (P[i].find(x) != P[i].end()) return i;
	}
}
