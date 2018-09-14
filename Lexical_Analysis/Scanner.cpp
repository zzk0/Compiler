#include "Scanner.h"

Scanner::Scanner()
{

}

Scanner::Scanner(const char *name)
{
	fileName = name;
}

Scanner::~Scanner()
{
	infile.close();
}

bool Scanner::InAlphabet(char c)
{
	if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A')) {
		return true;
	}
	else {
		return false;
	}
}

bool Scanner::IsNumber(char c)
{
	if (c >= '0' && c <= '9') {
		return true;
	}
	else {
		return false;
	}
}

vector<string> keyword_table = {
	"if", "else", "for", "bool", "int", "double", "float", "long", "void", "while", "do", "include", "using", "namespace", "continue"
};

bool Scanner::IsKeyword(string str)
{
	for (int i = 0; i < keyword_table.size(); i++) {
		if (str == keyword_table[i]) return true;
	}
	return false;
}

bool Scanner::IsIncludePath()
{
	int current_pos = infile.tellg();
	string line;
	getline(infile, line);
	infile.seekg(current_pos, std::ios_base::beg);
	if (line.find('>') != std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}

void Scanner::OpenFile()
{
	infile.open(fileName);
}

void Scanner::StartScan()
{
	OpenFile();
	int line_number = 1;
	while (!infile.eof()) 
	{
		char ch = infile.get();
		if (ch == '/') {
			char n_ch = infile.peek();

			if (n_ch == '/') {
				do {
					n_ch = infile.get();
				} while (n_ch != '\n');
				line_number++;
			}
			else if (n_ch == '*') {
				infile.get();
				char next;
				do {
					n_ch = infile.get();
					next = infile.peek();
					if (n_ch == '\n') line_number++;
				} while (n_ch != '*' || next != '/');
			}
		}
		else if (InAlphabet(ch)) {
			char next = infile.peek();
			string str(1, ch);

			while (InAlphabet(next) || IsNumber(next) || next == '_') {
				infile.get();
				str.append(1, next);
				next = infile.peek();
			}
			if (IsKeyword(str)) {
				Tokens.push_back(Token(str, KEYWORD, line_number));
			}
			else {
				Tokens.push_back(Token(str, ID, line_number));
			}

		}
		else if (IsNumber(ch)) {
			char next = infile.peek();
			
			string str(1, ch);
			while (IsNumber(next)) {
				infile.get();
				str.append(1, next);
				next = infile.peek();
			}
			Tokens.push_back(Token(str, NUM, line_number));
		}
		else if (ch == '\"') {
			char next = infile.get();
			string str;
			while (next != '\"') {
				str.append(1, next);
				next = infile.get();
			}
			Tokens.push_back(Token(str, STRING, line_number));
		}
		else {
			char peek;
			switch (ch)
			{
			case '#':
				Tokens.push_back(Token(string(1, ch), MACRO, line_number));
				break;
			case '=':
				peek = infile.peek();
				if (peek == '=') {
					infile.get();
					Tokens.push_back(Token(string("=="), OP, line_number));
				}
				else {
					Tokens.push_back(Token(string(1, ch), ASSIGN, line_number));
				}
				break;
			case '+':
				peek = infile.peek();
				if (peek == '+') {
					infile.get();
					Tokens.push_back(Token(string("++"), OP, line_number));
				}
				else {
					Tokens.push_back(Token(string(1, ch), OP, line_number));
				}
				break;
			case '-':
				peek = infile.peek();
				if (peek == '-') {
					infile.get();
					Tokens.push_back(Token(string("--"), OP, line_number));
				}
				else {
					Tokens.push_back(Token(string(1, ch), OP, line_number));
				}
				break;
			case '*':
			case '\\':
				peek = infile.peek();
				if (peek == '=') {
					infile.get();
					Tokens.push_back(Token(string(1, ch).append(1, '='), OP, line_number));
				}
				else {
					Tokens.push_back(Token(string(1, ch), OP, line_number));
				}
				break;
			case '<':
				peek = infile.peek();
				if (peek == ' ') {
					Tokens.push_back(Token(string(1, '<'), OP, line_number));
				}
				else if (peek == '<') {
					infile.get();
					Tokens.push_back(Token(string("<<"), OP, line_number));
				}
				else if (IsIncludePath()) {
					peek = infile.get();
					string str;
					while (peek != '>') {
						str.append(1, peek);
						peek = infile.get();
					}
					Tokens.push_back(Token(string(1, ch), BRACKETS, line_number));
					Tokens.push_back(Token(str, KEYWORD, line_number));
					infile.get();
					Tokens.push_back(Token(string(">"), BRACKETS, line_number));
				}
				else {
					Tokens.push_back(Token(string(1, ch), BRACKETS, line_number));
				}
				break;
			case '>':
				peek = infile.peek();
				if (peek == ' ') {
					Tokens.push_back(Token(string(1, '>'), OP, line_number));
				}
				else if (peek == '>') {
					infile.get();
					Tokens.push_back(Token(string(">>"), OP, line_number));
				}
				else {
					Tokens.push_back(Token(string(1, ch), BRACKETS, line_number));
				}
				break;
			case '{':
			case '}':
			case '[':
			case ']':
			case '(':
			case ')':
				Tokens.push_back(Token(string(1, ch), BRACKETS, line_number));
				break;
			case ',':
				Tokens.push_back(Token(string(1, ch), DOT, line_number));
				break;
			case ';':
				Tokens.push_back(Token(string(1, ch), SEMICOLON, line_number));
				break;
			case '\n':
				line_number++;
			case '\t':
			case ' ':
				// Skip all space 
				break;
			default:
				Tokens.push_back(Token(string(1, ch), UNKNOWN, line_number));
				break;
			}
		}
	}
}

const char *TypeStr[] = {
	"IF", "ELSE", "FOR", "DO", "WHILE", "OP", "ASSIGN", "MACRO", "DOT", "SEMICOLON", "NUM", "ID", "STRING", "BRACKETS", "KEYWORD", "UNKNOWN"
};

void Scanner::OutputResult()
{
	for (int i = 0; i < Tokens.size(); i++)
	{
		cout << Tokens[i].line_number << "\t" << Tokens[i].str << "\t " << TypeStr[Tokens[i].type] << endl;
	}
}