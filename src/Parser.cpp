#include "Parser.h"
#include <iostream>


/*
 * init_values()
 * This function stores the keyword information for the parser to distinguish
 */
#define NUM_WORDS 23
void Parser::init_values() {
	string _keywords[NUM_WORDS] = {
		"SELECT",
		"PROJECT",
		"RENAME",
		"OPEN",
		"CLOSE",
		"WRITE",
		"EXIT",
		"SHOW",
		"CREATE",
		"TABLE",
		"UPDATE",
		"SET",
		"PRIMARY",
		"KEY",
		"WHERE",
		"INSERT"
		"INTO",
		"VALUES",
		"FROM",
		"RELATION",
		"DELETE",
		"VARCHAR",
		"INTEGER"
	};
	keywords = _keywords;
	Keywords = set<string> (keywords,keywords+NUM_WORDS);
}

/*
 * to_string()
 * This function converts an integer into its string representation using stringstreams
 */
string to_string(int number) {
	stringstream ss;
	ss << number;
	return ss.str();
}

/*
 * tokenize()
 * This function converts a string into its basic parts based on the SQL grammar
 */
vector<Token> Parser::tokenize(string s) {

	// declare initial variables
	vector<Token> tokens;		// the final vector of tokens
	stringstream ss;		// stringstream to parse the data by char
	char c;				// a temporary char used to hold information
	char d;					
	Token tok;			// a temporary token based on the current data
	string buffer;			// a string used to hold more than one char data
	bool buffering = false;		// a state to determine when to read in more info

	// initialize the stringstream and read until characters finish
	ss << s;
	while (ss.good()) {
		c = ss.get();
		
		// print out more information if debugging
		if (DEBUG) {		
			cout << "Char :" << c << endl;
		}

		// if buffering, continue buffering or terminate and determine type of token
		if (buffering)
			if (isalnum(c))
				buffer += c;	
			else {
				ss.unget();
				buffering = false;
				tok.value = buffer;
				if (Keywords.find(buffer) != Keywords.end())
					tok.type = KEYWORD;
				else
					tok.type = IDENTIFIER;
				tokens.push_back(tok);
			}
		else {
			// if first character is a letter, look for more characters
			if (isalpha(c)) {
				buffering = true;
				buffer = c;
			}
			// look for symbol operators == != <= >= < > = <-
			else if (c == '=' || c == '!' || c == '<' || c == '>') {
				d = ss.get();
				// two character operators == != <= >= <-
				if (d == '=') {
					tok.value = c;
					tok.value += d;
					tok.type = OPERATOR;
					tokens.push_back(tok);
				}
				else if (c == '<' && d == '-'){
					tok.value = "<-";
					tok.type = OPERATOR;
					tokens.push_back(tok);

				}
				// one character operator doesn't exist case
				else if (c == '!') {
					tok.value = c;
					tok.type = PARSE_ERROR;
					tokens.push_back(tok);
					break;
				}
				// one character operators < > =
				else {
					ss.unget();
					tok.value = c;
					tok.type = OPERATOR;
					tokens.push_back(tok);	
				}
			}
			// look for symbol operators ( ) ; , " + - *
			else if (c == '(' || c == ')' || c == ';' || c == ',' || c == '\"' ||
				c == '+' || c == '-' || c == '*' || c == '|' || c == '&') {
				if (c == '|') {
					d = ss.get();
					if (d == '|') {
						tok.value = "||";
						tok.type = OPERATOR;
						tokens.push_back(tok);
					}
					else {
						ss.unget();
						tok.value = d;
						tok.type = PARSE_ERROR;
						tokens.push_back(tok);
						break;
					}
				}
				else if (c == '&') {
					d = ss.get();
					if (d == '&') {
						tok.value = "&&";
						tok.type = OPERATOR;
						tokens.push_back(tok);
					}
					else {
						ss.unget();
						tok.value = d;
						tok.type = PARSE_ERROR;
						tokens.push_back(tok);
						break;
					}
				}
				else {
					tok.value = c;
					tok.type = OPERATOR;
					tokens.push_back(tok);
				}
			}
			// look for numbers
			else if (isdigit(c)) {
				ss.unget();
				int num;
				ss >> num;
				tok.value = to_string(num);
				tok.type = NUMBER;
				tokens.push_back(tok);
			}
			// remove random whitespace
			else if (isblank(c)) {
				continue;
			}
			// unknown character encountered, exit
			else {
				tok.value = c;
				tok.type = PARSE_ERROR;
				tokens.push_back (tok);
				break;	
			}
		}
			
	}
	return tokens;	
}

Statement Parser::break_statements(vector<Token> tokens) {

	Statement statements;
	vector<Token> temp;
	for(int i = 0; i < tokens.size(); ++i) {
		if (tokens[i].value == ";" && tokens[i].type == OPERATOR) {
			statements.push_back(temp);
			temp.clear();
		}
		else {
			temp.push_back(tokens[i]);
		}
	}

	// if temp is not empty then expected EOF
	return statements;
}

