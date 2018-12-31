#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <stack>
#include "ParsingRule.h"
using namespace std;

class LLTable
{
public:
	LLTable();
	~LLTable();

	// Data
	map<string, vector<ParsingRule*>> parsingRules;
	vector<ParsingRule*> allRules;
	map<string, set<string>> theFirstSet;
	map<string, set<string>> theFollowSet;
	map<string, map<string, ParsingRule*>> table;
	set<string> nonTerminals;
	string startSymbol;
	// Set and Get Function
	void setParsingRules(const map<string, vector<ParsingRule*>> &parsingRules);
	void setNonTerminals(const set<string> &nonTerminals);
	void setStartSymbol(string startSymbol);
	// Utility Function
	void computeTheFirstSet();
	void computeTheFollowSet();
	void computeTable();
	void printTable(string type);
	void parseString(string str);

private:
	set<string> first(string symbol);
	bool isTerminal(string symbol);
	set<string> firstOfSymbols(vector<string> symbols);
	void transformToAllRules();
};