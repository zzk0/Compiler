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
		if (G[state][i].second == '\0' && G[state][i].first != state) {
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
			if (count(reachableStates.begin(), reachableStates.end(), G[state][i].first)) {
				continue;
			}
			else {
				reachableStates.push_back(G[state][i].first);
				epsilonClosure(G[state][i].first, reachableStates);
			}
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
	reachableStates.push_back(startState);
	epsilonClosure(startState, reachableStates);

	M[0] = vector<int>();
	M[1] = reachableStates;

	DFA dfa;
	dfa.addState(translate()); // state 0
	dfa.addState(translate()); // state 1
	dfa.setStartState(1);
	bool flag = false;
	for (int i = 0; i < reachableStates.size(); i++) {
		for (int j = 0; j < acceptStates.size(); j++) {
			if (reachableStates[i] == acceptStates[j]) {
				dfa.addAcceptState(1);
				flag = true;
				break;
			}
		}
		if (flag) break;
	}
	

	int j = 0, p = 1;
	while (j <= p)
	{
		for (int i = 1; i < 128; i++)
		{
			vector<int> temp;
			alphabetClosure(char(i), M[j], temp);
			bool find = false;
			if (temp.size() == 0)
			{
				find = true;
				dfa.states[0].table[i] = '\0';
			}
			else
			{
				for (int k = 1; k <= p; k++)
				{
					if (hasSameVector(temp, M[k]))
					{
						dfa.states[j].table[i] = k;
						find = true;
						break;
					}
				}
			}
			if(!find)
			{
				p = p + 1;
				for (int ii = 0; ii < temp.size(); ii++)
				{
					for (int jj = 0; jj < acceptStates.size(); jj++)
					{
						if (temp[ii] == acceptStates[jj])
						{
							dfa.addAcceptState(p);
							break;
						}
					}
				}
				M[p] = temp;
				dfa.addState(translate());
				dfa.states[j].table[i] = p;
			}
		}
		j++;
	}

	return dfa;
}


NFA nfa_concatenate(NFA a, NFA b)
{
	// 首先，将b的所有状态的ID加上a的偏移量。
	int bias = a.stateCount;
	for (int i = 0; i < b.stateCount; i++)
	{
		for (int j = 0; j < b.G[i].size(); j++)
		{
			b.G[i][j].first += bias;
		}
	}

	// 将b的图接到a的后面
	NFA c(a.stateCount + b.stateCount);
	for (int i = 0; i < a.stateCount; i++)
	{
		c.G[i].insert(c.G[i].end(), a.G[i].begin(), a.G[i].end());
	}
	for (int i = a.stateCount; i < a.stateCount + b.stateCount; i++)
	{
		c.G[i].insert(c.G[i].end(), b.G[i - a.stateCount].begin(), b.G[i - a.stateCount].end());
	}

	// 新增一个epsilon转移，从a的accept states到b的start states
	for (int i = 0; i < a.acceptStates.size(); i++)
	{
		c.G[a.acceptStates[i]].push_back(pair<int, char>(b.startState + a.stateCount, '\0'));
	}

	// 设置b的accept state 为 c的accept state
	for (int i = 0; i < b.acceptStates.size(); i++)
	{
		c.addAcceptState(b.acceptStates[i] + a.stateCount);
	}

	c.setStartState(a.startState);

	return c;
}


NFA nfa_union(NFA a, NFA b)
{
	// 首先，将b的所有状态的ID加上a的偏移量。
	int bias = a.stateCount;
	for (int i = 0; i < b.stateCount; i++)
	{
		for (int j = 0; j < b.G[i].size(); j++)
		{
			b.G[i][j].first += bias;
		}
	}

	// 将b的图接到a的后面
	NFA c(a.stateCount + b.stateCount + 1);
	for (int i = 0; i < a.stateCount; i++)
	{
		c.G[i].insert(c.G[i].end(), a.G[i].begin(), a.G[i].end());
	}
	for (int i = a.stateCount; i < a.stateCount + b.stateCount; i++)
	{
		c.G[i].insert(c.G[i].end(), b.G[i - a.stateCount].begin(), b.G[i - a.stateCount].end());
	}

	// 新增加一个start state, 这个start state ε 转移到 那两个的start state, 从0开始计数，所以最后一个状态是两个count的和。
	c.setStartState(a.stateCount + b.stateCount);
	c.addEdge(a.stateCount + b.stateCount, a.startState, '\0');
	c.addEdge(a.stateCount + b.stateCount, b.startState + a.stateCount, '\0');

	// c的accept state
	for (int i = 0; i < a.acceptStates.size(); i++)
	{
		c.addAcceptState(a.acceptStates[i]);
	}
	for (int i = 0; i < b.acceptStates.size(); i++)
	{
		c.addAcceptState(b.acceptStates[i] + a.stateCount);
	}

	return c;
}


/*
ITOC P62
*/
NFA nfa_star(NFA a)
{
	NFA c(a.stateCount + 1);
	for (int i = 0; i < a.stateCount; i++)
	{
		c.G[i].insert(c.G[i].end(), a.G[i].begin(), a.G[i].end());
	}
	c.setStartState(a.stateCount);
	c.addAcceptState(a.stateCount);
	c.addEdge(a.stateCount, a.startState, '\0');

	for (int i = 0; i < a.acceptStates.size(); i++)
	{
		c.addEdge(a.acceptStates[i], a.startState, '\0');
		c.addAcceptState(a.acceptStates[i]);
	}
	
	return c;
}
