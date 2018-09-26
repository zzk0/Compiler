/*
使用邻接链表来存储NFA的信息，不模拟运行，仅仅存储信息即可。
vector<pair<int, char>> G[]
*/

#pragma once
#include <iostream>
#include <vector>
#include "DFA.h"
using namespace std;
class NFA
{
public:
	NFA();
	NFA(int stateCount);
	~NFA();

	int startState;
	vector<int> acceptStates;
	vector<pair<int, char>> *G;

	void setStartState(int x);
	void addAcceptState(int state);
	void addEdge(int state1, int state2, char c);

	DFA convertToDFA();
};

