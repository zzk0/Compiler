#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

class ParsingRule
{
public:
	ParsingRule(string startStr);
	~ParsingRule();

	string startStr;
	vector<string> rule;
	int ruleLength;
	int *isNonTerminal;

	void addToRule(string entry);

	void processTheEntry(const set<string> &nonTerminals);

	string operator[](int i) const {
		if (i > rule.size()) {
			string str = "";
			return str;
		}
		else {
			return rule[i];
		}
	}

	void replaceFirst(const ParsingRule & subRule);

	vector<string> getRule();
	vector<string> getAfterFirst();
	vector<string> getAfterTheIndex(int index);

	void addWholeRule(ParsingRule *parsingRule);

	bool isANonTerminal(int index);
};

vector<ParsingRule*> parseFromFile(const char *filename);

map<string, vector<ParsingRule*>> makeGroup(const vector<ParsingRule*> &rules, set<string> nonTerminals);

void removeLeftRecursive(map<string, vector<ParsingRule*>>& rules);

void removeLeftCommon(map<string, vector<ParsingRule*>>& rules);

void removeDirectLeftCommon(map<string, vector<ParsingRule*>> &rules, vector<ParsingRule*> temp);

string hasCommonFactor(const vector<ParsingRule*> &rules);

void printTheGramma(const map<string, vector<ParsingRule*>> &rules);

bool hasLeftRecursive(const vector<ParsingRule*> &rules);

vector<ParsingRule*> removeDirectLeftRecursive(vector<ParsingRule*> &rules);

void printTheGramma1(const vector<vector<ParsingRule*>> &rules);

