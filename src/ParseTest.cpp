#include "Parse.h"
#include <iostream>
#include <string>

using namespace std;

void print_node(ParseNode* node, int level) {
	cout << "|" << flush;
	for (int i = 0; i < level; ++i)
		cout << "---" << flush;

	cout << node->value << endl;
	for (int i = 0; i < node->children.size(); ++i)
		print_node(node->children[i],level+1);
}

int main() {
	Parser p;
	string s;
	vector<Token> vec;
	cout << "Enter code to parse: " << flush;
	getline(cin, s);
	cout << "Str:" << s << endl;
	//vec = p.tokenize(s);
	//cout << "Token results: " << endl;
	//for (int i = 0; i < vec.size(); ++i) {
		//cout << "Token value: " << vec[i].value << endl;
		//switch (vec[i].type) {
			//case KEYWORD:
				//cout << "Token type: KEYWORD" << endl << endl;
				//break;
			//case IDENTIFIER:
				//cout << "Token type: IDENTIFIER" << endl << endl;
				//break;
//
			//case NUMBER:
				//cout << "Token type: NUMBER" << endl << endl;
				//break;
			//case OPERATOR:
				//cout << "Token type: OPERATOR" << endl << endl;
				//break;
			//case PARSE_ERROR:
				//cout << "PARSE ERROR" << endl << endl;
				//break;
		//}
	//}
	vector<ParseNode*> statements = p.parse(s);
	for(int i = 0; i < statements.size(); ++i) {
		print_node(statements[i],0);
	}
	return 0;
}
