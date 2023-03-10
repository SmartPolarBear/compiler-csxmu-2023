#include <iostream>
#include <string>
#include <experimental/generator>
#include <vector>
#include <stdexcept>

import scanner;
import parser;

using namespace std;

int main()
{
	string code;
	cin >> code;
	parser p{ scan(code) };
	cout << p.parse();
	return 0;
}