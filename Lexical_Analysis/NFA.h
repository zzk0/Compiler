/*
使用邻接链表来存储NFA的信息，不模拟运行，仅仅存储信息即可。
vector<pair<int, char>> G[]
*/

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "DFA.h"
using namespace std;
class NFA
{
public:
	NFA();
	NFA(int stateCount);
	~NFA();

	int stateCount;
	int startState;
	vector<int> acceptStates;
	vector<pair<int, char>> *G;

	void setStartState(int x);
	void addAcceptState(int state);
	void addEdge(int state1, int state2, char c);

	/*
	epsilon Closure
	*/
	void epsilonClosure(int state, vector<int> &reachableStates);
	void epsilonClosure(vector<int> states, vector<int> &reachableStates);

	/*
	input a character, return reachable states
	*/
	void alphabetClosure(char x, int state, vector<int> &reachableStates);
	void alphabetClosure(char x, vector<int> states, vector<int> &reachableStates);

	DFA convertToDFA();

private:
	int convertToValue(vector<int> &states);
	bool hasSameVector(vector<int> a, vector<int> b);
};

