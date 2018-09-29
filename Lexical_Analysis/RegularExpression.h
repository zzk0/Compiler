#pragma once

#include "NFA.h"
#include <iostream>
#include <string>
using namespace std;

enum State
{
	Concatenate,
	Union
};

NFA char_to_NFA(char x)
{
	NFA nfa(2);
	nfa.setStartState(0);
	nfa.addAcceptState(1);
	nfa.addEdge(0, 1, x);

	return nfa;
}


bool has_star(string str, int index)
{
	int count = 1;
	for (int i = index; i < str.length(); i++)
	{
		if (str[i] == '(') count++;
		if (str[i] == ')') count--;
		if (count == 0) {
			return str[i + 1] == '*';
		}
	}
}


NFA re_to_NFA(string str, int &index)
{
	NFA temp1;
	bool temp1Empty = true;
	State state = Concatenate;

	for (; index < str.length(); index++)
	{
		switch (str[index])
		{
		case '*':
			temp1 = nfa_star(temp1);
			break;
		
		case '(':
			if (state == Concatenate)
			{
				index = index + 1;
				if (temp1Empty) {
					temp1Empty = false;
					temp1 = re_to_NFA(str, index);
				}
				else {
					if (has_star(str, index)) {
						temp1 = nfa_concatenate(temp1, nfa_star(re_to_NFA(str, index)));
					}
					else {
						temp1 = nfa_concatenate(temp1, re_to_NFA(str, index));
					}
				}
			}
			else if (state == Union)
			{
				index = index + 1;
				temp1 = nfa_union(temp1, re_to_NFA(str, index));
			}
			break;
		
		case ')':
			return temp1;
			break;
		
		case '|':
			state = Union;
			break;

		default:
			if (temp1Empty) 
			{
				temp1Empty = false;
				temp1 = char_to_NFA(str[index]);
			}
			else 
			{
				if (state == Concatenate) {
					temp1 = nfa_concatenate(temp1, char_to_NFA(str[index]));
				}
				else if(state == Union) {
					temp1 = nfa_union(temp1, re_to_NFA(str, index));
					state = Concatenate;
				}
			}
			break;
		}
	}
	
	return temp1;
}