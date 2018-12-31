#include "ParsingRule.h"
#include <iostream>
#include <fstream>

ParsingRule::ParsingRule(string startStr) : startStr(startStr), ruleLength(0)
{
}

ParsingRule::~ParsingRule()
{
}

void ParsingRule::addToRule(string entry)
{
	if (ruleLength != 0 && entry == "epsilon") return;
	if (ruleLength != 0 && rule[0] == "epsilon")
	{
		rule[0] = entry;
		return;
	}
	ruleLength++;
	rule.push_back(entry);
}

void ParsingRule::processTheEntry(const set<string>& nonTerminals)
{
	isNonTerminal = new int[rule.size()];
	memset(isNonTerminal, 0, rule.size());
	for (int i = 0; i < rule.size(); i++)
	{
		for (auto j : nonTerminals)
		{
			if (j == rule[i])
			{
				isNonTerminal[i] = 1;
			}
		}
	}
}

void ParsingRule::replaceFirst(const ParsingRule & subRule)
{
	rule.erase(rule.begin());
	for (int i = subRule.ruleLength - 1; i >= 0; i--)
	{
		rule.insert(rule.begin(), subRule[i]);
	}
}

vector<string> ParsingRule::getRule()
{
	return rule;
}

vector<string> ParsingRule::getAfterFirst()
{
	vector<string> afterFirst;
	if (ruleLength == 1)
	{
		afterFirst.push_back("epsilon");
	}
	else
	{
		for (int i = 1; i < ruleLength; i++)
		{
			afterFirst.push_back(rule[i]);
		}
	}
	return afterFirst;
}

vector<string> ParsingRule::getAfterTheIndex(int index)
{
	vector<string> afterIndex;
	if (index + 1 == rule.size())
	{
		afterIndex.push_back("epsilon");
	}
	for (int i = index + 1; i < rule.size(); i++)
	{
		afterIndex.push_back(rule[i]);
	}
	return afterIndex;
}

void ParsingRule::addWholeRule(ParsingRule * parsingRule)
{
	for (int i = 0; i < parsingRule->ruleLength; i++)
	{
		addToRule((*parsingRule)[i]);
	}
}

bool ParsingRule::isANonTerminal(int index)
{
	if (0 == isNonTerminal[index])
	{
		return false;
	}
	else
	{
		return true;
	}
}

vector<ParsingRule*> parseFromFile(const char * filename)
{
	ifstream infile;
	infile.open(filename);
	if (!infile) {
		cerr << "open file error" << endl;
	}
	vector<ParsingRule*> rules;

	string startStr;
	while (infile >> startStr)
	{
		string buffer;
		infile >> buffer;
		infile >> buffer;

		ParsingRule *parsingRule = new ParsingRule(startStr);
		while (buffer != "$")
		{
			if (buffer == "|")
			{
				rules.push_back(parsingRule);
				parsingRule = new ParsingRule(startStr);
			}
			else {
				parsingRule->addToRule(buffer);
			}
			infile >> buffer;
		}
		rules.push_back(parsingRule);
	}
	return rules;
}

map<string, vector<ParsingRule*>> makeGroup(const vector<ParsingRule*>& rules, set<string> nonTerminals)
{
	// 1, Initialize the map ( vector )
	map<string, vector<ParsingRule*>> allRules;
	for (auto i : nonTerminals)
	{
		allRules[i] = vector<ParsingRule*>();
	}
	for (int i = 0; i < rules.size(); i++)
	{
		allRules[rules[i]->startStr].push_back(rules[i]);
	}
	return allRules;
}

