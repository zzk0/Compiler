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
using namespace std;

struct translate {
	int table[256];
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
	vector<translate> states;
	int startState;
	int acceptState;

	void setStartState(int x);
	void setAcceptState(int x);

	/*
	添加一个状态
	初始化state：state[int(x)] = 某一个状态的ID
	*/
	void addState(translate state);

	/*
	输入一个字符串str,长度length，然后返回这个DFA是否接受这个串
	*/
	bool IsAccepted(const char *str, int length);
	bool IsAccepted(string str, int length);
};


class exDFA
{
public:
	exDFA();
	~exDFA();

	vector<translate> states;
	vector<int> acceptStates;
	int startState;
	int lastAcceptState;
	int currentState;

	void resetToStart();
	void setStartState(int x);
	void addAcceptState(int x);
	void addState(translate state);
	int getLastAcceptState();

	void runOneStep(char x, bool &accepted, int &currentStateID);

	void convertFromDFAs(vector<DFA> DFAs);

	// bool IsAccepted(const char *str, int length);
	// bool IsAccepted(string str, int length);
};
 