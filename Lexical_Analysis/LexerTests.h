#pragma once

#include "Lexer.h"

void lexer_test1()
{
	Lexer lexer;
	lexer.addPattern("if");
	lexer.addPattern("else");
	lexer.addPattern(" ");
	lexer.convertToDFA();

	lexer.readText("test.txt");

	lexer.inputSample("if", "IF");
	lexer.inputSample("else", "ELSE");
	lexer.inputSample(" ", "BLANK");

	//if (lexer.dfa.IsAccepted("if", 2)) cout << "OK" << endl;
	//if (lexer.dfa.IsAccepted("ifx", 3)) cout << "NO" << endl;
	auto result = lexer.scan();
	for (int i = 0; i < result.size(); i++)
	{
		cout << result[i].first << " " << result[i].second << endl;
	}
}


void lexer_test2()
{
	Lexer lexer;
	lexer.addPattern("if");
	lexer.addPattern("else");
	lexer.addPattern(" ");
	lexer.convertToDFA();

	lexer.readText("test.txt");

	lexer.inputSample("if", "IF");
	lexer.inputSample("else", "ELSE");
	lexer.inputSample(" ", "BLANK");

	//if (lexer.dfa.IsAccepted("if", 2)) cout << "OK" << endl;
	//if (lexer.dfa.IsAccepted("ifx", 3)) cout << "NO" << endl;
	auto result = lexer.scan();
	for (int i = 0; i < result.size(); i++)
	{
		cout << result[i].first << " " << result[i].second << endl;
	}
