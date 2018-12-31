#include "LLTable.h"



LLTable::LLTable()
{
}


LLTable::~LLTable()
{
}

void LLTable::setParsingRules(const map<string, vector<ParsingRule*>>& parsingRules)
{
	this->parsingRules = parsingRules;
}

void LLTable::setNonTerminals(const set<string>& nonTerminals)
{
	this->nonTerminals = nonTerminals;
}

void LLTable::setStartSymbol(string startSymbol)
{
	this->startSymbol = startSymbol;
}

set<string> LLTable::first(string symbol)
{
	// If exists in the first set, then return, just like Dynamic Programming method.
	if (theFirstSet[symbol].size() != 0)
	{
		return theFirstSet[symbol];
	}
	// If symbol is a terminal, then store it in the first set, then return the set.
	if (isTerminal(symbol))
	{
		theFirstSet[symbol].insert(symbol);
		return theFirstSet[symbol];
	}

	set<string> firstSet;
	vector<ParsingRule*> rules = parsingRules[symbol];
	for (int i = 0; i < rules.size(); i++)
	{
		int k = 0;
		while (k < rules[i]->ruleLength)
		{
			if ((*rules[i])[k] == symbol)
			{
				// confuse here
				// A -> B A C | c, B -> a | epsilon, C -> x | epsilon
				// The Problem can be start from left recursive
				if (k == rules[i]->ruleLength - 1) k++;
				break;
			}
			set<string> firstOfFirstSymbol = first((*rules[i])[k]);
			firstSet.insert(firstOfFirstSymbol.begin(), firstOfFirstSymbol.end());
			auto it = firstSet.find("epsilon");
			if (it != firstSet.end())
			{
				firstSet.erase(it);
			}
			if (firstOfFirstSymbol.find("epsilon") == firstOfFirstSymbol.end()) break;
			k++;
		}
		if (k == rules[i]->ruleLength) firstSet.insert("epsilon");
	}
	theFirstSet[symbol] = firstSet;
	return firstSet;
}

bool LLTable::isTerminal(string symbol)
{
	for (auto str : nonTerminals)
	{
		if (str == symbol)
		{
			return false;
		}
	}
	return true;
}

void LLTable::computeTheFirstSet()
{
	for (auto str : nonTerminals)
	{
		first(str);
	}
}

void LLTable::computeTheFollowSet()
{
	theFollowSet[startSymbol].insert("$");
	bool setHaveChanged = true;
	transformToAllRules();
	while (setHaveChanged)
	{
		setHaveChanged = false;
		for (int i = 0; i < allRules.size(); i++)
		{
			for (int j = 0; j < allRules[i]->ruleLength; j++)
			{
				if (!isTerminal((*allRules[i])[j]))
				{
					vector<string> ruleAfterIndex = allRules[i]->getAfterTheIndex(j);
					set<string> firstSetOfRuleAfterIndex = firstOfSymbols(ruleAfterIndex);
					bool haveEpsilon = false;
					int setSize = theFollowSet[(*allRules[i])[j]].size();
					if (firstSetOfRuleAfterIndex.find("epsilon") != firstSetOfRuleAfterIndex.end())
					{
						haveEpsilon = true;
						firstSetOfRuleAfterIndex.erase(firstSetOfRuleAfterIndex.find("epsilon"));
					}
					theFollowSet[(*allRules[i])[j]].insert(firstSetOfRuleAfterIndex.begin(), firstSetOfRuleAfterIndex.end());
					if (haveEpsilon)
					{
						theFollowSet[(*allRules[i])[j]].insert(theFollowSet[allRules[i]->startStr].begin(), theFollowSet[allRules[i]->startStr].end());
					}
					if (setSize != theFollowSet[(*allRules[i])[j]].size()) setHaveChanged = true;
				}
			}
		}
	}
}

