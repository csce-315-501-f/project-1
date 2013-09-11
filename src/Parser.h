#ifndef PARSER_H
#define PARSER_H

#define DEBUG false

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iostream>
#include <cctype>

using namespace std;

typedef int TokenType;
#define KEYWORD 0
#define IDENTIFIER 1
#define NUMBER 2
#define OPERATOR 3
#define PARSE_ERROR 4
#define NO_TOKEN -1

struct Token {
	string value;
	TokenType type;
};

class TokenStream {
public:
	TokenStream() : i(0) {}
	TokenStream(vector<Token> t) : i(0), tokens(t) {}
	vector<Token> tokens;
	Token get() {
		if (i < tokens.size())
			return tokens[i++];
		else {
			Token tmp;
			tmp.value = "";
			tmp.type = NO_TOKEN;
			return tmp;
		}
	}
	void unget() {
		if (i > 0) --i;
	}
private:
	int i;
};

typedef vector< vector<Token> > Statement;

class Parser {

public:
	Parser() {
		init_values();
	}
	vector<string> parse(string s);

//private:
	void init_values();
	vector<Token> tokenize(string s);
	Statement break_statements(vector<Token>);
	bool check_grammar(vector<Token> tokens);

	bool query(TokenStream& tokens);
	bool expr(TokenStream& tokens);
	bool atomic_expr(TokenStream& tokens);
	bool operand(TokenStream& tokens);
	bool attrib(TokenStream& tokens);
	bool attrib_list(TokenStream& tokens);
	bool literal(TokenStream& tokens);
	bool op(TokenStream& tokens);
	bool comparison(TokenStream& tokens);
	bool conjunction(TokenStream& tokens);
	bool condition(TokenStream& tokens);
	bool selection(TokenStream& tokens);
	bool projection(TokenStream& tokens);
	bool renaming(TokenStream& tokens);
	bool union_g(TokenStream& tokens);
	bool difference(TokenStream& tokens);
	bool product(TokenStream& tokens);

	bool command(TokenStream& tokens);
	bool open_cmd(TokenStream& tokens);
	bool close_cmd(TokenStream& tokens);
	bool write_cmd(TokenStream& tokens);
	bool exit_cmd(TokenStream& tokens);
	//bool show_cmd(TokenStream& tokens);

	string* keywords;
	set<string> Keywords;
};


#endif