bool Parser::relation(TokenStream& tokens) {
	Token name = tokens.get();
	if (name.type == IDENTIFIER)
		return true;
	tokens.unget();
	return false;
}

bool Parser::open_cmd(TokenStream& tokens) {
	Token open = tokens.get();
	if (open.value == "OPEN") {
		bool rel = relation(tokens);
		if (rel) return true;
	}
	tokens.unget();
	return false;
}

bool Parser::close_cmd(TokenStream& tokens) {
	Token close = tokens.get();
	if (close.value == "CLOSE") {
		bool rel = relation(tokens);
		if (rel) return true;
	}
	tokens.unget();
	return false;
}

bool Parser::write_cmd(TokenStream& tokens) {
	Token write = tokens.get();
	if (write.value == "WRITE") {
		bool rel = relation(tokens);
		if (rel) return true;
	}
	tokens.unget();
	return false;
}

bool Parser::exit_cmd(TokenStream& tokens) {
	Token exit = tokens.get();
	if (exit.value == "EXIT")
		return true;
	tokens.unget();
	return false;
}

bool Parser::show_cmd(TokenStream& tokens) {
	Token show = tokens.get();
	if (show.value == "SHOW") {
		bool ae =  atomic_expr(tokens);
		if (ae) return true;
	}
	tokens.unget();
	return false;
}


bool Parser::create_cmd(TokenStream& tokens) {
	Token create = tokens.get();
	if (create.value != "CREATE") {
		tokens.unget();
		return false;
	}
	Token table = tokens.get();
	if (create.value != "TABLE") {
		tokens.unget();
		tokens.unget();
		return false;
	}
	return true;
}

bool Parser::update_cmd(TokenStream& tokens) {
	return false;
	Token create = tokens.get();
	if (create.value != "CREATE") {
		tokens.unget();
		return false;
	}
	Token table = tokens.get();
	if (create.value != "TABLE") {
		tokens.unget();
		tokens.unget();
		return false;
	}
	Token relation_name = tokens.get();
	if (relation_name.type == IDENTIFIER)
		return true;
	tokens.unget();
	return false;
}

bool Parser::insert_cmd(TokenStream& tokens) {
	return false;
	Token create = tokens.get();
	if (create.value != "CREATE") {
		tokens.unget();
		return false;
	}
	Token table = tokens.get();
	if (create.value != "TABLE") {
		tokens.unget();
		tokens.unget();
		return false;
	}
	Token relation_name = tokens.get();
	if (relation_name.type == IDENTIFIER)
		return true;
	tokens.unget();
	return false;
}

bool Parser::delete_cmd(TokenStream& tokens) {
	return false;
	Token create = tokens.get();
	if (create.value != "CREATE") {
		tokens.unget();
		return false;
	}
	Token table = tokens.get();
	if (create.value != "TABLE") {
		tokens.unget();
		tokens.unget();
		return false;
	}
	Token relation_name = tokens.get();
	if (relation_name.type == IDENTIFIER)
		return true;
	tokens.unget();
	return false;
}


bool Parser::command(TokenStream& tokens) {
	return	open_cmd(tokens) ||
		close_cmd(tokens) ||
		write_cmd(tokens) ||
		exit_cmd(tokens) ||
		show_cmd(tokens) ||
		create_cmd(tokens) ||
		update_cmd(tokens) ||
		insert_cmd(tokens) ||
		delete_cmd(tokens);
}

bool Parser::atomic_expr(TokenStream& tokens) {
	if (relation(tokens))
		return true;
	Token next = tokens.get();
	if (next.value == "(") {
		bool exp = expr(tokens);
		if (exp) {
			Token end_paren = tokens.get();
			if (end_paren.value == ")")
				return true;
			tokens.unget();
		}
	}
	tokens.unget();
	return false;
}


bool Parser::op(TokenStream& tokens) {
	Token oprnd = tokens.get();
	if (	oprnd.value == "=" ||
		oprnd.value == "==" ||
		oprnd.value == "!=" ||
		oprnd.value == "<=" ||
		oprnd.value == ">=" ||
		oprnd.value == "<" ||
		oprnd.value == ">") 
		return true;
	tokens.unget();
	return false;
}

bool Parser::attrib(TokenStream& tokens) {
	Token name = tokens.get();
	if (name.type == IDENTIFIER)
		return true;
	tokens.unget();
	return false;
}

bool Parser::literal(TokenStream& tokens) {
	Token name = tokens.get();
	if (name.type == NUMBER)
		return true;
	tokens.unget();
	return false;
}

bool Parser::operand(TokenStream& tokens) {
	return attrib(tokens) || literal(tokens);
}

bool Parser::comparison(TokenStream& tokens) {
	bool choice_1 =  (operand(tokens) && op(tokens) && operand(tokens));
	if (choice_1)
		return true;
	Token open_paren = tokens.get();
	if (open_paren.value == "(") {
		bool exp = condition(tokens);
		if (exp) {
			Token end_paren = tokens.get();
			if (end_paren.value == ")")
				return true;
			tokens.unget();
			tokens.unget();
			return false;
		}
	}
	tokens.unget();
	return true;
}

