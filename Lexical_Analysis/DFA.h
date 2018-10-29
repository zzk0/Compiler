/*
使用方法：
初始化好translate，然后按照顺序压入到DFA中。DFA要设置开始和接受。
输入一个string和string的长度，输出是否接受。
translate a0, a1, a2;
for (int i = 0; i < 256; i++) {
a0.table[i] = -1;
a1.table[i] = -1;
a2.table[i] = -1;
}

a0.table[int('a')] = 1;
a0.table[int('c')] = 2;
a1.table[int('a')] = 2;
char x = 'b';
do {
a1.table[int(x)] = 1;
x = x + 1;
} while (x != 'z');
a2.table[int('c')] = 2;

DFA dfa;
dfa.setAcceptState(2);
dfa.setStartState(0);
dfa.addState(a0);
dfa.addState(a1);
dfa.addState(a2);

string str;
while (cin >> str) {
bool x = dfa.IsAccepted(str, str.length());
cout << str << (x ? " accept" : " reject") << endl;
}*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <queue>
using namespace std;

struct translate {
	int table[128];

	translate()
	{
		for (int i = 0; i < 128; i++)
		{
			table[i] = -1;
		}
	}
};

class DFA
{
public:
	DFA();
	~DFA();

	/*
	假设输入的char是x，translate[int(x)]得到的是转移过去的状态。如果为-1，表示不存在这样的转移函数。
	如果需要添加一个新的状态，那么只需要初始化好translate之后，向states push一个translate。
	*/
	map<int, string> stateIDs;
	//vector<translate> states;
	map<int, translate> states;
	int index = 0;

	int startState;
	vector<int> acceptStates;

	void setStartState(int x);
	void addAcceptState(int x);

	/*
	添加一个状态
	初始化state：state[int(x)] = 某一个状态的ID
	*/
	void addState(translate state);
	void addState(translate state, string stateID);
	void addStateID(int x, string y);

	/*
	通过stateID来查找对应vector<translate>中的位置
	返回-1表示没有找到。
	*/
	int statesPos(string stateID);

	/*
	输入一个字符串str,长度length，然后返回这个DFA是否接受这个串
	*/
	bool IsAccepted(const char *str, int length);
	bool IsAccepted(string str, int length);

	/*
	输入一个字符，一次转移
	需要保存的状态：当前的state，上一次accept的state
	*/
	void runOneStep(char x, bool &accepted, int &currentStateID);
	void reset();
	int getLastAcceptState();

	void removeUnreachableStates();

	void mergeNondistinguishableStates();

	void minized();

private:
	int lastAcceptState;
	int currentState;

	bool seperate(set<int> input, char x, set<int> &A, set<int> &B);
	//set<int> translateToOneState(char c, set<int> theStates);
	int findOwner(vector<set<int>> P, int x);
};
