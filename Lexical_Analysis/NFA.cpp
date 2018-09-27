#include "NFA.h"


NFA::NFA()
{
}


NFA::NFA(int stateCount)
{
	G = new vector<pair<int, char>>[stateCount];
	this->stateCount = stateCount;
}


NFA::~NFA()
{
}


void NFA::setStartState(int x)
{
	startState = x;
}


void NFA::addAcceptState(int state)
{
	acceptStates.push_back(state);
}


void NFA::addEdge(int state1, int state2, char c)
{
	G[state1].push_back(pair<int, char>(state2, c));
}


void NFA::epsilonClosure(int state, vector<int> &reachableStates)
{
	for (int i = 0; i < G[state].size(); i++)
	{
		if (G[state][i].second == '\0') {
			reachableStates.push_back(G[state][i].first);
			epsilonClosure(G[state][i].first, reachableStates);
		}
	}
}


void NFA::epsilonClosure(vector<int> states, vector<int> &reachableStates)
{
	for (int i = 0; i < states.size(); i++) 
	{
		epsilonClosure(states[i], reachableStates);
	}
}


void NFA::alphabetClosure(char x, int state, vector<int> &reachableStates)
{
	for (int i = 0; i < G[state].size(); i++)
	{
		if (G[state][i].second == x) {
			reachableStates.push_back(G[state][i].first);
			epsilonClosure(G[state][i].first, reachableStates);
		}
	}
}


void NFA::alphabetClosure(char x, vector<int> states, vector<int> &reachableStates)
{
	for (int i = 0; i < states.size(); i++)
	{
		alphabetClosure(x, states[i], reachableStates);
	}
}


bool NFA::hasSameVector(vector<int> a, vector<int> b)
{
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	if (a.size() != b.size()) return false;
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}


DFA NFA::convertToDFA()
{
	map<int, vector<int>> M;
	vector<int> reachableStates;
	epsilonClosure(startState, reachableStates);

	M[0] = vector<int>();
	M[1] = reachableStates;

	DFA dfa;
	dfa.addState(translate()); // state 0
	dfa.addState(translate()); // state 1
	dfa.setStartState(1);

	int j = 0, p = 1;
	while (j <= p)
	{
		for (int i = 1; i < 128; i++)
		{
			reachableStates.clear();
			alphabetClosure(char(i), M[j], reachableStates);
			for (int k = 0; k < p; k++)
			{
				if (hasSameVector(reachableStates, M[k]))
				{
					dfa.states[j].table[char(i)] = k;
				}
				else
				{
					p = p + 1;
					dfa.addState(translate());
					dfa.states[j].table[char(i)] = p;
				}
			}
		}
		j++;
	}

	return DFA();
}


/*
example:
If state 0 is reachable, then sum += 1;
If state 1 is reachable, then sum += 2;

But only 32 states is allowed
*/
int NFA::convertToValue(vector<int> &states)
{
	int sum = 0;
	for (int i = 0; i < states.size(); i++)
	{
		sum += (1 << states[i]);
	}
	return sum;
}