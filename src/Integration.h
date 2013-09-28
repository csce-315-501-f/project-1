//  Integration.h

#include <iostream>
#include <string>
#include "Parse.h"
#include "Attribute.h"
#include "Database.h"
#include "Relation.h"

using namespace std;

void execute(ParseNode* node, Database& db) {
    string name, s, ext;
    Parser p;
    Relation rel, relunion, relproj, relcp, reldif, select;
    Attribute att;
    vector<int> attributes;
    vector<string> row;
    vector<vector<string> > vrow;
    ParseNode* relnode; // Relation node
    ParseNode* condnode; // Condition Node
    ParseNode* tlnode; // Type List node
    ParseNode* tsnode1, *tsnode2; // Type Set nodes
    ParseNode* setnode1, *setnode2; // Set nodes
    ParseNode* compnode1, *compnode2; // Comparison nodes
    ParseNode* qnode; // Query node
    ParseNode* opnode; // Operation Node
    
    /********************************************************Query************************************************************/
	if (node->value == "<-") {
        qnode = (node->children[1]);
    /****************************************UNION*****************************************************/
        if (qnode->value == "+") {
            tsnode1 = (qnode->children[0]);
            tsnode2 = (qnode->children[1]);
    /****************************************SELECT*****************************************************/
            if (tsnode2->value == "SELECT") {
                relnode = node->children[1];
                compnode1 = (((qnode->children[0])->children[0])->children[0])->children[0];    // Attribute
                compnode2 = (((qnode->children[0])->children[0])->children[0])->children[2];    // Value
                opnode = (((qnode->children[0])->children[0])->children[0])->children[1];       // Inequlaity
                setnode1 = (qnode->children[1]);                                                // Relation node
                select = db.selection(setnode1->value, compnode1->value,compnode2->value, opnode->value);
                db.addRelation(relnode->value);
                db.addAttribute(relnode->value, select.getAttributeNames(0), select.getAttributeType(compnode1->value), select.getAttributeLength(compnode1->value));
                vrow = db.getRowsWhere(setnode1->value, compnode1->value, compnode2->value, opnode->value);
                /*for (int i = 0; vrow.size(); i++) {
                 db.addRow(relnode->value,vrow[i]);
                 }*/
                //db.show(relnode->value);
                // cout << "SELECTION done succesfully" << endl;
                relunion = db.relationUnion(select.getName(), tsnode1->value);
                relunion.setName(node->children[0]->value);
            }
    /****************************************Project*****************************************************/
            else if (tsnode2->value == "PROJECT") {
                for (int i = 0; i < ((tsnode2->children[0])->children.size()); i++ ) {
                    row.push_back((tsnode2->children[0])->children[i]->value);
                }
                relnode = (qnode->children[0]);                          // Relation node
                relproj = db.projection(relnode->value, row);
                relproj.setName("temp");
                relunion = db.relationUnion(relproj.getName(), relnode->value);
                relunion.setName(node->children[0]->value);
                //cout << "+ and project worked" << endl;
            }
    /****************************************RENAME*****************************************************/
            else if (tsnode2->value == "RENAME") {
                for (int i = 0; i < (tsnode2->children[0])->children.size(); i++) {
                    db.renameRelation((tsnode2->children[i])->value, (tsnode2->children[0])->children[i]->value);
                }
                relunion = db.relationUnion(tsnode1->value, tsnode2->value);
                relunion.setName(node->children[0]->value);
            }
            else {
                relunion = db.relationUnion(tsnode1->value, tsnode2->value);
            }
        }
    /****************************************Difference*****************************************************/
        if (qnode->value == "-") {
            tsnode1 = (qnode->children[0]);
            tsnode2 = (qnode->children[1]);
    /****************************************SELECT*****************************************************/
            if (tsnode2->value == "SELECT") {
                relnode = node->children[1];
                compnode1 = (((qnode->children[0])->children[0])->children[0])->children[0];    // Attribute
                compnode2 = (((qnode->children[0])->children[0])->children[0])->children[2];    // Value
                opnode = (((qnode->children[0])->children[0])->children[0])->children[1];       // Inequlaity
                setnode1 = (qnode->children[1]);                                                // Relation node
                select = db.selection(setnode1->value, compnode1->value,compnode2->value, opnode->value);
                db.addRelation(relnode->value);
                db.addAttribute(relnode->value, select.getAttributeNames(0), select.getAttributeType(compnode1->value), select.getAttributeLength(compnode1->value));
                vrow = db.getRowsWhere(setnode1->value, compnode1->value, compnode2->value, opnode->value);
                /*for (int i = 0; vrow.size(); i++) {                                                        // FIX THIS!!
                 db.addRow(relnode->value,vrow[i]);
                 }*/
                //db.show(relnode->value);
                // cout << "SELECTION done succesfully" << endl;
                reldif = db.relationDifference(select.getName(), tsnode1->value);
                reldif.setName(node->children[0]->value);
            }
    /****************************************Projection*****************************************************/
            else if (tsnode2->value == "PROJECT") {
                for (int i = 0; i < ((tsnode2->children[0])->children.size()); i++ ) {
                    row.push_back((tsnode2->children[0])->children[i]->value);
                }
                relnode = (qnode->children[0]);                          // Relation node
                relproj = db.projection(relnode->value, row);
                relproj.setName("temp");
                reldif = db.relationDifference(relproj.getName(), relnode->value);
                reldif.setName(node->children[0]->value);
            }
    /****************************************RENAME*****************************************************/
            else if (tsnode2->value == "RENAME") {
                for (int i = 0; i < (tsnode2->children[0])->children.size(); i++) {
                    db.renameRelation((tsnode2->children[i])->value, (tsnode2->children[0])->children[i]->value);
                }
                reldif = db.relationDifference(tsnode1->value, tsnode2->value);
                reldif.setName(tsnode1->value);
            }
            else {
                reldif = db.relationDifference(tsnode1->value, tsnode2->value);
                reldif.setName(node->children[0]->value);
            }
        }
    /****************************************Cross Product*****************************************************/
        if (qnode->value == "*") {
            tsnode1 = (qnode->children[0]);
            tsnode2 = (qnode->children[1]);
    /****************************************SELECT*****************************************************/
            if (tsnode2->value == "SELECT") {
                relnode = node->children[1];
                compnode1 = (((qnode->children[0])->children[0])->children[0])->children[0];    // Attribute
                compnode2 = (((qnode->children[0])->children[0])->children[0])->children[2];    // Value
                opnode = (((qnode->children[0])->children[0])->children[0])->children[1];       // Inequlaity
                setnode1 = (qnode->children[1]);                                                // Relation node
                select = db.selection(setnode1->value, compnode1->value,compnode2->value, opnode->value);
                db.addRelation(relnode->value);
                db.addAttribute(relnode->value, select.getAttributeNames(0), select.getAttributeType(compnode1->value), select.getAttributeLength(compnode1->value));
                vrow = db.getRowsWhere(setnode1->value, compnode1->value, compnode2->value, opnode->value);
                /*for (int i = 0; vrow.size(); i++) {                                                        // FIX THIS!!
                 db.addRow(relnode->value,vrow[i]);
                 }*/
                //db.show(relnode->value);
                // cout << "SELECTION done succesfully" << endl;
                relcp = db.relationCrossProduct(select.getName(), tsnode1->value);
                relcp.setName(node->children[0]->value);
            }
    /****************************************Projection*****************************************************/
            else if (tsnode2->value == "PROJECT") {
                for (int i = 0; i < ((tsnode2->children[0])->children.size()); i++ ) {
                    row.push_back((tsnode2->children[0])->children[i]->value);
                }
                relnode = (qnode->children[0]);                          // Relation node
                relproj = db.projection(relnode->value, row);
                relproj.setName("temp");
                relcp = db.relationCrossProduct(relproj.getName(), relnode->value);
                relcp.setName(node->children[0]->value);
                //cout << "alright 2" << endl;
            }
    /****************************************Rename*****************************************************/
            else if (tsnode2->value == "RENAME") {
                setnode2 = tsnode2->children[1];
                if (setnode2->value == "PROJECT") {
                    for (int i = 0; i < ((setnode2->children[0])->children.size()); i++ ) {
                        row.push_back((setnode2->children[0])->children[i]->value);
                        db.renameRelation((setnode2->children[0])->children[i]->value, (tsnode2->children[0])->children[i]->value);
                    }
                    relnode = (tsnode2->children[1]);                          // Relation node
                    relproj = db.projection(relnode->value, row);
                    relproj.setName("temp");
                    db.relationCrossProduct(relproj.getName(), tsnode1->value);
                }
                else {
                    for (int i = 0; i < (tsnode2->children[0])->children.size(); i++) {
                        db.renameRelation((tsnode2->children[i])->value, (tsnode2->children[0])->children[i]->value);
                    }
                    //cout << " RENAME in *" << endl;
                }
            }
            else {
                relcp = db.relationCrossProduct(tsnode1->value, tsnode2->value);
                //cout << "CrossProduct Made" << endl;
            }
        }
       
        if (qnode->value == "SELECT") {
            relnode = node->children[1];
            compnode1 = (((qnode->children[0])->children[0])->children[0])->children[0];    // Attribute
            compnode2 = (((qnode->children[0])->children[0])->children[0])->children[2];    // Value
            opnode = (((qnode->children[0])->children[0])->children[0])->children[1];       // Inequlaity
            setnode1 = (qnode->children[1]);                                                // Relation node
            select = db.selection(setnode1->value, compnode1->value,compnode2->value, opnode->value);
            db.addRelation(relnode->value);
            db.addAttribute(relnode->value, select.getAttributeNames(0), select.getAttributeType(compnode1->value), select.getAttributeLength(compnode1->value));
            vrow = db.getRowsWhere(setnode1->value, compnode1->value, compnode2->value, opnode->value);
            /*for (int i = 0; vrow.size(); i++) {
               db.addRow(relnode->value,vrow[i]);
            }*/
            //db.show(relnode->value);
        }
        if (qnode->value == "PROJECT") {
            for (int i = 0; i < ((qnode->children[0])->children.size()); i++ ) {
                row.push_back((qnode->children[0])->children[i]->value);
            }
            relnode = (qnode->children[1]);                          // Relation node
            relproj = db.projection(relnode->value, row);
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
            }
            else {
                for (int i = 0; i < (qnode->children[0])->children.size(); i++) {
                    db.renameRelation((qnode->children[i])->value, (qnode->children[0])->children[i]->value);
                }
            }
        }
	}
    
