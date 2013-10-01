#include "Parse.h"
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
ParseNode* Parser::relation(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN RELATION" << endl;
	Token name = tokens.get();
	if (DEBUG)
		cout << "RELATION NAME: " << name.value << endl;
	if (name.type == IDENTIFIER)
		return new ParseNode(name.value,IDENTIFIER);
	if (name.type != NO_TOKEN) tokens.unget();
	return NULL;
}

/*
 * open_cmd()
 * This function determines if a set of tokens represents an open command "OPEN relation"
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::open_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN OPEN" << endl;
	int loc = tokens.save();
	Token open = tokens.get();
	if (open.value == "OPEN") {
		ParseNode* rel = relation(tokens);
		if (rel) {
			ParseNode* ope = new ParseNode("OPEN",KEYWORD);
			ope->children.push_back(rel);
			return ope;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * close_cmd()
 * This function determines if a set of tokens represents an close command "CLOSE relation"
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::close_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN CLOSE" << endl;
	int loc = tokens.save();
	Token close = tokens.get();
	if (close.value == "CLOSE") {
		ParseNode* rel = relation(tokens);
		if (rel) {
			ParseNode* clo = new ParseNode("CLOSE",KEYWORD);
			clo->children.push_back(rel);
			return clo;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * write_cmd()
 * This function determines if a set of tokens represents an write command "WRITE relation"
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::write_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN WRITE" << endl;
	int loc = tokens.save();
	Token write = tokens.get();
	if (write.value == "WRITE") {
		ParseNode* rel = relation(tokens);
		if (rel) {
			ParseNode* wri = new ParseNode("WRITE",KEYWORD);
			wri->children.push_back(rel);
			return wri;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * exit_cmd()
 * This function determines if a set of tokens represents an exit command "EXIT"
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::exit_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN EXIT" << endl;
	Token exit = tokens.get();
	if (exit.value == "EXIT")
		return new ParseNode("EXIT",KEYWORD);
	if (exit.type != NO_TOKEN) tokens.unget();
	return NULL;
}

/*
 * show_cmd()
 * This function determines if a set of tokens represents an show command "SHOW relation"
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::show_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN SHOW" << endl;
	int loc = tokens.save();
	Token show = tokens.get();
	if (show.value == "SHOW") {
		ParseNode* ae = atomic_expr(tokens);
		if (ae) {
			ParseNode* sh = new ParseNode("SHOW",KEYWORD);
			sh->children.push_back(ae);
			return sh;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * integer()
 * This function determines if a set of tokens represents an integer 
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::integer(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN INT" << endl;
	return literal(tokens);
}

/*
 * type()
 * This function determines if a set of tokens represents a type INTEGER or VARCHAR(n) 
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::type(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN TYPE" << endl;
	int loc = tokens.save();
	Token var = tokens.get();
	if (var.value == "INTEGER")
		return new ParseNode("INTEGER",TYPE);
	if (var.value == "VARCHAR") {
		Token open_paren = tokens.get();
		if (open_paren.value == "(") {
			ParseNode* inte = integer(tokens);
			if (inte) {
				Token end_paren = tokens.get();
				if (end_paren.value == ")") 
					return inte;
			}
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * typed_list()
 * This function determines if a set of tokens represents a type list
 * name type { , typed_list } 
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::typed_list(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN TYPED LIST" << endl;
	int loc = tokens.save();
	ParseNode* attr = attrib(tokens);
	if (!attr) 
		return NULL;
	ParseNode* t = type(tokens);
	if (!t) {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* type_set = new ParseNode("TYPE SET",KEYWORD);
	type_set->children.push_back(attr);
	type_set->children.push_back(t);
	ParseNode* attr_list = new ParseNode("TYPED LIST",KEYWORD);
	attr_list->children.push_back(type_set);
	Token comma = tokens.get();
	if (comma.value == ",") {
		ParseNode* al =  typed_list(tokens); // changed type to typed_list
		if (!al) {
			tokens.restore(loc);
			delete attr_list;
			delete type_set;
			return NULL;
		}
		for (int i = 0; i < al->children.size(); ++i)
			attr_list->children.push_back(al->children[i]);
		return attr_list;
	}
	if (comma.type != NO_TOKEN) tokens.unget();
	return attr_list;
}

/*
 * create_cmd()
 * This function determines if a set of tokens represents a create command
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::create_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN CREATE" << endl;
	int loc = tokens.save();
	Token create = tokens.get();
	if (create.value != "CREATE") {
		tokens.restore(loc);
		return NULL;
	}
	Token table = tokens.get();
	if (table.value != "TABLE") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return NULL;
	}
	Token open_paren = tokens.get();
	if (open_paren.value != "(") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* tl = typed_list(tokens);
	if (!tl) {
		tokens.restore(loc);
		return NULL;
	}
	Token end_paren = tokens.get();
	if (end_paren.value != ")") {
		tokens.restore(loc);
		return NULL;
	}
	Token prim = tokens.get();
	if (prim.value != "PRIMARY") {
		tokens.restore(loc);
		return NULL;
	}
	Token key = tokens.get();
	if (key.value != "KEY") {
		tokens.restore(loc);
		return NULL;
	}
	Token open_paren2 = tokens.get();
	if (open_paren2.value != "(") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* al = attrib_list(tokens);
	if (!al) {
		tokens.restore(loc);
		return NULL;
	}
	Token end_paren2 = tokens.get();
	if (end_paren2.value != ")") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* cre = new ParseNode("CREATE",KEYWORD);
	cre->children.push_back(rel);
	cre->children.push_back(tl);
	cre->children.push_back(al);
	return cre;
}

/*
 * update_cmd()
 * This function determines if a set of tokens represents an update command
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::update_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN UPDATE" << endl;
	int loc = tokens.save();
	Token update = tokens.get();
	if (update.value != "UPDATE") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return NULL;
	}
	Token set = tokens.get();
	if (set.value != "SET") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* at = attrib(tokens);
	if (!at) {
		tokens.restore(loc);
		return NULL;
	}
	Token equal = tokens.get();
	if (equal.value != "=") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* lit = literal(tokens);
	if (!lit) {
		tokens.restore(loc);
		return NULL;
	}
	Token where = tokens.get();
	if (where.value != "WHERE") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* cond = condition(tokens);
	if (!cond) {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* upd = new ParseNode("UPDATE",KEYWORD);
	upd->children.push_back(rel);
	upd->children.push_back(at);
	upd->children.push_back(lit);
	upd->children.push_back(cond);
	return upd;
}

/*
 * insert_cmd1()
 * This function determines if a set of tokens represents an insert command
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::insert_cmd1(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN INSERT1" << endl;
	int loc = tokens.save();
	Token ins = tokens.get();
	if (ins.value != "INSERT") {
		tokens.restore(loc);
		return NULL;
	}
	Token into = tokens.get();
	if (into.value != "INTO") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return NULL;
	}
	Token vals = tokens.get();
	if (vals.value != "VALUES") {
		tokens.restore(loc);
		return NULL;
	}
	Token from = tokens.get();
	if (from.value != "FROM") {
		tokens.restore(loc);
		return NULL;
	}
	Token rela = tokens.get();
	if (rela.value != "RELATION") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* exp = expr(tokens);
	if (!exp) {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* inse = new ParseNode("INSERT",KEYWORD);
	inse->children.push_back(rel);
	inse->children.push_back(exp);
	return inse;
}
/*
 * literal_list()
 * This function determines if a set of tokens represents a attribute list
 * attrib {, attrib_list}
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::literal_list(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN LITERAL LIST" << endl;
	int loc = tokens.save();
	ParseNode* lits = literal(tokens);
	if (!lits)
		return NULL;
	ParseNode* lit_list = new ParseNode("LITERAL LIST", KEYWORD);
	lit_list->children.push_back(lits);
	Token comma = tokens.get();
	if (comma.value == ",") {
		ParseNode* ll = literal_list(tokens); // changed attrib to attrib_list
		if (!ll) {
			tokens.restore(loc);
			delete lit_list;
			return NULL;
		}
		for (int i = 0; i < ll->children.size(); ++i)
			lit_list->children.push_back(ll->children[i]);
		return lit_list;
	}
	if (comma.type != NO_TOKEN) tokens.unget();
	return lit_list;
}


/*
 * insert_cmd2()
 * This function determines if a set of tokens represents an insert command
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::insert_cmd2(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN INSERT2" << endl;
	int loc = tokens.save();
	Token ins = tokens.get();
	if (ins.value != "INSERT") {
		tokens.restore(loc);
		return NULL;
	}
	Token into = tokens.get();
	if (into.value != "INTO") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return NULL;
	}
	Token vals = tokens.get();
	if (vals.value != "VALUES") {
		tokens.restore(loc);
		return NULL;
	}
	Token from = tokens.get();
	if (from.value != "FROM") {
		tokens.restore(loc);
		return NULL;
	}
	Token open_paren = tokens.get();
	if (open_paren.value != "(") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* lits = literal_list(tokens);
	if (!lits) {
		tokens.restore(loc);
		return NULL;
	}
	Token end_paren = tokens.get();
	if (end_paren.value != ")") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* inse = new ParseNode("INSERT",KEYWORD);
	inse->children.push_back(rel);
	inse->children.push_back(lits);
	return inse;
}

/*
 * delete_cmd()
 * This function determines if a set of tokens represents a delete command
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::delete_cmd(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN DELETE" << endl;
	int loc = tokens.save();
	Token del = tokens.get();
	if (del.value != "DELETE") {
		tokens.restore(loc);
		return NULL;
	}
	Token from = tokens.get();
	if (from.value != "FROM") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* rel = relation(tokens);
	if (!rel) {
		tokens.restore(loc);
		return NULL;
	}
	Token where = tokens.get();
	if (where.value != "WHERE") {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* cond = condition(tokens);
	if (!cond) {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* dele = new ParseNode("DELETE",KEYWORD);
	dele->children.push_back(rel);
	dele->children.push_back(cond);
	return dele;
}

/*
 * command()
 * This function determines if a set of tokens represents a command
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::command(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN COMMAND" << endl;
	int loc = tokens.save();
	if (open_cmd(tokens.restore(loc)))
		return open_cmd(tokens.restore(loc));
	if (close_cmd(tokens.restore(loc)))
		return close_cmd(tokens.restore(loc));
	if (write_cmd(tokens.restore(loc)))
		return write_cmd(tokens.restore(loc));
	if (exit_cmd(tokens.restore(loc)))
		return exit_cmd(tokens.restore(loc));
	if (show_cmd(tokens.restore(loc)))
		return show_cmd(tokens.restore(loc));
	if (create_cmd(tokens.restore(loc)))
		return create_cmd(tokens.restore(loc));
	if (update_cmd(tokens.restore(loc)))
		return update_cmd(tokens.restore(loc));
	if (insert_cmd1(tokens.restore(loc)))
		return insert_cmd1(tokens.restore(loc));
	if (insert_cmd2(tokens.restore(loc)))
		return insert_cmd2(tokens.restore(loc));
	if (delete_cmd(tokens.restore(loc)))
		return delete_cmd(tokens.restore(loc));
	return NULL;
}

/*
 * atomic_expr()
 * This function determines if a set of tokens represents an atomic expression
 * relation | (expr)
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::atomic_expr(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN ATOMIC EXPR" << endl;
	int loc = tokens.save();
	ParseNode* rel = relation(tokens);
	if (rel)
		return rel;
	Token next = tokens.get();
	if (next.value == "(") {
		ParseNode* exp = expr(tokens);
		if (exp) {
			Token end_paren = tokens.get();
			if (end_paren.value == ")")
				return exp;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * op()
 * This function determines if a set of tokens represents an operator
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::op(TokenStream& tokens) {
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
		return new ParseNode(oprnd.value,OPERATOR);
	if (oprnd.type != NO_TOKEN) tokens.unget();
	return NULL;
}

/*
 * attrib()
 * This function determines if a set of tokens represents an attribute
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::attrib(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN ATRIB" << endl;
	Token name = tokens.get();
	if (name.type == IDENTIFIER)
		return new ParseNode(name.value,IDENTIFIER);
	if (name.type != NO_TOKEN) tokens.unget();
	return NULL;
}

/*
 * literal()
 * This function determines if a set of tokens represents a literal number
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::literal(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN LITERAL" << endl;
	Token name = tokens.get();
	if (DEBUG)
		cout << "LITERAL TYPE " << name.type << endl;
	if (name.type == NUMBER)
		return new ParseNode(name.value,NUMBER);
	if (name.type == LITERAL)
		return new ParseNode(name.value,LITERAL);
	if (name.type != NO_TOKEN) tokens.unget();
	return NULL;
}

/*
 * operand()
 * This function determines if a set of tokens represents an operand
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::operand(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN OPERAND" << endl;
	int loc = tokens.save();
	if (attrib(tokens.restore(loc)))
		return attrib(tokens.restore(loc));
	if (literal(tokens.restore(loc)))
		return literal(tokens.restore(loc));
	return NULL;
}

/*
 * comparison()
 * This function determines if a set of tokens represents a comparison
 * operand op operand | ( condition )
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::comparison(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN COMP" << endl;
	int loc = tokens.save();
	ParseNode* oper1 = operand(tokens);
	ParseNode* op1 = NULL;
	ParseNode* oper2 = NULL;
	if (oper1) {
		op1 = op(tokens);
		if (op1) {
			oper2 = operand(tokens);
			if (oper2) {
				ParseNode* comp = new ParseNode("COMPARISON",KEYWORD);
				comp->children.push_back(oper1);
				comp->children.push_back(op1);
				comp->children.push_back(oper2);
				return comp;
			}
		}
	}
	tokens.restore(loc);
	Token open_paren = tokens.get();
	if (open_paren.value == "(") {
		ParseNode* exp = condition(tokens);
		if (exp) {
			Token end_paren = tokens.get();
			if (end_paren.value == ")")
				return exp;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * comparison()
 * This function determines if a set of tokens represents a conjunction
 * comparison {&& comparison}
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::conjunction(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN CONJ" << endl;
	int loc = tokens.save();
	ParseNode* comp = comparison(tokens);
	if (!comp) {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* conj = new ParseNode("CONJUCTION",KEYWORD);
	conj->children.push_back(comp);
	Token andand = tokens.get();
	if (andand.value == "&&") {
		ParseNode* cmp =  comparison(tokens);
		if (!cmp) {
			tokens.restore(loc);
			delete conj;
			return NULL;
		}
		conj->children.push_back(cmp);
		return conj;
	}
	if (andand.type != NO_TOKEN) tokens.unget();
	return conj;
}

/*
 * comparison()
 * This function determines if a set of tokens represents a condition
 * condition {|| condition}
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::condition(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN CONDITION" << endl;
	int loc = tokens.save();
	ParseNode* conj = conjunction(tokens);
	if (!conj) {
		tokens.restore(loc);
		return NULL;
	}
	ParseNode* cond = new ParseNode("CONDITION",KEYWORD);
	cond->children.push_back(conj);
	Token oror = tokens.get();
	if (oror.value == "||") {
		ParseNode* cmp = conjunction(tokens);
		if (!cmp) {
			tokens.restore(loc);
			delete cond;
			return NULL;
		}
		cond->children.push_back(cmp);
		return cond;
	}
	if (oror.type != NO_TOKEN) tokens.unget();
	return cond;
}

/*
 * selection()
 * This function determines if a set of tokens represents a select statement
 * SELECT (condition) relation
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::selection(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN SELECT" << endl;
	int loc = tokens.save();
	Token select = tokens.get();
	if (select.value == "SELECT") {
		Token open_paren = tokens.get();
		if (open_paren.value == "(") {
			ParseNode* cond = condition(tokens);
			if (cond) {
				Token end_paren = tokens.get();
				if (end_paren.value == ")") {
					ParseNode* ae = atomic_expr(tokens);
					if (ae) {
						ParseNode* sel = new ParseNode("SELECT",KEYWORD);
						sel->children.push_back(cond);
						sel->children.push_back(ae);
						return sel;
					}
				}
			}
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * attrib_list()
 * This function determines if a set of tokens represents a attribute list
 * attrib {, attrib_list}
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::attrib_list(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN ATRIBLIST" << endl;
	int loc = tokens.save();
	ParseNode* attr = attrib(tokens);
	if (!attr)
		return NULL;
	ParseNode* attr_list = new ParseNode("ATTRIB", KEYWORD);
	attr_list->children.push_back(attr);
	Token comma = tokens.get();
	if (comma.value == ",") {
		ParseNode* al =  attrib_list(tokens); // changed attrib to attrib_list
		if (!al) {
			tokens.restore(loc);
			delete attr_list;
			return NULL;
		}
		for (int i = 0; i < al->children.size(); ++i)
			attr_list->children.push_back(al->children[i]);
		return attr_list;
	}
	if (comma.type != NO_TOKEN) tokens.unget();
	return attr_list;
}

/*
 * projection()
 * This function determines if a set of tokens represents a projection statement
 * PROJECT (attrib_list) atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::projection(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN PROJECT" << endl;
	int loc = tokens.save();
	Token pro = tokens.get();
	if (pro.value == "PROJECT") {
		Token open_paren = tokens.get();
		if (open_paren.value == "(") {
			ParseNode* cond = attrib_list(tokens);
			if (cond) {
				Token end_paren = tokens.get();
				if (end_paren.value == ")") {
					ParseNode* ae =  atomic_expr(tokens);
					if (ae) {
						ParseNode* pro = new ParseNode("PROJECT",KEYWORD);
						pro->children.push_back(cond);
						pro->children.push_back(ae);
						return pro;
					}
				}
			}
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * renaming()
 * This function determines if a set of tokens represents a renaming statement
 * RENAME (attrib_list) atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::renaming(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN RENAME" << endl;
	int loc = tokens.save();
	Token rename = tokens.get();
	if (rename.value == "RENAME") {
		Token open_paren = tokens.get();
		if (open_paren.value == "(") {
			ParseNode* cond = attrib_list(tokens);
			if (cond) {
				Token end_paren = tokens.get();
				if (end_paren.value == ")") {
					ParseNode* ae = atomic_expr(tokens);
					if (ae) {
						ParseNode* ren = new ParseNode("RENAME",KEYWORD);
						ren->children.push_back(cond);
						ren->children.push_back(ae);
						return ren;
					}
				}
			}
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * union_g()
 * This function determines if a set of tokens represents a union statement
 * atomic_expr + atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::union_g(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN UNION" << endl;
	int loc = tokens.save();
	ParseNode* at_expr = atomic_expr(tokens);
	if (!at_expr) {
		tokens.restore(loc);
		return NULL;
	}
	Token plus = tokens.get();
	if (plus.value == "+") {
		ParseNode* ae = atomic_expr(tokens);
		if (ae) {
			ParseNode* plus = new ParseNode("+",OPERATOR);
			plus->children.push_back(at_expr);
			plus->children.push_back(ae);
			return plus;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * difference()
 * This function determines if a set of tokens represents a difference statement
 * atomic_expr - atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::difference(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN DIFFERENCE" << endl;
	int loc = tokens.save();
	ParseNode* at_expr = atomic_expr(tokens);
	if (!at_expr) {
		tokens.restore(loc);
		return NULL;
	}
	Token minus = tokens.get();
	if (minus.value == "-") {
		ParseNode* ae = atomic_expr(tokens);
		if (ae) {
			ParseNode* sub = new ParseNode("-",OPERATOR);
			sub->children.push_back(at_expr);
			sub->children.push_back(ae);
			return sub;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * product()
 * This function determines if a set of tokens represents a product statement
 * atomic_expr * atomic_expr
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::product(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN PRODUCT" << endl;
	int loc = tokens.save();
	ParseNode* at_expr = atomic_expr(tokens);
	if (!at_expr) {
		tokens.restore(loc);
		return NULL;
	}
	Token times = tokens.get();
	if (times.value == "*") {
		ParseNode* ae = atomic_expr(tokens);
		if (ae) {
			ParseNode* star = new ParseNode("*",OPERATOR);
			star->children.push_back(at_expr);
			star->children.push_back(ae);
			return star;
		}
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * expr()
 * This function determines if a set of tokens represents a expression
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::expr(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN EXPR" << endl;
	int loc = tokens.save();
	if (selection(tokens.restore(loc)))
		return selection(tokens.restore(loc));
	if (projection(tokens.restore(loc)))
		return projection(tokens.restore(loc));
	if (renaming(tokens.restore(loc)))
		return renaming(tokens.restore(loc));
	if (union_g(tokens.restore(loc)))
		return union_g(tokens.restore(loc));
	if (difference(tokens.restore(loc)))
		return difference(tokens.restore(loc));
	if (product(tokens.restore(loc)))
		return product(tokens.restore(loc));
	if (atomic_expr(tokens.restore(loc)))
		return atomic_expr(tokens.restore(loc));
	return NULL;
}

/*
 * query()
 * This function determines if a set of tokens represents a query
 * relation <- expr
 * @param tokens A TokenStream object already broken into one statement
 */