void removeLeftRecursive(map<string, vector<ParsingRule*>>& rules)
{
	// 1, 消除直接左递归
	for (auto aSetOfRules : rules)
	{
		vector<ParsingRule*> temp = aSetOfRules.second;
		if (hasLeftRecursive(temp))
		{
			vector<ParsingRule*> newRule = removeDirectLeftRecursive(temp);
			rules[newRule[0]->startStr] = newRule;
			rules[temp[0]->startStr] = temp;
		}
	}

	// 2, 消除间接左递归
	vector<vector<ParsingRule*>> orderedRules;
	for (auto aSetOfRules : rules)
	{
		orderedRules.push_back(aSetOfRules.second);
	}

	for (int i = 0; i < orderedRules.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			string startStr = orderedRules[j][0]->startStr;
			for(int k = 0; k < orderedRules[i].size(); k++)
			{
				if ((*orderedRules[i][k])[0] == startStr)
				{
					// 改写
					vector<ParsingRule*> newRules;
					for (auto ruleInSecond : orderedRules[j])
					{
						ParsingRule *newRule = new ParsingRule((orderedRules[i][k])->startStr);
						newRule->addWholeRule(ruleInSecond);
						vector<string> alpha = (orderedRules[i][k])->getAfterFirst();
						for (string x : alpha)
						{
							newRule->addToRule(x);
						}
						newRules.push_back(newRule);
					}
					// 加入到规则中
					for (int x = 0; x < newRules.size(); x++)
					{
						orderedRules[i].push_back(newRules[x]);
					}
					// 消除直接左递归
					if (hasLeftRecursive(orderedRules[i]))
					{
						vector<ParsingRule*> newNewRules = removeDirectLeftRecursive(orderedRules[i]);
						orderedRules.push_back(newNewRules);
						delete orderedRules[i][k];
						orderedRules[i][k] = nullptr;
						orderedRules[i].erase(orderedRules[i].begin() + k);
						k--;
					}
					else
					{
						// 如果没有左递归，那么代入要回到原来的状态
						for (int x = 0; x < newRules.size(); x++)
						{
							orderedRules[i].pop_back();
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < orderedRules.size(); i++)
	{
		rules[orderedRules[i][0]->startStr] = orderedRules[i];
	}
}

void removeLeftCommon(map<string, vector<ParsingRule*>>& rules)
{
	// 1，直接左公共因子
	// A -> aB | aC	| a 	=>	 A -> aA', A' -> B | C | epsilon
	for (auto aSetOfRules : rules)
	{
		vector<ParsingRule*> temp = aSetOfRules.second;
		removeDirectLeftCommon(rules, temp);
	}

	// 2，间接左公共因子，代入，消除直接左公共因子，方法不详
	// A -> B | C, B -> aB | x， C -> aC		=>	 A -> aA' | B | C，A' -> B | C, B -> x, C -> epsilon
}

void removeDirectLeftCommon(map<string, vector<ParsingRule*>>& rules, vector<ParsingRule*> temp)
{
	int replaceTime = 0;
	string commonFactor;
	while ((commonFactor = hasCommonFactor(temp)) != "")
	{
		string newGrammaName = temp[0]->startStr + to_string(replaceTime++);
		string oldGrammaName = temp[0]->startStr;
		vector<ParsingRule*> newGramma;
		vector<ParsingRule*>::iterator it = temp.begin();
		while (it != temp.end())
		{
			if ((**it)[0] == commonFactor)
			{
				ParsingRule *newRule = new ParsingRule(newGrammaName);
				vector<string> ruleContent = (*it)->getAfterFirst();
				for (auto ruleEntry : ruleContent)
				{
					newRule->addToRule(ruleEntry);
				}
				newGramma.push_back(newRule);
				it = temp.erase(it);
			}
			else
			{
				++it;
			}
		}
		ParsingRule *newRule = new ParsingRule(oldGrammaName);
		newRule->addToRule(commonFactor);
		newRule->addToRule(newGrammaName);
		rules[newGrammaName] = newGramma;
		temp.push_back(newRule);
		rules[oldGrammaName] = temp;
	}
}

string hasCommonFactor(const vector<ParsingRule*>& rules)
{
	for (int i = 0; i < rules.size(); i++)
	{
		for (int j = i + 1; j < rules.size(); j++)
		{
			if ((*rules[i])[0] == (*rules[j])[0])
			{
				return (*rules[i])[0];
			}
		}
	}
	return "";
}

void printTheGramma(const map<string, vector<ParsingRule*>>& rules)
{
	for (auto aSetOfRule : rules)
	{
		cout << aSetOfRule.first << " -> ";
		for (auto i : aSetOfRule.second)
		{
			for (int j = 0; j < i->ruleLength; j++)
			{
				cout << (*i)[j] << " ";
			}
			auto it = aSetOfRule.second[aSetOfRule.second.size() - 1];
			if (i != it)
			{
				cout << "| ";
			}
		}
		cout << "$" << endl;
	}
}

bool hasLeftRecursive(const vector<ParsingRule*>& rules)
{
	string startStr = rules[0]->startStr;
	for (int i = 0; i < rules.size(); i++) 
	{
		if (startStr == (*rules[i])[0])
		{
			return true;
		}
	}
	return false;
}

vector<ParsingRule*> removeDirectLeftRecursive(vector<ParsingRule*> &rules)
{
	string startStr = rules[0]->startStr;
	vector<ParsingRule*> newRule;
	vector<ParsingRule*>::iterator it = rules.begin();
	while (it != rules.end())
	{
		if (startStr == (**it)[0])
		{
			vector<string> afterFirst = (*it)->getAfterFirst();
			ParsingRule *rule = new ParsingRule(startStr + "'");
			for (int i = 0; i < afterFirst.size(); i++)
			{
				rule->addToRule(afterFirst[i]);
			}
			rule->addToRule(startStr + "'");
			newRule.push_back(rule);
			// clean
			delete (*it);
			(*it) = nullptr;
			it = rules.erase(it);
		}
		else
		{
			(*it)->addToRule(startStr + "'");
			++it;
		}
	}
	if (rules.size() == 0)
	{
		ParsingRule *rule = new ParsingRule(startStr);
		rule->addToRule(startStr + "'");
		rules.push_back(rule);
	}
	ParsingRule *rule = new ParsingRule(startStr + "'");
	rule->addToRule("epsilon");
	newRule.push_back(rule);
	return newRule;
}

void printTheGramma1(const vector<vector<ParsingRule*>>& rules)
{
	for (int i = 0; i < rules.size(); i++)
	{
		cout << rules[i][0]->startStr << " -> ";
		for (int j = 0; j < rules[i].size(); j++)
		{
			for (int k = 0; k < rules[i][j]->ruleLength; k++)
			{
				cout << (*rules[i][j])[k] << " ";
			}
			cout << " | ";
		}
		cout << endl;
	}
}
