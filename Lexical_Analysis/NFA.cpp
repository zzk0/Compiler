#include "NFA.h"



NFA::NFA()
{
}


NFA::NFA(int stateCount)
{
	G = new vector<pair<int, char>>(stateCount);
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


DFA NFA::convertToDFA()
{

}