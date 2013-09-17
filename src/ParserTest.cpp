#include "Parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main() {
	Parser p;
	vector<Token> vec;
	string s;

	// open a file and get all info
	ifstream ifs;
	ifs.open("exampleSQL");
	getline(ifs,s,'\0');

	// parse the file using the output from parser
	p.parse(s);

	return 0;
}
