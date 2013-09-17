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
	bool literal = false;		// a state to determine if buffering a literal 
					// ie between quotes "

	// initialize the stringstream and read until characters finish
	ss << s;
	while (ss.good()) {
		c = ss.get();
		
		// print out more information if DEBUGging
		if (DEBUG) {		
			cout << "Char :" << c << endl;
		}

		// if buffering, continue buffering or terminate and determine type of token
		if (buffering)
			if (isalnum(c))
				buffer += c;
			else if (literal && c == '\"') {
				buffering = false;
				literal = false;
				tok.value = buffer;
				tok.type = LITERAL;
				tokens.push_back(tok);
			}
			else if (literal) {
				buffer += c;
			}
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
				else if (c == '\"') {
					buffering = true;
					literal = true;
					buffer = "";
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

/*
 * break_statements()
 * This function breaks a list of tokens into statements, based on the ; operator
 * @param tokens A list of tokens from the tokenizer to break into a list of statements
 * @return a Statement list
 */
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

/*
 * relation()
 * This function determines if a set of tokens represents a relation
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::relation(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN RELATION" << endl;
	Token name = tokens.get();
	if (DEBUG)
		cout << "RELATION NAME: " << name.value << endl;
	if (name.type == IDENTIFIER)
		return true;
	if (name.type != NO_TOKEN) tokens.unget();
	return false;
}

/*
 * open_cmd()
 * This function determines if a set of tokens represents an open command "OPEN relation"
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::open_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN OPEN" << endl;
	int loc = tokens.save();
	Token open = tokens.get();
	if (open.value == "OPEN") {
		bool rel = relation(tokens);
		if (rel) return true;
	}
	tokens.restore(loc);
	return false;
}

/*
 * close_cmd()
 * This function determines if a set of tokens represents an close command "CLOSE relation"
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::close_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN CLOSE" << endl;
	int loc = tokens.save();
	Token close = tokens.get();
	if (close.value == "CLOSE") {
		bool rel = relation(tokens);
		if (rel) return true;
	}
	tokens.restore(loc);
	return false;
}

/*
 * write_cmd()
 * This function determines if a set of tokens represents an write command "WRITE relation"
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::write_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN WRITE" << endl;
	int loc = tokens.save();
	Token write = tokens.get();
	if (write.value == "WRITE") {
		bool rel = relation(tokens);
		if (rel) return true;
	}
	tokens.restore(loc);
	return false;
}

/*
 * exit_cmd()
 * This function determines if a set of tokens represents an exit command "EXIT"
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::exit_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN EXIT" << endl;
	Token exit = tokens.get();
	if (exit.value == "EXIT")
		return true;
	if (exit.type != NO_TOKEN) tokens.unget();
	return false;
}

/*
 * show_cmd()
 * This function determines if a set of tokens represents an show command "SHOW relation"
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::show_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN SHOW" << endl;
	int loc = tokens.save();
	Token show = tokens.get();
	if (show.value == "SHOW") {
		bool ae = atomic_expr(tokens);
		if (ae) return true;
	}
	tokens.restore(loc);
	return false;
}

/*
 * integer()
 * This function determines if a set of tokens represents an integer 
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::integer(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN INT" << endl;
	return literal(tokens);
}

/*
 * type()
 * This function determines if a set of tokens represents a type INTEGER or VARCHAR(n) 
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::type(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN TYPE" << endl;
	int loc = tokens.save();
	Token var = tokens.get();
	if (var.value == "INTEGER")
		return true;
	if (var.value == "VARCHAR") {
		Token open_paren = tokens.get();
		if (open_paren.value == "(") {
			if (integer(tokens)) {
				Token end_paren = tokens.get();
				if (end_paren.value == ")") 
					return true;
			}
		}
	}
	tokens.restore(loc);
}

/*
 * typed_list()
 * This function determines if a set of tokens represents a type list
 * name type { , typed_list } 
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::typed_list(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN TYPED LIST" << endl;
	int loc = tokens.save();
	bool attr = attrib(tokens);
	if (!attr)
		return false;
	bool t = type(tokens);
	if (!t) {
		tokens.restore(loc);
		return false;
	}
	Token comma = tokens.get();
	if (comma.value == ",") {
		bool al =  typed_list(tokens); // changed type to typed_list
		return (al);
	}
	if (comma.type != NO_TOKEN) tokens.unget();
	return true;
}

/*
 * create_cmd()
 * This function determines if a set of tokens represents a create command
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::create_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN CREATE" << endl;
	int loc = tokens.save();
	Token create = tokens.get();
	if (create.value != "CREATE") {
		tokens.restore(loc);
		return false;
	}
	Token table = tokens.get();
	if (table.value != "TABLE") {
		tokens.restore(loc);
		return false;
	}
	bool rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return false;
	}
	Token open_paren = tokens.get();
	if (open_paren.value != "(") {
		tokens.restore(loc);
		return false;
	}
	bool tl = typed_list(tokens);
	if (!tl) {
		tokens.restore(loc);
		return false;
	}
	Token end_paren = tokens.get();
	if (end_paren.value != ")") {
		tokens.restore(loc);
		return false;
	}
	Token prim = tokens.get();
	if (prim.value != "PRIMARY") {
		tokens.restore(loc);
		return false;
	}
	Token key = tokens.get();
	if (key.value != "KEY") {
		tokens.restore(loc);
		return false;
	}
	Token open_paren2 = tokens.get();
	if (open_paren2.value != "(") {
		tokens.restore(loc);
		return false;
	}
	bool al = attrib_list(tokens);
	if (!al) {
		tokens.restore(loc);
		return false;
	}
	Token end_paren2 = tokens.get();
	if (end_paren2.value != ")") {
		tokens.restore(loc);
		return false;
	}
	return true;
}

/*
 * update_cmd()
 * This function determines if a set of tokens represents an update command
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::update_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN UPDATE" << endl;
	int loc = tokens.save();
	Token update = tokens.get();
	if (update.value != "UPDATE") {
		tokens.restore(loc);
		return false;
	}
	bool rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return false;
	}
	Token set = tokens.get();
	if (set.value != "SET") {
		tokens.restore(loc);
		return false;
	}
	bool at = attrib(tokens);
	if (!at) {
		tokens.restore(loc);
		return false;
	}
	Token equal = tokens.get();
	if (equal.value != "=") {
		tokens.restore(loc);
		return false;
	}
	bool lit = literal(tokens);
	if (!lit) {
		tokens.restore(loc);
		return false;
	}
	Token where = tokens.get();
	if (where.value != "WHERE") {
		tokens.restore(loc);
		return false;
	}
	bool cond = condition(tokens);
	if (!cond) {
		tokens.restore(loc);
		return false;
	}
	return true;
}

/*
 * insert_cmd()
 * This function determines if a set of tokens represents an insert command
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::insert_cmd1(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN INSERT1" << endl;
	int loc = tokens.save();
	Token ins = tokens.get();
	if (ins.value != "INSERT") {
		tokens.restore(loc);
		return false;
	}
	Token into = tokens.get();
	if (into.value != "INTO") {
		tokens.restore(loc);
		return false;
	}
	bool rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return false;
	}
	Token vals = tokens.get();
	if (vals.value != "VALUES") {
		tokens.restore(loc);
		return false;
	}
	Token from = tokens.get();
	if (from.value != "FROM") {
		tokens.restore(loc);
		return false;
	}
	Token rela = tokens.get();
	if (rela.value != "RELATION") {
		tokens.restore(loc);
		return false;
	}
	bool exp = expr(tokens);
	if (!exp) {
		tokens.restore(loc);
		return false;
	}
	return true;
}
bool Parser::insert_cmd2(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN INSERT2" << endl;
	int loc = tokens.save();
	Token ins = tokens.get();
	if (ins.value != "INSERT") {
		tokens.restore(loc);
		return false;
	}
	Token into = tokens.get();
	if (into.value != "INTO") {
		tokens.restore(loc);
		return false;
	}
	bool rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return false;
	}
	Token vals = tokens.get();
	if (vals.value != "VALUES") {
		tokens.restore(loc);
		return false;
	}
	Token from = tokens.get();
	if (from.value != "FROM") {
		tokens.restore(loc);
		return false;
	}
	Token rela = tokens.get();
	if (rela.value != "RELATION") {
		tokens.restore(loc);
		return false;
	}
	bool exp = expr(tokens);
	if (!exp) {
		tokens.restore(loc);
		return false;
	}
	return true;
}

/*
 * delete_cmd()
 * This function determines if a set of tokens represents a delete command
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::delete_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN DELETE" << endl;
	int loc = tokens.save();
	Token del = tokens.get();
	if (del.value != "DELETE") {
		tokens.restore(loc);
		return false;
	}
	Token from = tokens.get();
	if (from.value != "FROM") {
		tokens.restore(loc);
		return false;
	}
	bool rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return false;
	}
	Token where = tokens.get();
	if (where.value != "WHERE") {
		tokens.restore(loc);
		return false;
	}
	bool cond = condition(tokens);
	if (!cond) {
		tokens.restore(loc);
		return false;
	}
	return true;
}

/*
 * command()
 * This function determines if a set of tokens represents a command
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::command(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN COMMAND" << endl;
	int loc = tokens.save();
	return	open_cmd(tokens) ||
		close_cmd(tokens.restore(loc)) ||
		write_cmd(tokens.restore(loc)) ||
		exit_cmd(tokens.restore(loc)) ||
		show_cmd(tokens.restore(loc)) ||
		create_cmd(tokens.restore(loc)) ||
		update_cmd(tokens.restore(loc)) ||
		insert_cmd1(tokens.restore(loc)) ||
		insert_cmd2(tokens.restore(loc)) ||
		delete_cmd(tokens.restore(loc));
}

/*
 * atomic_expr()
 * This function determines if a set of tokens represents an atomic expression
 * relation | (expr)
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::atomic_expr(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN ATOMIC EXPR" << endl;
	int loc = tokens.save();
	if (relation(tokens))
		return true;
	Token next = tokens.get();
	if (next.value == "(") {
		bool exp = expr(tokens);
		if (exp) {
			Token end_paren = tokens.get();
			if (end_paren.value == ")")
				return true;
		}
	}
	tokens.restore(loc);
	return false;
}

/*
 * op()
 * This function determines if a set of tokens represents an operator
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::op(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN OP" << endl;
	Token oprnd = tokens.get();
	if (	oprnd.value == "=" ||
		oprnd.value == "==" ||
		oprnd.value == "!=" ||
		oprnd.value == "<=" ||
		oprnd.value == ">=" ||
		oprnd.value == "<" ||
		oprnd.value == ">") 
		return true;
	if (oprnd.type != NO_TOKEN) tokens.unget();
	return false;
}

/*
 * attrib()
 * This function determines if a set of tokens represents an attribute
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::attrib(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN ATRIB" << endl;
	Token name = tokens.get();
	if (name.type == IDENTIFIER)
		return true;
	if (name.type != NO_TOKEN) tokens.unget();
	return false;
}

/*
 * literal()
 * This function determines if a set of tokens represents a literal number
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::literal(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN LITERAL" << endl;
	Token name = tokens.get();
	if (DEBUG)
		cout << "LITERAL TYPE " << name.type << endl;
	if (name.type == NUMBER || name.type == LITERAL)
		return true;
	if (name.type != NO_TOKEN) tokens.unget();
	return false;
}

/*
 * operand()
 * This function determines if a set of tokens represents an operand
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::operand(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN OPERAND" << endl;
	int loc = tokens.save();
	return attrib(tokens) || literal(tokens.restore(loc));
}

/*
 * comparison()
 * This function determines if a set of tokens represents a comparison
 * operand op operand | ( condition )
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::comparison(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN COMP" << endl;
	int loc = tokens.save();
	bool choice_1 =  (operand(tokens) && op(tokens) && operand(tokens));
	if (DEBUG)
		cout << "COMPARISON OPERATION " << choice_1 << endl;
	if (choice_1)
		return true;
	tokens.restore(loc);
	Token open_paren = tokens.get();
	if (open_paren.value == "(") {
		bool exp = condition(tokens);
		if (exp) {
			Token end_paren = tokens.get();
			if (end_paren.value == ")")
				return true;
		}
	}
	tokens.restore(loc);
	return false;
}

/*
 * comparison()
 * This function determines if a set of tokens represents a conjunction
 * comparison {&& comparison}
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::conjunction(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN CONJ" << endl;
	bool comp = comparison(tokens);
	if (!comp)
		return false;
	Token andand = tokens.get();
	if (andand.value == "&&") {
		bool cmp =  comparison(tokens);
		return (cmp);
	}
	if (andand.type != NO_TOKEN) tokens.unget();
	return true;
}

/*
 * comparison()
 * This function determines if a set of tokens represents a condition
 * condition {&& condition}
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::condition(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN CONDITION" << endl;
	bool conj = conjunction(tokens);
	if (!conj)
		return false;
	Token oror = tokens.get();
	if (oror.value == "||") {
		bool cmp = conjunction(tokens);
		return (cmp);
	}
	if (oror.type != NO_TOKEN) tokens.unget();
	return true;
}

/*
 * selection()
 * This function determines if a set of tokens represents a select statement
 * SELECT (condition) relation
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::selection(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN SELECT" << endl;
	int loc = tokens.save();
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
			}
		}
	}
	tokens.restore(loc);
	return false;
}

/*
 * attrib_list()
 * This function determines if a set of tokens represents a attribute list
 * attrib {, attrib_list}
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::attrib_list(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN ATRIBLIST" << endl;
	bool attr = attrib(tokens);
	if (!attr)
		return false;
	Token comma = tokens.get();
	if (comma.value == ",") {
		bool al =  attrib_list(tokens); // changed attrib to attrib_list
		return (al);
	}
	if (comma.type != NO_TOKEN) tokens.unget();
	return true;
}

/*
 * projection()
 * This function determines if a set of tokens represents a projection statement
 * PROJECT (attrib_list) atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::projection(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN PROJECT" << endl;
	int loc = tokens.save();
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
			}
		}
	}
	tokens.restore(loc);
	return false;
}

/*
 * renaming()
 * This function determines if a set of tokens represents a renaming statement
 * RENAME (attrib_list) atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::renaming(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN RENAME" << endl;
	int loc = tokens.save();
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
			}
		}
	}
	tokens.restore(loc);
	return false;
}

/*
 * union_g()
 * This function determines if a set of tokens represents a union statement
 * atomic_expr + atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::union_g(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN UNION" << endl;
	int loc = tokens.save();
	bool at_expr = atomic_expr(tokens);
	if (!at_expr) {
		tokens.restore(loc);
		return false;
	}
	Token plus = tokens.get();
	if (plus.value == "+") {
		bool ae = atomic_expr(tokens);
		if (ae) return true;
	}
	tokens.restore(loc);
	return false;
}

/*
 * difference()
 * This function determines if a set of tokens represents a difference statement
 * atomic_expr - atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::difference(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN DIFFERENCE" << endl;
	int loc = tokens.save();
	bool at_expr = atomic_expr(tokens);
	if (!at_expr) {
		tokens.restore(loc);
		return false;
	}
	Token minus = tokens.get();
	if (minus.value == "-") {
		bool ae = atomic_expr(tokens);
		if (ae) return true;
	}
	tokens.restore(loc);
	return false;
}

/*
 * product()
 * This function determines if a set of tokens represents a product statement
 * atomic_expr * atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::product(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN PRODUCT" << endl;
	int loc = tokens.save();
	bool at_expr = atomic_expr(tokens);
	if (!at_expr) {
		tokens.restore(loc);
		return false;
	}
	Token times = tokens.get();
	if (times.value == "*") {
		bool ae = atomic_expr(tokens);
		if (ae) return true;
	}
	tokens.restore(loc);
	return false;
}

/*
 * expr()
 * This function determines if a set of tokens represents a expression
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::expr(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN EXPR" << endl;
	int loc = tokens.save();
	return selection(tokens)||
		projection(tokens.restore(loc)) ||
		renaming(tokens.restore(loc)) ||
		union_g(tokens.restore(loc)) ||
		difference(tokens.restore(loc)) ||
		product(tokens.restore(loc)) ||
		atomic_expr(tokens.restore(loc));
}

/*
 * query()
 * This function determines if a set of tokens represents a query
 * relation <- expr
 * @param tokens A TokenStream object already broken into one statement
 */
bool Parser::query(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN QUERY" << endl;
	int loc = tokens.save();
	if (relation(tokens)) {
		Token arrow = tokens.get();
		if (arrow.value != "<-") {
			tokens.restore(loc);
			return false;
		}
		return expr(tokens);
	}
	tokens.restore(loc);
	return false;
}

/*
 * check_grammar()
 * This function determines if a set of tokens has proper grammar
 * @param tokens A vector of tokens already broken into one statement
 */
bool Parser::check_grammar(vector<Token> tokens) {
	TokenStream tok(tokens);
	bool result;
	result = query(tok) || command(tok);
	if (DEBUG)
		cout << "RESULT: " << result << endl;
	Token extra = tok.get();
	if (DEBUG)
		cout << "EXTRA: " << extra.value << endl;
	if (extra.type == NO_TOKEN)
		return result;
	else return false;
}

/*
 * parse()
 * This function tokenizes a string and breaks it into statements then checks their grammar
 * individually for correct syntax in the SQL language
 * @param tokens A string representing the SQL language
 */
vector<string> Parser::parse(string s) {
	string info = "";
	stringstream ss;
	ss << s;
	string line;
	while (getline(ss,line))
		info += line;
	vector<Token> tokens = tokenize(info);
	Statement statements = break_statements(tokens);
	for (int i = 0; i < statements.size(); ++i) {
		cout << "STATEMENT " << i+1 << ":"<< endl;
		for (int j = 0; j < statements[i].size(); ++j) {
			cout << statements[i][j].value << " " << flush;
		}
		cout << ";" << endl;
		cout << "GRAMMAR " << flush << (check_grammar(statements[i])?"GOOD":"BAD") << endl << endl;
	}
	vector<string> temp;
	return temp;
}