bool Parser::conjunction(TokenStream& tokens) {
	bool comp = comparison(tokens);
	if (!comp)
		return false;
	Token andand = tokens.get();
	if (andand.value == "&&") {
		bool cmp =  comparison(tokens);
		if (cmp) return true;
	}
	tokens.unget();
	return true;
}

bool Parser::condition(TokenStream& tokens) {
	bool conj = conjunction(tokens);
	if (!conj)
		return false;
	Token oror = tokens.get();
	if (oror.value == "||") {
		bool cmp = conjunction(tokens);
		if (cmp) return true;
	}
	tokens.unget();
	return true;
}

bool Parser::selection(TokenStream& tokens) {
	Token select = tokens.get();
	if (select.value == "SELECT") {
		Token open_paren = tokens.get();
		if (open_paren.value == "(") {
			bool cond = condition(tokens);
			if (cond) {
				Token end_paren = tokens.get();
				if (end_paren.value == ")") {
					bool ae = atomic_expr(tokens);
					if (ae) return true;
				}
				tokens.unget();
			}
			else {
				tokens.unget();
				tokens.unget();
				return false;
			}
		}
		tokens.unget();
	}
	tokens.unget();
	return false;

}

bool Parser::attrib_list(TokenStream& tokens) {
	bool attr = attrib(tokens);
	if (!attr)
		return false;
	Token comma = tokens.get();
	if (comma.value == ",") {
		bool al =  attrib_list(tokens); // changed attrib to attrib_list
		if (al) return true;
	}
	tokens.unget();
	return true;
}

bool Parser::projection(TokenStream& tokens) {
	Token pro = tokens.get();
	if (pro.value == "PROJECT") {
		Token open_paren = tokens.get();
		if (open_paren.value == "(") {
			bool cond = attrib_list(tokens);
			if (cond) {
				Token end_paren = tokens.get();
				if (end_paren.value == ")") {
					bool ae =  atomic_expr(tokens);
					if (ae) return true;
				}
				tokens.unget();
			}
			else {
				tokens.unget();
				tokens.unget();
				return false;
			}
		}
		tokens.unget();
	}
	tokens.unget();
	return false;
}


bool Parser::renaming(TokenStream& tokens) {
	Token rename = tokens.get();
	if (rename.value == "RENAME") {
		Token open_paren = tokens.get();
		if (open_paren.value == "(") {
			bool cond = attrib_list(tokens);
			if (cond) {
				Token end_paren = tokens.get();
				if (end_paren.value == ")") {
					bool ae = atomic_expr(tokens);
					if (ae) return true;
				}
				tokens.unget();
			}
			else {
				tokens.unget();
				tokens.unget();
				return false;
			}
		}
		tokens.unget();
	}
	tokens.unget();
	return false;
}


bool Parser::union_g(TokenStream& tokens) {
	bool at_expr = atomic_expr(tokens);
	Token plus = tokens.get();
	if (plus.value == "+") {
		bool ae = atomic_expr(tokens);
		if (ae) return true;
	}
	tokens.unget();
	return false;
}

bool Parser::difference(TokenStream& tokens) {
	bool at_expr = atomic_expr(tokens);
	Token minus = tokens.get();
	if (minus.value == "-") {
		bool ae = atomic_expr(tokens);
		if (ae) return true;
	}
	tokens.unget();
	return false;
}

bool Parser::product(TokenStream& tokens) {
	bool at_expr = atomic_expr(tokens);
	Token times = tokens.get();
	if (times.value == "*") {
		bool ae = atomic_expr(tokens);
		if (ae) return true;
	}
	tokens.unget();
	return false;
}

bool Parser::expr(TokenStream& tokens) {
	return selection(tokens)||
		projection(tokens) ||
		renaming(tokens) ||
		union_g(tokens) ||
		difference(tokens) ||
		product(tokens) ||
		atomic_expr(tokens);
}

bool Parser::query(TokenStream& tokens) {
	if (relation(tokens)) {
		Token arrow = tokens.get();
		if (arrow.value != "<-") {
			tokens.unget();
			return false;
		}
		return expr(tokens);
	}
	return false;
}

bool Parser::check_grammar(vector<Token> tokens) {
	TokenStream tok(tokens);
	bool result = query(tok) || command(tok);
	Token extra = tok.get();
	cout << "EXTRA " << extra.value<<extra.type << endl;
	if (extra.type == NO_TOKEN)
		return result;
	else return false;
}

vector<string> Parser::parse(string s) {
	vector<Token> tokens = tokenize(s);
	Statement statements = break_statements(tokens);
	for (int i = 0; i < statements.size(); ++i) {
		cout << "STATEMENT " << i << endl;
		for (int j = 0; j < statements[i].size(); ++j)
			cout << "TOKEN " << statements[i][j].value << " " << statements[i][j].type << endl;
		cout << "GRAMMAR " << flush << (check_grammar(statements[i])?"GOOD":"BAD") << endl << endl;
	}
	vector<string> temp;
	return temp;
}
