#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

namespace tiny
{
	struct ParsingTreeNode {
		string data;
		static int index;
		int count;
		vector<ParsingTreeNode*> children;

		ParsingTreeNode() {
			this->data = "";
			this->count = this->index++;
		}

		ParsingTreeNode(string x) {
			this->data = x;
			this->count = this->index++;
		}
	};

	int ParsingTreeNode::index = 0;

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

	ParsingTreeNode* exp();
	ParsingTreeNode* term();
	ParsingTreeNode* simple_exp();
	ParsingTreeNode* stmt_sequence();

	ParsingTreeNode* factor()
	{
		ParsingTreeNode *node = new ParsingTreeNode("factor"); 
		if (tok.second == "(") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("(");
			node->children.push_back(exp());
			//exp();
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat(")");
		}
		else if (tok.second == "number") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("number");
		}
		else if (tok.second == "identifier") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("identifier");
		}
		else {
			error();
		}
		return node;
	}

	ParsingTreeNode* mulop()
	{
		ParsingTreeNode *node = new ParsingTreeNode("mulop"); 
		if (tok.second == "*") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("*");
		}
		else if (tok.second == "/") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("/");
		}
		else {
			error();
		}
		return node;
	}
	
	ParsingTreeNode* mul_part()
	{
		ParsingTreeNode *node = new ParsingTreeNode("mul_part"); 
		if (tok.second == "*" || tok.second == "/") {
			node->children.push_back(mulop());
			node->children.push_back(term());
			//mulop();
			//term();
		}
		return node;
	}

	ParsingTreeNode* term()
	{
		ParsingTreeNode *node = new ParsingTreeNode("term"); 
		if (tok.second == "(" || tok.second == "number" || tok.second == "identifier") {
			node->children.push_back(factor());
			node->children.push_back(mul_part());
		}
		else {
			error();
		}
		return node;
	}

	ParsingTreeNode* addop()
	{
		ParsingTreeNode *node = new ParsingTreeNode("addop"); 
		if (tok.second == "+") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("+");
		}
		else if (tok.second == "-") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("-");
		}
		else {
			error();
		}
		return node;
	}

	ParsingTreeNode* add_part()
	{
		ParsingTreeNode *node = new ParsingTreeNode("add_part"); 
		if (tok.second == "+" || tok.second == "-") {
			node->children.push_back(addop());
			node->children.push_back(simple_exp());
		}
		return node;
	}

	ParsingTreeNode* simple_exp()
	{
		ParsingTreeNode *node = new ParsingTreeNode("simple_exp"); 
		if (tok.second == "(" || tok.second == "number" || tok.second == "identifier") {
			node->children.push_back(term());
			node->children.push_back(add_part());
		}
		else {
			error();
		}
		return node;
	}

	ParsingTreeNode* comparison_op()
	{
		ParsingTreeNode *node = new ParsingTreeNode("comparison_op"); 
		if (tok.second == "<") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("<");
		}
		else if (tok.second == ">") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat(">");
		}
		else if (tok.second == "=") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("=");
		}
		else {
			error();
		}
		return node;
	}

	ParsingTreeNode* comparison_part()
	{
		ParsingTreeNode *node = new ParsingTreeNode("comparison_part"); 
		if (tok.second == ">" || tok.second == "=") {
			node->children.push_back(comparison_op());
			node->children.push_back(simple_exp());
		}
		return node;
	}

	ParsingTreeNode* exp()
	{
		ParsingTreeNode *node = new ParsingTreeNode("exp"); 
		if (tok.second == "(" || tok.second == "number" || tok.second == "identifier") {
			node->children.push_back(simple_exp());
			node->children.push_back(comparison_part());
		}
		else {
			error();
		}
		return node;
	}

	ParsingTreeNode* else_part()
	{
		ParsingTreeNode *node = new ParsingTreeNode("else_part"); 
		if (tok.second == "else") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("else");
			node->children.push_back(stmt_sequence());
		}
		return node;
	}

	ParsingTreeNode* if_stmt()
	{
		ParsingTreeNode *node = new ParsingTreeNode("if_stmt"); 
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("if");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("(");
		node->children.push_back(exp());
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat(")");
		node->children.push_back(stmt_sequence());
		node->children.push_back(else_part());

		return node;
	}

	ParsingTreeNode* assign_stmt()
	{
		ParsingTreeNode *node = new ParsingTreeNode("assign_stmt"); 
		if (tok.second == "identifier") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("identifier");
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat(":=");
			node->children.push_back(exp());
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat(";");
		}
		else {
			error();
		}
		return node;
	}

	ParsingTreeNode* read_stmt()
	{
		ParsingTreeNode *node = new ParsingTreeNode("read_stmt"); 
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("read");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("identifier");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat(";");
		return node;
	}

	ParsingTreeNode* write_stmt()
	{
		ParsingTreeNode *node = new ParsingTreeNode("write_stmt"); 
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("write");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("identifier");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat(";");
		return node;
	}

	ParsingTreeNode* dowhile_stmt()
	{
		ParsingTreeNode *node = new ParsingTreeNode("dowhile_stmt"); 
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("do");
		node->children.push_back(stmt_sequence());
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("while");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("(");
		node->children.push_back(exp());
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat(")");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat(";");

		return node;
	}

	ParsingTreeNode* to_stmt()
	{
		ParsingTreeNode *node = new ParsingTreeNode("to_stmt"); 
		if (tok.second == "to") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("to");
		}
		else if (tok.second == "downto") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("downto");
		}
		else {
			error();
		}
		node->children.push_back(simple_exp());
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("do");
		node->children.push_back(stmt_sequence());
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("enddo");
		
		return node;
	}

	ParsingTreeNode* for_stmt()
	{
		ParsingTreeNode *node = new ParsingTreeNode("for_stmt"); 
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("for");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat("identifier");
		node->children.push_back(new ParsingTreeNode(tok.first));
		eat(":=");
		node->children.push_back(simple_exp());
		node->children.push_back(to_stmt());
		return node;
	}
	
	ParsingTreeNode* while_stmt()
	{
		ParsingTreeNode *node = new ParsingTreeNode("while_stmt"); 
		if (tok.second == "while") {
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("while");
			node->children.push_back(exp());
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("do");
			node->children.push_back(stmt_sequence());
			node->children.push_back(new ParsingTreeNode(tok.first));
			eat("endwhile");
		}
		else {
			error();
		}
		return node;
	}

	ParsingTreeNode* statement()
	{
		ParsingTreeNode *node = new ParsingTreeNode("statement"); 
		if (tok.second == "if")
		{
			node->children.push_back(if_stmt());
		}
		else if (tok.second == "identifier")
		{
			node->children.push_back(assign_stmt());
		}
		else if (tok.second == "read")
		{
			node->children.push_back(read_stmt());
		}
		else if (tok.second == "write")
		{
			node->children.push_back(write_stmt());
		}
		else if (tok.second == "while")
		{
			node->children.push_back(while_stmt());
		}
		else if (tok.second == "do")
		{
			node->children.push_back(dowhile_stmt());
		}
		else if (tok.second == "for")
		{
			node->children.push_back(for_stmt());
		}
		else
		{
			error();
		}
		return node;
	}

	ParsingTreeNode* stmt_sequence()
	{
		ParsingTreeNode *node = new ParsingTreeNode("stmt_sequence"); 
		if (tok.second == "if" || tok.second == "identifier" || tok.second == "write" ||
			tok.second == "do" || tok.second == "for" || tok.second == "read" || tok.second == "while") {
			
			if (tok.second == "while" && preTok.second == "(" ) return nullptr;
			
			node->children.push_back(statement());
			node->children.push_back(stmt_sequence());
		}

		return node;
	}

	ParsingTreeNode* program()
	{
		ParsingTreeNode *node = new ParsingTreeNode("program"); 
		advance();
		node->children.push_back(stmt_sequence());
		return node;
	}


	void generate_to_dot(ParsingTreeNode *tree)
	{
		ofstream outFile("ParsingTree.dot");

		outFile << "digraph graphname {\n";
		
		ParsingTreeNode *node = tree;

		queue<ParsingTreeNode*> NodeQueue;
		NodeQueue.push(node);
		
		int index = 0;
		while (!NodeQueue.empty())
		{
			ParsingTreeNode *cur = NodeQueue.front();
			NodeQueue.pop();

			for (int i = 0; i < cur->children.size(); i++)
			{
				if (cur->children[i] != nullptr) {
					NodeQueue.push(cur->children[i]);
					outFile << "\"" << cur->count << ": "<< cur->data << "\"" << " -> " << "\"" << cur->children[i]->count <<": " << cur->children[i]->data  << "\"" << endl;
				}
			}
			index++;
		}

		outFile << "}";
	}
};

int main()
{
	tiny::allToken = tiny::getAllToken();
	tiny::preTok = tiny::getToken();

	tiny::ParsingTreeNode *tree = tiny::program();

	if (tiny::accept)
	{
		cout << "accept" << endl;
		tiny::generate_to_dot(tree);
	}
}
