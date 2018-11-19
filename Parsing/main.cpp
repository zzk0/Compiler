#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

namespace tiny
{
	struct ParsingTreeNode {
		string data;
		vector<ParsingTreeNode*> children;
	};

	bool accept = false;

	vector<pair<string, string>> getAllToken()
	{
		// 1, Read the token,
		// 2, Process the token identifier, to find out the keyword
		ifstream file;
		file.open("lexResult.txt");
		if (!file) {
			cerr << "Open File Failed" << endl;
		}
		vector<pair<string, string>> tokens;
		char buffer1[255], buffer2[255];

		string keywords[] = {
			"read", "write", "do", "while", "downto", "enddo", "for", "if", "endwhile", "else"
		};

		while (file >> buffer1 >> buffer2)
		{
			string content(buffer1);
			string token(buffer2);
			if (content == "END") break;

			bool isKeyword = false;

			for (int i = 0; i < 10; i++)
			{
				if (content == keywords[i] && token == "identifier")
				{
					tokens.push_back(pair<string, string>(content, keywords[i]));
					isKeyword = true;
					break;
				}
			}

			if (!isKeyword)
			{
				tokens.push_back(pair<string, string>(content, token));
			}

		}

		file.close();

		return tokens;
	}

	int index = 0;
	vector<pair<string, string>> allToken;
	pair<string, string> getToken()
	{
		if (index >= allToken.size()) return pair<string, string>();
		return allToken[index++];
	}

	pair<string, string> tok;
	pair<string, string> preTok;

	void advance()
	{
		tok = preTok;
		preTok = getToken();
		if (tok.first == "" && tok.second == "") {
			accept = true;
			//cout << "parse to the end" << endl;
		}
	}

	void error()
	{
		//cerr << "Reject !!!" << endl;
		getToken();
	}

	void eat(string token)
	{
		if (token == tok.second) advance();
		else error();
	}

	void exp();
	void term();
	void simple_exp();
	void stmt_sequence();

	void factor()
	{
		if (tok.second == "(") {
			eat("(");
			exp();
			eat(")");
		}
		else if (tok.second == "number") {
			eat("number");
		}
		else if (tok.second == "identifier") {
			eat("identifier");
		}
		else {
			error();
		}
	}

	void mulop()
	{
		if (tok.second == "*") {
			eat("*");
		}
		else if (tok.second == "/") {
			eat("/");
		}
		else {
			error();
		}
	}
	
	void mul_part()
	{
		if (tok.second == "*" || tok.second == "/") {
			mulop();
			term();
		}
	}

	void term()
	{
		if (tok.second == "(" || tok.second == "number" || tok.second == "identifier") {
			factor();
			mul_part();
		}
		else {
			error();
		}
	}

	void addop()
	{
		if (tok.second == "+") {
			eat("+");
		}
		else if (tok.second == "-") {
			eat("-");
		}
		else {
			error();
		}
	}

	void add_part()
	{
		if (tok.second == "+" || tok.second == "-") {
			addop();
			simple_exp();
		}
	}

	void simple_exp()
	{
		if (tok.second == "(" || tok.second == "number" || tok.second == "identifier") {
			term();
			add_part();
		}
		else {
			error();
		}
	}

	void comparison_op()
	{
		if (tok.second == "<") {
			eat("<");
		}
		else if (tok.second == ">") {
			eat(">");
		}
		else if (tok.second == "=") {
			eat("=");
		}
		else {
			error();
		}
	}

	void comparison_part()
	{
		if (tok.second == ">" || tok.second == "=") {
			comparison_op();
			simple_exp();
		}
	}

	void exp()
	{
		if (tok.second == "(" || tok.second == "number" || tok.second == "identifier") {
			simple_exp();
			comparison_part();
		}
		else {
			error();
		}
	}

	void else_part()
	{
		if (tok.second == "else") {
			eat("else");
			stmt_sequence();
		}
	}

	void if_stmt()
	{
		eat("if");
		eat("(");
		exp();
		eat(")");
		stmt_sequence();
		else_part();
	}

	void assign_stmt()
	{
		if (tok.second == "identifier") {
			eat("identifier");
			eat(":=");
			exp();
			eat(";");
		}
		else {
			error();
		}
	}

	void read_stmt()
	{
		eat("read");
		eat("identifier");
		eat(";");
	}

	void write_stmt()
	{
		eat("write");
		eat("identifier");
		eat(";");
	}

	void dowhile_stmt()
	{
		eat("do");
		stmt_sequence();
		eat("while");
		eat("(");
		exp();
		eat(")");
		eat(";");
	}

	void to_stmt()
	{
		if (tok.second == "to") {
			eat("to");
		}
		else if (tok.second == "downto") {
			eat("downto");
		}
		else {
			error();
		}
		simple_exp();
		eat("do");
		stmt_sequence();
		eat("enddo");
	}

	void for_stmt()
	{
		eat("for");
		eat("identifier");
		eat(":=");
		simple_exp();
		to_stmt();
	}
	
	void while_stmt()
	{
		if (tok.second == "while") {
			eat("while");
			exp();
			eat("do");
			stmt_sequence();
			eat("endwhile");
		}
		else {
			error();
		}
	}

	void statement()
	{
		if (tok.second == "if")
		{
			if_stmt();
		}
		else if (tok.second == "identifier")
		{
			assign_stmt();
		}
		else if (tok.second == "read")
		{
			read_stmt();
		}
		else if (tok.second == "write")
		{
			write_stmt();
		}
		else if (tok.second == "while")
		{
			while_stmt();
		}
		else if (tok.second == "do")
		{
			dowhile_stmt();
		}
		else if (tok.second == "for")
		{
			for_stmt();
		}
		else
		{
			error();
		}
	}

	void stmt_sequence()
	{
		if (tok.second == "if" || tok.second == "identifier" || tok.second == "write" ||
			tok.second == "do" || tok.second == "for" || tok.second == "read" || tok.second == "while") {
			
			if (tok.second == "while" && preTok.second == "(" ) return;
			
			statement();
			stmt_sequence();
		}
	}

	void program()
	{
		advance();
		stmt_sequence();
	}

};

int main()
{
	tiny::allToken = tiny::getAllToken();
	tiny::preTok = tiny::getToken();

	tiny::program();

	if (tiny::accept)
	{
		cout << "accept" << endl;
	}
}
