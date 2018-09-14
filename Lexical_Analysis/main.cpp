#include <iostream>
#include <fstream>
#include  <string>
#include "Scanner.h"
using namespace std;



int main()
{
	Scanner scanner("sample.cpp");
	scanner.StartScan();
	scanner.OutputResult();
	system("pause");
}

/*
Read File sample code:
ifstream infile;
infile.open("sample.cpp");

if (!infile) {
cout << "Open Failed" << endl;
return 0;
}
char ch;
while (!infile.eof()) {
infile.get(ch);
if (ch == '\n') break;
cout << ch;
}
ch = infile.peek();
cout << ch;
ch = infile.get();
cout << ch;
*/