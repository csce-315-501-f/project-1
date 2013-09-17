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

/*
 * TokenType
 * This defines the possible types that a token may be
 */
typedef int TokenType;
#define KEYWORD 0
#define IDENTIFIER 1
#define NUMBER 2
#define OPERATOR 3
#define PARSE_ERROR 4
#define LITERAL 6
#define NO_TOKEN -1

/*
 * Token class
 * Each token holds a value and a type
 */
struct Token {
	string value;
	TokenType type;
};

/*
 * TokenStream class
 * A TokenStream allows a user to retreive one token at a time
 */
class TokenStream {
public:
	TokenStream() : i(0) {}
	TokenStream(vector<Token> t) : i(0), tokens(t) {}
	vector<Token> tokens;
	int save() {return i;}
	TokenStream& restore(int x) {
		i = x;
		return *this;
	}
	Token get() {
		if (i < tokens.size())
			return tokens[i++];
		else {
			Token tmp;
			tmp.value = "NO_TOKEN";
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

/*
 * ParseNode class
 * Contains a node for the Abstract Syntax Tree
 */
struct ParseNode {
	string value;
	TokenType type;
	vector<ParseNode*> children;
	ParseNode (string v = "NO_TOKEN", TokenType t = NO_TOKEN): value(v), type(t) {}
};

typedef vector<ParseNode*> ParseList;

/*
 * Parser class
 * The Parser contains the necessary elements for a user to parse a string and return
 * grammatical correctness
 */
class Parser {
public:
	Parser() {
		init_values();
	}
	vector<string> parse(string s);

private:

	string* keywords;
	set<string> Keywords;

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

	bool relation(TokenStream& tokens);
	bool command(TokenStream& tokens);
	bool integer(TokenStream& tokens);
	bool type(TokenStream& tokens);
	bool typed_list(TokenStream& tokens);
	bool open_cmd(TokenStream& tokens);
	bool close_cmd(TokenStream& tokens);
	bool write_cmd(TokenStream& tokens);
	bool exit_cmd(TokenStream& tokens);
	bool show_cmd(TokenStream& tokens);
	bool create_cmd(TokenStream& tokens);
	bool update_cmd(TokenStream& tokens);
	bool insert_cmd1(TokenStream& tokens);
	bool insert_cmd2(TokenStream& tokens);
	bool delete_cmd(TokenStream& tokens);

};


#endif