/*******************************************************Commands***********************************************************/
	else if (node->value == "UPDATE") {
		relnode = (node->children[0]);
		setnode1 = (node->children[1]);
        setnode2 = (node->children[2]);
        condnode = (node->children[3]);
        compnode1 = ((condnode->children[0])->children[0])->children[0];
        compnode2 = ((condnode->children[0])->children[0])->children[2];
        opnode = ((condnode->children[0])->children[0])->children[1];       // Have to check if integer
        if (db.relationExists(relnode->value)) {
            attributes = att.findValue(compnode2->value);
            for (int i = 0; i < attributes.size(); i++) {
                db.updateAttributeValue(relnode->value, attributes[i], setnode1->value, setnode2->value);
            }
        }
	}
	
	else if (node->value == "OPEN") {
        /* 
         Open file, pass it through parser and execute lines
         */
        
        relnode = (node->children[0]);
        ifstream ifs;                                                               //concatenate string + .db
        ext = ".db";
        name = relnode->value + ext;
        ifs.open(name.c_str());
        getline(ifs,s,'\0');
        vector<ParseNode*> statements = p.parse(s);
        for (int i = 0; i< statements.size(); i++) {
        execute(statements[i], db);
        }
	}
	else if (node->value == "CLOSE") {
        relnode = (node->children[0]);
        db.writeRelation(relnode->value);
        
	}
	else if (node->value == "WRITE") {
        relnode = (node->children[0]);
        db.writeRelation(relnode->value);
	}
	else if (node->value == "EXIT") {
        exit(0);
	}
	else if (node->value == "SHOW") {
        relnode = (node->children[0]);
        db.show(relnode->value);
	}
	else if (node->value == "CREATE") {
        relnode = (node->children[0]);
        tlnode = (node->children[1]);
        db.addRelation(relnode->value);
        for (int i = 0; i< (tlnode->children).size(); i++) {
            tsnode1 = (tlnode->children[i]);
            if ((tsnode1->children[1])->value == "INTEGER") {
                db.addAttribute(relnode->value,(tsnode1->children[0])->value,(tsnode1->children[1])->value, 0);
            }
            else {
                int x = atoi((tsnode1->children[1])->value.c_str());
                db.addAttribute(relnode->value,(tsnode1->children[0])->value,"STRING", x);
            }
        }
        //db.show(relnode->value);
	}

