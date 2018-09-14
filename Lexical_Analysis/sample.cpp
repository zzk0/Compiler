/*
This file is used to test the lexical analysis program
Skip Comment
*/

#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

double sum(double a, double b)
{
	double c = a + b;
	return c;
}

void test_various_operation()
{
	// Initialize the variable in different way
	int a = 1;
	int b = 2, c = 3, d = 4;
	a = b + c;
	c += d;
	b = c - d;
	d -= a;
	a = a * b;
	a *= b;
	b = b / c;
	b /= c;

	a++;
	a--;
}

void test_string()
{
	int i;
	cin >> i;
	if (i > 3) cout << "OK" << endl;
	// string and character
	string a_string = "This is a string";
	char a_char = 'c';
}

void test_control()
{
	for (int i = 0; i < 10; i++) {
		if (1 == 1) continue;
		else if (1 == 2) break;
	}

	int i = 0;
	while (i < 10) {
		i++;
	}

	do {
		i++;
	} while (i < 20);
}

void test_output()
{
	int b = 2333;
	int a = b << 1;
	b = a >> 1;
	cin >> a;
	if(a < b)
		cout << "Test";
}