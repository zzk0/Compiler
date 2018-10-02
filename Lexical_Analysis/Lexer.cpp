#include "Lexer.h"



Lexer::Lexer()
{
}


Lexer::~Lexer()
{
}


/*
Reference:
https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
*/
void Lexer::readText(const char *fileName)
{
	ifstream t(fileName);
	stringstream buffer;
	buffer << t.rdbuf();
	text = buffer.str();
}


void Lexer::addPattern(string pattern)
{
	patterns.push_back(pattern);
}


void Lexer::convertToDFA()
{
	for (int i = 0; i < patterns.size(); i++) {
		NFAs.push_back(re_to_NFA(patterns[i]));
	}
	NFA nfa = NFAs[0];
	for (int i = 1; i < NFAs.size(); i++) {
		nfa = nfa_union(nfa, NFAs[i]);
	}
	dfa = nfa.convertToDFA();
}


/*
input: x
stateID: y
*/
void Lexer::inputSample(string x, string y)
{
	dfa.reset();
	bool accepted;
	int acceptedID;
	for (int i = 0; i < x.length(); i++)
	{
		dfa.runOneStep(x[i], accepted, acceptedID);
	}
	dfa.addStateID(acceptedID, y);
	//dfa.stateIDs[acceptedID] = y;
}


vector<pair<string, string>> Lexer::scan()
{
	int startIndex = 0, lastAcceptIndex = 0, indexExplorer = 0;
	bool accepted;
	int currentState;
	vector<pair<string, string>> result;
	dfa.reset();
	bool haveAccepted = false;
	int lastAcceptState = 0;
	while (indexExplorer < text.length())
	{
		dfa.runOneStep(text[indexExplorer], accepted, currentState);
		if (accepted)
		{
			lastAcceptState = currentState;
			haveAccepted = true;
			lastAcceptIndex = indexExplorer;
			indexExplorer++;
			if (indexExplorer == text.length()) {
				string input = text.substr(startIndex, lastAcceptIndex - startIndex + 1);
				string stateID = dfa.stateIDs[dfa.getLastAcceptState()];
				result.push_back(pair<string, string>(input, stateID));
				break;
			}
		}
		else if (!accepted && (currentState == -1) && haveAccepted)
		{
			string input = text.substr(startIndex, lastAcceptIndex - startIndex + 1);
			string stateID = dfa.stateIDs[lastAcceptState];
			startIndex = lastAcceptIndex + 1;
			indexExplorer = lastAcceptIndex + 1;
			lastAcceptIndex++;
			result.push_back(pair<string, string>(input, stateID));

			dfa.reset();
			haveAccepted = false;
		}
		else {
			indexExplorer++;
			if (indexExplorer == text.length()) cerr << "EOF" << endl;
		}
	}
	return result;
}
