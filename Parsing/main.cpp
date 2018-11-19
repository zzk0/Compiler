#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

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
		"read", "write", "do", "while", "downto", "enddo", "for", "if", "endwhile"
	};

	while (file >> buffer1 >> buffer2)
	{
		string content(buffer1);
		string token(buffer2);
		if (content == "END") break;

		bool isKeyword = false;

		for (int i = 0; i < 9; i++)
		{
			if (content == keywords[i] && token == "IDENTIFIER")
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
}


int index = 0;
vector<pair<string, string>> allToken;
pair<string, string> getToken()
{
	if (index > allToken.size()) return pair<string, string>();
	return allToken[index++];
}


int main()
{
	allToken = getAllToken();
}