//-----------------------------------------------Command INSERT---------------------------------------------------------------------------------
	else if (node->value == "INSERT") {
        relnode = (node->children[0]);
        tlnode = (node->children[1]);
        if (db.relationExists(relnode->value)) {
            if (tlnode->value == "LITERAL LIST") {
                for (int i = 0; i < tlnode->children.size(); i++) { // Get litrals for the row
                    row.push_back((tlnode->children[i])->value);
                }
                db.addRow(relnode->value, row);
            }
            else if (tlnode->value == "PROJECT") {
                db.addRelation(relnode->value);
                for (int i = 0; i < ((tlnode->children[0])->children.size()); i++ ) {
                    row.push_back((tlnode->children[0])->children[i]->value);
                }
                db.addRow(relnode->value, row);
            }
        }
        else {
            cout << "Error: Relation doesn't exists!" << endl;
        }
	}

	else if (node->value == "DELETE") {
        relnode = (node->children[0]);
        condnode = (node->children[1]);
        compnode1 = ((condnode->children[0])->children[0])->children[0];
        compnode2 = ((condnode->children[0])->children[0])->children[2];
        opnode = ((condnode->children[0])->children[0])->children[1];
        cout << "Age = " << compnode2->type << endl;
        if (db.relationExists(relnode->value)) {
            attributes = att.findValue(compnode2->value);
            if (opnode->value == "=") {
                for (int i = 0; i < attributes.size(); i++) {
                    db.removeRow(relnode->value, attributes[i]);
                }
            }

        }
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