ParseNode* Parser::query(TokenStream& tokens) {
	if (DEBUG)
		cout << "IN QUERY" << endl;
	int loc = tokens.save();
	ParseNode* rel = relation(tokens);
	if (rel) {
		Token arrow = tokens.get();
		if (arrow.value != "<-") {
			tokens.restore(loc);
			return NULL;
		}
		ParseNode* exp = expr(tokens);
		if (exp) {
			ParseNode* arrow = new ParseNode("<-",OPERATOR);
			arrow->children.push_back(rel);
			arrow->children.push_back(exp);
			return arrow;
		}
		return NULL;
	}
	tokens.restore(loc);
	return NULL;
}

/*
 * check_grammar()
 * This function determines if a set of tokens has proper grammar
 * @param tokens A vector of tokens already broken into one statement
 */
ParseNode* Parser::check_grammar(vector<Token> tokens) {
	TokenStream tok(tokens);
	int loc = tok.save();
	ParseNode* result;
	result = ( (query(tok))?query(tok.restore(loc)):command(tok.restore(loc)) );
	Token extra = tok.get();
	if (extra.type == NO_TOKEN)
		return result;
	return NULL;
}

/*
 * parse()
 * This function tokenizes a string and breaks it into statements then checks their grammar
 * individually for correct syntax in the SQL language
 * @param tokens A string representing the SQL language
 */
vector<ParseNode*> Parser::parse(string s) {
	string info = "";
	stringstream ss;
	ss << s;
	string line;
	while (getline(ss,line))
		info += line;
	// cout << info << endl;
	vector<Token> tokens = tokenize(info);
	Statement statements = break_statements(tokens);
	vector<ParseNode*> results;
	for (int i = 0; i < statements.size(); ++i) {
		results.push_back(check_grammar(statements[i]));
	}
	return results;
}
