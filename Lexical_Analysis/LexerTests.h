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

	auto result = lexer.scan();
	for (int i = 0; i < result.size(); i++)
	{
		cout << result[i].first << " " << result[i].second << endl;
	}
}

string keywords[] = { "asm", "else", "new", "this", "auto", "enum", "operator",
								"throw", "bool", "explicit", "private", "true", "break", 
								"export", "protected", "try", "case", "extern", "public", 
								"typedef", "catch", "false", "register", "typeid", "char", 
								"float", "reinterpret_cast", "typename", "class", "for", "return",
								"union", "const", "friend", "short", "unsigned", "const_cast", "goto", 
								"signed", "using", "continue", "if", "sizeof", "virtual", "default", 
								"inline", "static", "void", "delete", "int", "static_cast", "volatile", 
								"do", "long", "struct", "wchar_t", "double", "mutable", "switch",
								"while", "dynamic_cast", "namespace", "template" };

void lexer_test2()
{
	Lexer lexer;
	lexer.addPattern("/");

	// comment
	lexer.addPattern(R"(/\*(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|,| |\n)*\*/)");
	lexer.addPattern(R"(//(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|,| )*)");
	// macron
	lexer.addPattern(R"(#(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|<|>|.| )*)");
	// identifier
	lexer.addPattern("(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_)(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)*");
	lexer.addPattern("(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)");
	// number
	lexer.addPattern("(0|1|2|3|4|5|6|7|8|9)*.*(0|1|2|3|4|5|6|7|8|9)*");
	// keyword
	//for (int i = 0; i < 63; i++)
	//{
	//	lexer.addPattern(keywords[i]);
	//}
	// parentheses
	lexer.addPattern(R"(\()");
	lexer.addPattern(R"(\))");
	lexer.addPattern("[");
	lexer.addPattern("]");
	lexer.addPattern("{");
	lexer.addPattern("}");

	// operators
	lexer.addPattern(R"(\+)");
	lexer.addPattern("-");
	lexer.addPattern(R"(\*)");
	lexer.addPattern("<<");
	lexer.addPattern(">>");
	lexer.addPattern("<");
	lexer.addPattern(">");
	lexer.addPattern("<=");
	lexer.addPattern(">=");
	lexer.addPattern("==");
	lexer.addPattern("!=");
	lexer.addPattern(R"(\+\+)");
	lexer.addPattern("--");
	lexer.addPattern("%");
	lexer.addPattern("=");
	lexer.addPattern(R"(\+=)");
	lexer.addPattern("-=");
	lexer.addPattern(R"(\*=)");
	lexer.addPattern("/=");
	lexer.addPattern("->");
	lexer.addPattern("&");
	lexer.addPattern("!");
	lexer.addPattern("^");
	lexer.addPattern(R"(\|)");
	lexer.addPattern(R"(\|\|)");
	lexer.addPattern("&&");
	lexer.addPattern(",");

	// String
	lexer.addPattern(R"(\"(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)*\")");

	// Delimiter
	lexer.addPattern(";");

	// Blank
	lexer.addPattern(" ");
	lexer.addPattern("\n");
	lexer.addPattern("\t");

	lexer.convertToDFA();

	lexer.readText("Scanner.cpp");

	lexer.inputSample("/*comment*/", "COMMENT");
	lexer.inputSample("// comment", "COMMENT");
	lexer.inputSample("#include <iostream>", "MACRON");
	lexer.inputSample("abc1", "IDENTIFIER");
	for (char x = 'a'; x <= 'z'; x++) {
		string str(1, x);
		lexer.inputSample(str, "IDENTIFIER");
	}
	lexer.inputSample("1.2", "NUMBER");
	lexer.inputSample("20", "NUMBER");

	// The following will slow down the program very much
	// we can figure out the keywords in the next phase not here.
	// Process the tokens to get the keyword
	//for (int i = 0; i < 63; i++)
	//{
	//	lexer.inputSample(keywords[i], keywords[i]);
	//}
	
	lexer.inputSample(R"(\"string\")", "STRING");
	lexer.inputSample(";", "DELIMITER");
	lexer.inputSample(" ", "BLANK");
	lexer.inputSample("\n", "BLANK");
	lexer.inputSample("\t", "BLANK");
	// parentheses
	lexer.inputSample(R"(\()", "sLPARE");
	lexer.inputSample(R"(\))", "sRPARE");
	lexer.inputSample("[", "mLPARE");
	lexer.inputSample("]", "mRPARE");
	lexer.inputSample("{", "lLPARE");
	lexer.inputSample("}", "lRPARE");

	// operators
	lexer.inputSample("+", "PLUS");
	lexer.inputSample("-", "MINUS");
	lexer.inputSample("*", "MULTI");
	lexer.inputSample("/", "DIVIDE");
	lexer.inputSample("<<", "LL");
	lexer.inputSample(">>", "RR");
	lexer.inputSample("<", "SMALLER");
	lexer.inputSample(">", "GREATTER");
	lexer.inputSample("<=", "SE");
	lexer.inputSample(">=", "GE");
	lexer.inputSample("==", "EQUAL");
	lexer.inputSample("!=", "NOTEQUAL");
	lexer.inputSample("++", "PLUSPLUS");
	lexer.inputSample("--", "MINUSMINUS");
	lexer.inputSample("%", "REMAINDER");
	lexer.inputSample("=", "ASSIGN");
	lexer.inputSample("+=", "PE");
	lexer.inputSample("-=", "ME");
	lexer.inputSample("*=", "MULE");
	lexer.inputSample("/=", "DE");
	lexer.inputSample("->", "MEMBER");
	lexer.inputSample("&", "AND");
	lexer.inputSample("!", "NOT");
	lexer.inputSample("^", "XOR");
	lexer.inputSample(R"(\|)", "OR");
	lexer.inputSample(R"(\|\|)", "LOR");
	lexer.inputSample("&&", "LAND");
	lexer.inputSample(",", "DOT");


	// DFA not minized 155ms
	lexer.dfa.removeUnreachableStates();

	auto result = lexer.scan();
	for (int i = 0; i < result.size(); i++)
	{
		if (result[i].second == "COMMENT" || result[i].second == "BLANK") continue;
		cout << result[i].first << "\t\t" << result[i].second << endl;
	}

}