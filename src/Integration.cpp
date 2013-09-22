//  Integration.cpp

#include <iostream>
#include <string>
#include "Parse.h"
#include "Attribute.h"
#include "Database.h"
#include "Relation.h"

using namespace std;

/*bool checkifint (string literal){
    int x = atoi(literal.c_str());
}*/

void execute(ParseNode* node, Database db) {
    string name;
    Relation rel, relunion, relproj, relcp, reldif;
    Attribute att;
    vector<int> attributes;
    vector<string> row;
    ParseNode* relnode; // Relation node
    ParseNode* condnode; // Condition Node
    ParseNode* tlnode; // Type List node
    ParseNode* tsnode1, *tsnode2; // Type Set nodes
    ParseNode* setnode1, *setnode2; // Set nodes
    ParseNode* compnode1, *compnode2; // Comparison nodes
    ParseNode* qnode; // Query node
    ParseNode* opnode; // Operation Node
    
	if (node->value == "<-") {
        qnode = (node->children[1]);
		cout << "ok 1" << endl;
        if (qnode->value == "+") {
            tsnode1 = (qnode->children[0]);
            tsnode2 = (qnode->children[1]);
            if (tsnode2->value == "SELECT") {
                compnode1 = ((tsnode2->children[0])->children[0])->children[0];
                compnode2 = ((tsnode2->children[0])->children[0])->children[2];
                opnode = ((tsnode2->children[0])->children[0])->children[1];
                cout << "alright 1" << endl;
            }
            else if (tsnode2->value == "PROJECT") {
                for (int i = 0; i < ((tsnode2->children[0])->children.size()); i++ ) {
                    row.push_back((tsnode2->children[0])->children[i]->value);
                }
                relnode = (qnode->children[0]);                          // Relation node
                relproj = db.projection(relnode->value, row);
                relproj.setName("temp");
                relunion = db.relationUnion(relproj.getName(), relnode->value);
                relunion.setName(tsnode1->value);
                cout << "alright 2" << endl;
            }
            else if (tsnode2->value == "RENAME") {
                for (int i = 0; i < (tsnode2->children[0])->children.size(); i++) {
                    db.renameRelation((tsnode2->children[i])->value, (tsnode2->children[0])->children[i]->value);
                }
                cout << "alright 3" << endl;
            }
            else {
                relunion = db.relationUnion(tsnode1->value, tsnode2->value);
                cout << "Union Made" << endl;
            }
        }
        if (qnode->value == "-") {
            tsnode1 = (qnode->children[0]);
            tsnode2 = (qnode->children[1]);
            if (tsnode2->value == "SELECT") {
                compnode1 = ((tsnode2->children[0])->children[0])->children[0];
                compnode2 = ((tsnode2->children[0])->children[0])->children[2];
                opnode = ((tsnode2->children[0])->children[0])->children[1];
                cout << "alright 1" << endl;
            }
            else if (tsnode2->value == "PROJECT") {
                for (int i = 0; i < ((tsnode2->children[0])->children.size()); i++ ) {
                    row.push_back((tsnode2->children[0])->children[i]->value);
                }
                relnode = (qnode->children[0]);                          // Relation node
                relproj = db.projection(relnode->value, row);
                relproj.setName("temp");
                reldif = db.relationDifference(relproj.getName(), relnode->value);
                reldif.setName(tsnode1->value);
                cout << "alright 2" << endl;
            }
            else if (tsnode2->value == "RENAME") {
                for (int i = 0; i < (tsnode2->children[0])->children.size(); i++) {
                    db.renameRelation((tsnode2->children[i])->value, (tsnode2->children[0])->children[i]->value);
                }
                cout << "alright 3" << endl;
            }
            else {
                reldif = db.relationDifference(tsnode1->value, tsnode2->value);
                cout << "Difference Made" << endl;
            }
        }
        if (qnode->value == "*") {
            tsnode1 = (qnode->children[0]);
            tsnode2 = (qnode->children[1]);
            if (tsnode2->value == "SELECT") {
                compnode1 = ((tsnode2->children[0])->children[0])->children[0];
                compnode2 = ((tsnode2->children[0])->children[0])->children[2];
                opnode = ((tsnode2->children[0])->children[0])->children[1];
                cout << "alright 1" << endl;
            }
            else if (tsnode2->value == "PROJECT") {
                for (int i = 0; i < ((tsnode2->children[0])->children.size()); i++ ) {
                    row.push_back((tsnode2->children[0])->children[i]->value);
                }
                relnode = (qnode->children[0]);                          // Relation node
                relproj = db.projection(relnode->value, row);
                relproj.setName("temp");
                relcp = db.relationCrossProduct(relproj.getName(), relnode->value);
                relcp.setName(tsnode1->value);
                cout << "alright 2" << endl;
            }
            else if (tsnode2->value == "RENAME") {
                for (int i = 0; i < (tsnode2->children[0])->children.size(); i++) {
                    db.renameRelation((tsnode2->children[i])->value, (tsnode2->children[0])->children[i]->value);
                }
                cout << "alright 3" << endl;
            }
            else {
                relcp = db.relationCrossProduct(tsnode1->value, tsnode2->value);
                cout << "CrossProduct Made" << endl;
            }
        }
        if (qnode->value == "SELECT") {
            compnode1 = (((qnode->children[0])->children[0])->children[0])->children[0];
            compnode2 = (((qnode->children[0])->children[0])->children[0])->children[2];
            opnode = (((qnode->children[0])->children[0])->children[0])->children[1];
            setnode1 = (qnode->children[1]);                          // Relation node
            db.getRowsWhere(setnode1->value, compnode1->value, compnode2->value, opnode->value);
            //cout << setnode1->value <<endl;
            cout << "SELECTION done succesfully" << endl;
        }
        if (qnode->value == "PROJECT") {
            for (int i = 0; i < ((qnode->children[0])->children.size()); i++ ) {
                row.push_back((qnode->children[0])->children[i]->value);
            }
            relnode = (qnode->children[1]);                          // Relation node
            relproj = db.projection(relnode->value, row);
            cout << "PROJECTION done succesfully" << endl;
        }
        if (qnode->value == "RENAME") {
            tsnode2 = qnode->children[1];
            if (tsnode2->value == "PROJECT") {
                for (int i = 0; i < ((tsnode2->children[0])->children.size()); i++ ) {
                    row.push_back((tsnode2->children[0])->children[i]->value);
                    db.renameRelation((tsnode2->children[0])->children[i]->value, (qnode->children[0])->children[i]->value);
                }
                relnode = (tsnode2->children[1]);                          // Relation node
                relproj = db.projection(relnode->value, row);
                // Change the name of relation?
            }
            else {
                for (int i = 0; i < (qnode->children[0])->children.size(); i++) {
                    db.renameRelation((qnode->children[i])->value, (qnode->children[0])->children[i]->value);
                }
            }
            cout << "RENAMING done succesfully" << endl;
        }
	}
	else if (node->value == "UPDATE") {
		relnode = (node->children[0]);
		setnode1 = (node->children[1]);
        setnode2 = (node->children[2]);
        condnode = (node->children[3]);
        compnode1 = ((condnode->children[0])->children[0])->children[0];
        compnode2 = ((condnode->children[0])->children[0])->children[2];
        opnode = ((condnode->children[0])->children[0])->children[1];       // Have to check if integer
        db.addRelation(relnode->value);                                     // For debugging
        if (db.relationExists(relnode->value)) {
            attributes = att.findValue(compnode2->value);
            cout << "YAY!" << endl;
            for (int i = 0; i < attributes.size(); i++) {
                db.updateAttributeValue(relnode->value, attributes[i], setnode1->value, setnode2->value);
            }
        }
		cout << "ok 2" << endl;
	}
	
	else if (node->value == "OPEN") {
		cout << "ok 5" << endl;
	}
	else if (node->value == "CLOSE") {
		cout << "ok 6" << endl;
	}
	else if (node->value == "WRITE") {
        relnode = (node->children[0]);
        db.writeRelation(relnode->value);
		cout << "ok 7" << endl;
	}
	else if (node->value == "EXIT") {
		cout << "ok 8" << endl;
	}
	else if (node->value == "SHOW") {
        relnode = (node->children[0]);
        db.show(relnode->value);
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
    /*
     Are the literals going to be in order in relation to the attributes location 
     or can they be out of order?
     How do I get the correct attribute?
     Use function addRow?
     INSERT INTO animals VALUES FROM RELATION (Joe);
        How do you know which attribute Joe would go into?
     */
	else if (node->value == "INSERT") {
        relnode = (node->children[0]);
        tlnode = (node->children[1]);
        db.addRelation(relnode->value);                 // Create relation for debugging
        if (db.relationExists(relnode->value)) {
            if (tlnode->value == "LITERAL LIST") {
                for (int i = 0; i < tlnode->children.size(); i++) { // Get litrals for the row
                    row.push_back((tlnode->children[i])->value);
                    cout << (tlnode->children[i])->value << endl;                              // Debugging
                }
            }
            else {
                rel = db.relationUnion(relnode->value,tlnode->value);
                cout << "Union made" << endl;
            }
            cout << "YAY!" << endl;
        }
        else {
            cout << "Error: Relation doesn't exists!" << endl;
        }
		cout << "ok 11" << endl;
	}
    /*
     Are we deleting just the attribute or the whole row?
     */
	else if (node->value == "DELETE") {
        relnode = (node->children[0]);
        condnode = (node->children[1]);
        compnode1 = ((condnode->children[0])->children[0])->children[0];
        compnode2 = ((condnode->children[0])->children[0])->children[2];
        opnode = ((condnode->children[0])->children[0])->children[1];
        db.addRelation(relnode->value);                                     // For debugging
        cout << "Age = " << compnode2->type << endl;
        if (db.relationExists(relnode->value)) {
            attributes = att.findValue(compnode2->value); // How to initialize to the right att?
            if (opnode->value == "=") {
                for (int i = 0; i < attributes.size(); i++) {
                    db.removeRow(relnode->value, attributes[i]);
                }
            }
            cout << "YAY!" << endl;

        }
		cout << "ok Delete" << endl;
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