void LLTable::computeTable()
{
	computeTheFirstSet();
	computeTheFollowSet();
	for (int i = 0; i < allRules.size(); i++)
	{
		set<string> firstOfFirstSymbol = firstOfSymbols(allRules[i]->getRule());
		for (auto x : firstOfFirstSymbol)
		{
			if (x == "epsilon") continue;
			table[allRules[i]->startStr][x] = allRules[i];
		}
		if (firstOfFirstSymbol.find("epsilon") != firstOfFirstSymbol.end())
		{
			set<string> followOfFirstSymbol = theFollowSet[allRules[i]->startStr];
			for (auto x : followOfFirstSymbol)
			{
				if (x == "epsilon") continue;
				table[allRules[i]->startStr][x] = allRules[i];
			}
		}
	}
}

void LLTable::printTable(string type)
{
	map<string, set<string>> selectedSet;
	if (type == "first")
	{
		selectedSet = theFirstSet;
	}
	else if(type == "follow")
	{
		selectedSet = theFollowSet;
	}
	else if (type == "LL(1)")
	{
		for (auto x : table)
		{
			cout << x.first << " : ";
			for (auto y : x.second)
			{
				cout << "(" + y.first + ", " + y.second->startStr + " -> ";
				for (int i = 0; i < y.second->ruleLength; i++)
				{
					cout << (*y.second)[i] << " ";
				}
				cout << ") \t";
			}
			cout << endl;
		}
	}
	else
	{
		cout << "Unknown type" << endl;
		return;
	}

	for (auto x : selectedSet)
	{
		cout << x.first << ": ";
		for (auto y : x.second)
		{
			cout << y << ", ";
		}
		cout << endl;
	}
}

void LLTable::parseString(string str)
{
	vector<string> parseStack;
	parseStack.push_back(startSymbol);
	int i = 0, length = str.length();
	while (i < length && !parseStack.empty())
	{
		cout << "$ ";
		for (auto str : parseStack)
		{
			cout << str << " ";
		}
		cout << endl;
		for (int j = i; j < length; j++)
		{
			cout << str[j] << " ";
		}
		cout << "$" << endl;
		
		if (!isTerminal(parseStack.back()))
		{
			string x = parseStack.back();
			parseStack.pop_back();
			ParsingRule *rule = table[x][string(1, str[i])];
			if (rule == nullptr)
			{
				cout << "Parse Error" << endl;
				return;
			}
			for (int j = rule->ruleLength - 1; j >= 0; j--)
			{
				parseStack.push_back((*rule)[j]);
			}
		}
		else
		{
			if (parseStack.back() == "epsilon")
			{
				parseStack.pop_back();
			}
			else if (parseStack.back() == string(1, str[i]))
			{
				i++;
				parseStack.pop_back();
			}
			else if (parseStack.back() == "number")
			{
				if (str[i] >= '0' && str[i] <= '9')
				{
					i++;
					parseStack.pop_back();
				}
				else
				{
					cout << "Parse Error" << endl;
					return;
				}
			}
			else
			{
				cout << "Parse Error" << endl;
				return;
			}
		}
	}
	if (i != length || !parseStack.empty())
	{
		cout << "Parse Error" << endl;
	}
	else
	{
		cout << "Parse Complete" << endl;
	}
}

set<string> LLTable::firstOfSymbols(vector<string> symbols)
{
	set<string> result;
	int i;
	for (i = 0; i < symbols.size(); i++)
	{
		set<string> firstOfSymbol = first(symbols[i]);
		result.insert(firstOfSymbol.begin(), firstOfSymbol.end());
		if (firstOfSymbol.find("epsilon") == firstOfSymbol.end())
		{
			break;
		}
	}
	if (i != symbols.size() && result.find("epsilon") != result.end())
	{
		result.erase(result.find("epsilon"));
	}
	return result;
}

void LLTable::transformToAllRules()
{
	for (auto aSetOfRules : parsingRules)
	{
		for (int i = 0; i < aSetOfRules.second.size(); i++)
		{
			allRules.push_back(aSetOfRules.second[i]);
		}
	}
}