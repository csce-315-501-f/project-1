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
#define TYPE 5
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
	vector<ParseNode*> parse(string s);

private:

	string* keywords;
	set<string> Keywords;

	void init_values();
	vector<Token> tokenize(string s);
	Statement break_statements(vector<Token>);
	ParseNode* check_grammar(vector<Token> tokens);

	ParseNode* query(TokenStream& tokens);
	ParseNode* expr(TokenStream& tokens);
	ParseNode* atomic_expr(TokenStream& tokens);
	ParseNode* operand(TokenStream& tokens);
	ParseNode* attrib(TokenStream& tokens);
	ParseNode* attrib_list(TokenStream& tokens);
	ParseNode* literal(TokenStream& tokens);
	ParseNode* op(TokenStream& tokens);
	ParseNode* comparison(TokenStream& tokens);
	ParseNode* conjunction(TokenStream& tokens);
	ParseNode* condition(TokenStream& tokens);
	ParseNode* selection(TokenStream& tokens);
	ParseNode* projection(TokenStream& tokens);
	ParseNode* renaming(TokenStream& tokens);
	ParseNode* union_g(TokenStream& tokens);
	ParseNode* difference(TokenStream& tokens);
	ParseNode* product(TokenStream& tokens);

	ParseNode* relation(TokenStream& tokens);
	ParseNode* command(TokenStream& tokens);
	ParseNode* integer(TokenStream& tokens);
	ParseNode* type(TokenStream& tokens);
	ParseNode* typed_list(TokenStream& tokens);
	ParseNode* open_cmd(TokenStream& tokens);
	ParseNode* close_cmd(TokenStream& tokens);
	ParseNode* write_cmd(TokenStream& tokens);
	ParseNode* exit_cmd(TokenStream& tokens);
	ParseNode* show_cmd(TokenStream& tokens);
	ParseNode* create_cmd(TokenStream& tokens);
	ParseNode* update_cmd(TokenStream& tokens);
	ParseNode* insert_cmd1(TokenStream& tokens);
	ParseNode* literal_list(TokenStream& tokens);
	ParseNode* insert_cmd2(TokenStream& tokens);
	ParseNode* delete_cmd(TokenStream& tokens);

};


#endif
