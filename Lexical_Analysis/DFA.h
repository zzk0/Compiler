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

