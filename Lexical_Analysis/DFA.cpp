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

	if (unreachableStates.size() == 0) return;
	for (auto x : unreachableStates)
	{
		states.erase(x);
		index--;
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

	set<set<int>> P;
	P.insert(finalStates);
	P.insert(otherStates);
	set<set<int>> W;
	W.insert(finalStates);

	while (!W.empty())
	{
		set<set<int>>::iterator it = W.begin();
		set<int> A = *it;
		W.erase(*it);

		//if (A.size() == 1) continue;

		for (int i = 0; i < 128; i++)
		{
			set<int> X = translate_to_a_set(A, char(i));
			if (X.size() == 0) continue;

			set<int> prev_set;
			bool first = true;
			for (set<int> Y : P)
			{
				if (!first)
				{
					set<set<int>>::iterator it = P.find(prev_set);
					if (it != P.end()) {
						P.erase(it);
					}
				}
				set<int> intersection, difference;
				set_intersection(X.begin(), X.end(), Y.begin(), Y.end(), inserter(intersection, intersection.begin()));
				set_difference(Y.begin(), Y.end(), X.begin(), X.end(), inserter(difference, difference.begin()));

				if (intersection.size() == 0 || difference.size() == 0) continue;

				prev_set = Y;
				if (first) {
					first = false;
				}
				P.insert(intersection);
				P.insert(difference);

				set<set<int>>::iterator wit = W.find(Y);
				if (wit != W.end())
				{
					W.insert(intersection);
					W.insert(difference);
				}
				else
				{
					if (difference.size() < intersection.size())
					{
						W.insert(difference);
					}
					else
					{
						W.insert(intersection);
					}
				}
			}
		}
	}

	vector<set<int>> newStates;
	for (set<int> x : P)
	{
		newStates.push_back(x);
	}

	*this = create_newDFA(newStates);
}


DFA DFA::create_newDFA(const vector<set<int>>& newStates)
{
	DFA newDFA;

	for (int i = 0; i < newStates.size(); i++)
	{
		set<int> X = newStates[i];
		set<int>::iterator it = X.find(startState);
		if (it != X.end()) newDFA.setStartState(i);
		newDFA.addState(translate());
	}

	set<int> newAcceptStates;
	for (int i = 0; i < newStates.size(); i++)
	{
		for (int x : newStates[i])
		{
			for (int j = 0; j < 128; j++)
			{
				bool isAcceptState = false;
				for (int state_i = 0; state_i < acceptStates.size(); state_i++)
				{
					if (acceptStates[state_i] == x) {
						isAcceptState = true;
						break;
					}
				}
				int newCurrentState = findOwner(newStates, x);

				if (isAcceptState) {
					//newDFA.addAcceptState(newCurrentState);
					newAcceptStates.insert(newCurrentState);
				}

				int nextState = states[x].table[j];
				if (nextState == -1) continue;
				int newNextState = findOwner(newStates, nextState);
				newDFA.states[newCurrentState].table[j] = newNextState;
			}
		}
	}
	for (auto x : newAcceptStates)
	{
		newDFA.addAcceptState(x);
	}

	return newDFA;
}

set<int> DFA::translate_to_a_set(const set<int>& A, char c)
{
	set<int> result;
	for (int i = 0; i < states.size(); i++)
	{
		int nextState = states[i].table[int(c)];
		if (A.find(nextState) != A.end())
		{
			result.insert(i);
		}
	}
	return result;
}


void DFA::minized()
{
	mergeNondistinguishableStates();
	removeUnreachableStates();
}


void DFA::generate_DOT(const char *name)
{
	ofstream outFile(name);
	string shapeCircle = "[shape=circle]";
	string shapeDoubleCircle = "[shape=doublecircle]";

	outFile << "digraph graphname {\n";
	outFile << "\trankdir=LR;\n";

	// define shape
	outFile << "\t\"\" [shape=none]\n";
	set<int> finalStates;
	set<int> otherStates;
	set<int> allStates;
	for (int i = 0; i < acceptStates.size(); i++)
	{
		finalStates.insert(acceptStates[i]);
	}
	for (int i = 0; i < states.size(); i++)
	{
		map<int, translate>::iterator it = states.find(i);
		if(it != states.end())
			allStates.insert(i);
	}
	set_difference(allStates.begin(), allStates.end(), finalStates.begin(), finalStates.end(), inserter(otherStates, otherStates.begin()));
	for (auto x : finalStates)
	{
		outFile << "\t" << x << " " << shapeDoubleCircle << endl;
	}
	for (auto x : otherStates)
	{
		outFile << "\t" << x << " " << shapeCircle << endl;
	}

	// define translate
	// sample a->b[label = "0"]
	string labelStart = "[label =\"";
	string labelEnd = "\"]";
	string arrow = "->";

	outFile << "\t" << "\"\"" << arrow << startState << endl;

	for (auto x : states)
	{
		for (int i = 0; i < 128; i++)
		{
			int nextState = x.second.table[i];
			if (nextState != -1)
			{
				outFile << "\t" << x.first << arrow << nextState << " " << labelStart << char(i) << labelEnd << endl;
			}
		}
	}


	outFile << "}";
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
