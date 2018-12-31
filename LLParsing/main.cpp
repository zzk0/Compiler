#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include "ParsingRule.h"
#include "LLTable.h"
using namespace std;

int main() 
{
	vector<ParsingRule*> rules = parseFromFile("a.txt");
	string temp = (*rules[0])[0];
	set<string> nonTerminals;
	for (int i = 0; i < rules.size(); i++) {
		nonTerminals.insert(rules[i]->startStr);
	}

	auto allRules = makeGroup(rules, nonTerminals);

	for (int i = 0; i < rules.size(); i++) {
		rules[i]->processTheEntry(nonTerminals);
	}

	printTheGramma(allRules);
	cout << "--------------------" << endl;

	removeLeftCommon(allRules);
	printTheGramma(allRules); 
	cout << "--------------------" << endl;

	removeLeftRecursive(allRules);
	printTheGramma(allRules);
	cout << "--------------------" << endl;

	for (auto rule : allRules)
	{
		nonTerminals.insert(rule.first);
	}

	LLTable *table = new LLTable;
	table->setParsingRules(allRules);
	table->setNonTerminals(nonTerminals);
	table->setStartSymbol("S");
	table->computeTheFirstSet();
	table->computeTheFollowSet();
	table->computeTable();
	table->printTable("first");
	cout << "--------------------" << endl;
	table->printTable("follow");
	cout << "--------------------" << endl;
	table->printTable("LL(1)");
	cout << "--------------------" << endl;
	table->parseString("aabe");
}