#pragma once

#include "NFA.h"
#include <iostream>
#include <string>
#include <stack>
#include <queue>
using namespace std;


inline NFA char_to_NFA(char x)
{
	NFA nfa(2);
	nfa.setStartState(0);
	nfa.addAcceptState(1);
	nfa.addEdge(0, 1, x);

	return nfa;
}


inline bool greater_precedence(char a, char b)
{
	if (a == '*' && (b =='*' || b == '|' || b =='+')) return true;
	if (a == '+' && (b == '|' || b == '+')) return true;
	return false;
}


inline bool isSpecialCharacter(char x)
{
	if (x == '|' || x == '*' || x == '(' || x == ')' || x == '+') {
		return true;
	}
	return false;
}


// Not complete yet
//string preprocess_to_basic(string str)
//{
//	string temp;
//	for (int i = 0; i < str.length(); i++)
//	{
//		bool alter = false;
//		char tempc1;
//		char tempc2;
//		if (str[i] == '+') {
//			temp += str[i - 1];
//			temp += '*';
//		}
//		else if (str[i] == '[') {
//			alter = true;
//		}
//		else if (str[i] == ']') {
//			alter = false;
//		}
//		else if (str[i] == '-' && alter) {
//			for (char tempx = tempc1; tempc1 < tempc2; tempc1++) {
//				temp += tempx;
//				temp += '|';
//			}
//			temp += tempc2;
//		}
//		else if(alter) {
//			temp += str[i];
//			temp += '|';
//		}
//		else {
//			temp += str[i];
//		}
// 	}
//	return temp;
//}


inline string preprocess(string str)
{
	string temp;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '(' || str[i] == '|' || str[i + 1] == '|' || str[i + 1] == ')' || str[i + 1] == '*' || str[i] == '\\' || (i == str.length() - 1)) {
			temp += str[i];
		}
		else {
			temp += str[i];
			temp += '+';
		}
	}
	return temp;
}


inline queue<char> infix_to_postfix(string str)
{
	queue<char> Q;
	stack<char> S;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '(') {
			S.push(str[i]);
		}
		else if (str[i] == ')') {
			while (S.top() != '(') {
				char temp = S.top();
				S.pop();
				Q.push(temp);
			};
			S.pop();
		}
		else if (str[i] == '|' || str[i] == '*' || str[i] == '+') {
			while (!S.empty() && greater_precedence(S.top(), str[i])) {
				char temp = S.top();
				S.pop();
				Q.push(temp);
			}
			S.push(str[i]);
		}
		else if (str[i] == '\\') {
			Q.push('\\');
			if (str[i + 1] == 'n') {
				Q.push('\n');
			}
			else {
				Q.push(str[i + 1]);
			}
			i++;
		}
		else {
			Q.push(str[i]);
		}
	}
	while (!S.empty()) {
		char temp = S.top();
		S.pop();
		Q.push(temp);
	}

	return Q;
}


inline NFA evaluate(queue<char> Q)
{
	stack<NFA> S;
	while (!Q.empty())
	{
		if (Q.front() == '\\') {
			Q.pop();
			S.push(char_to_NFA(Q.front()));
		}
		else if (!isSpecialCharacter(Q.front())) {
			S.push(char_to_NFA(Q.front()));
		}
		else {
			NFA b = S.top(); S.pop();
			NFA a;
			bool lastFlag = (S.size() == 1);
			if (!S.empty()) { a = S.top(); S.pop(); }
			NFA c;
			switch (Q.front())
			{
			case '+':
				c = nfa_concatenate(a, b);
				S.push(c);
				break;
			case '*':
				if(lastFlag || !S.empty()) S.push(a);
				c = nfa_star(b);
				S.push(c);
				break;
			case '|':
				c = nfa_union(a, b);
				S.push(c);
				break;
			default:
				break;
			}
		}
		Q.pop();
	}
	return S.top();
}


inline NFA re_to_NFA(string str)
{
	str = preprocess(str);
	queue<char> Q = infix_to_postfix(str);
	return evaluate(Q);
}
