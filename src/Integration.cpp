//  Integration.cpp

#include <iostream>
#include <string>
#include "Parse.h"
#include "Attribute.h"
#include "Database.h"
#include "Relation.h"

using namespace std;

void execute(ParseNode* node, Database db) {
    string name;
    Relation rel;
    Attribute att;
    vector<int> attributes;
    ParseNode* relnode; // Relation node
    ParseNode* condnode; // Condition Node
    ParseNode* tlnode; // Type List node
    ParseNode* tsnode1; // Type Set node
    ParseNode* setnode1; // Set node
    ParseNode* setnode2; // Set node
    ParseNode* compnode1; // Comparison node
    ParseNode* compnode2; // Comparison node
    
	if (node->value == "<-") {
		cout << "ok 1" << endl;
	}
	else if (node->value == "==") {
		cout << "ok 2" << endl;
	}
	else if (node->value == "!=") {
		cout << "ok 3" << endl;
	}
	else if (node->value == "<=") {
		cout << "ok 4" << endl;
	}
	else if (node->value == "=>") {
		cout << "ok 5" << endl;
	}
	else if (node->value == "<") {
		cout << "ok 6" << endl;
	}
	else if (node->value == ">") {
		cout << "ok 7" << endl;
	}
	else if (node->value == "=") {
		cout << "ok 8" << endl;
	}
	else if (node->value == "UPDATE") {
		relnode = (node->children[0]);
		setnode1 = (node->children[1]);
        setnode2 = (node->children[2]);
        condnode = (node->children[3]);
        compnode1 = ((condnode->children[0])->children[0])->children[0];
        compnode2 = ((condnode->children[0])->children[0])->children[2];
        db.addRelation(relnode->value); // For debugging
        if (db.relationExists(relnode->value)) {
            attributes = att.findValue(compnode2->value);
            cout << "YAY!" << endl;
            for (int i = 0; i < attributes.size(); i++) {
                db.updateAttributeValue(relnode->value, attributes[i], setnode1->value, setnode2->value);
            }
        }
		cout << "ok 2" << endl;
	}
	else if (node->value == "PROJECT") {
		cout << "ok 3" << endl;
	}
	else if (node->value == "RENAME") {
		cout << "ok 4" << endl;
	}
	else if (node->value == "OPEN") {
		cout << "ok 5" << endl;
	}
	else if (node->value == "CLOSE") {
		cout << "ok 6" << endl;
	}
	else if (node->value == "WRITE") {
		cout << "ok 7" << endl;
	}
	else if (node->value == "EXIT") {
		cout << "ok 8" << endl;
	}
	else if (node->value == "SHOW") {
        //relnode = (node->children[0]);
        //db.show(relnode->value);
		cout << "Ok Show" << endl;
	}
	else if (node->value == "CREATE") {
        relnode = (node->children[0]);
        tlnode = (node->children[1]);
        db.addRelation(relnode->value);
        for (int i = 0; i< (tlnode->children).size(); i++) {
            tsnode1 = (tlnode->children[i]);
            if ((tsnode1->children[1])->value == "INTEGER") {
                db.addAttribute(relnode->value,(tsnode1->children[0])->value,(tsnode1->children[1])->value, 0);
                cout << "Added Attribute "<< (tsnode1->children[0])->value <<endl;
            }
            else {
                int x = atoi((tsnode1->children[1])->value.c_str());
                db.addAttribute(relnode->value,(tsnode1->children[0])->value,"STRING", x);
                cout << "Added Attribute "<< (tsnode1->children[0])->value <<endl;
            }
        }
        db.show(relnode->value);
        cout << "Succesfully added attributes" << endl;
	}
	else if (node->value == "INSERT") {
		cout << "ok 11" << endl;
	}
	else if (node->value == "DELETE") {
		cout << "ok 12" << endl;
	}
}

void print_node(ParseNode* node, int level) {
	cout << "|" << flush;
	for (int i = 0; i < level; ++i)
		cout << "---" << flush;
    
	cout << node->value << endl;
	for (int i = 0; i < node->children.size(); ++i)
		print_node(node->children[i],level+1);
}

int main()
{
    Database db;
	Parser p;
	string s;
	vector<Token> vec;
    // Open
	cout << "Enter code to parse: " << flush;
	getline(cin, s);
	cout << "Str:" << s << endl;
	vector<ParseNode*> statements = p.parse(s);
	
	for(int i = 0; i < statements.size(); ++i) {
		print_node(statements[i],0);
	}
	for (int i = 0; i< statements.size(); i++) {
		execute(statements[i], db);
	}
	
    return 0;
}
