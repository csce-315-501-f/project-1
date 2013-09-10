#include "Database.h"

bool Database::relationExists(string name) {
    if (relations.find(name) == relations.end()) {
        // Relation name not found in database
        return false;
    }
    else {
        return true;
    }
}

bool Database::attributeExists(string relName, string attrName) {
    if (relationExists(relName)) {
        return relations[relName].attributeExists(attrName);
    }
    else {
        return false;
    }
}

bool Database::addRelation(string name) {
    if (relations.find(name) == relations.end()) {
        // attribute name does not exist in `attributes`
        Relation relation;
        relations[name] = relation;
        return true;
    }
    else {
        // error, relation name already exists in attributes
        return false;
    }
}

bool Database::addAttribute(string relName, string attrName, string type, int length) {
    if (relations.find(relName) == relations.end()) {
        // relName not found in database
        return false;
    }
    return relations[relName].addAttribute(attrName, type, length);
}

bool Database::removeAttribute(string relName, string attrName) {
    if (relationExists(relName)) {
        return relations[relName].removeAttribute(attrName);
    }
    else {
        // error, relName not found in database
        return false;
    }
    
}

bool Database::removeRelation(string name) {
    if (relations.find(name) == relations.end()) {
        // error, relName not found in database
        return false;
    }
    relations.erase(name);
    // remove(relationNames.begin(), relationNames.end(), name);
    vector<string> newRelationNames;
    for (int i = 0; i < relationNames.size(); ++i) {
        if (relationNames[i] != name) {
            newRelationNames.push_back(relationNames[i]);
        }
    }
    return !relationExists(name);
}

void Database::show(string name) {
    if (name == "") {
        // print all relations
        for (int i = 0; i < relationNames.size(); ++i) {
            relations[relationNames[i]].show();
        }
    }
    else if (relations.find(name) == relations.end()) {
        // name is not a relation in databse
    }
    else {
        relations[name].show();
    }
}

int Database::addRow(string relName, vector<string> row) {
    if (relationExists(relName)) {
        return relations[relName].addRow(row);
    }
    else {
        return -1;
    }
}

void Database::removeRow(string relName, int key) {
    if (relationExists(relName)) {
        relations[relName].removeRow(key);
    }
}

bool Database::updateAttributeValue(string relName, int key, string attributeName, string value) {
    if (relationExists(relName)) {
        return relations[relName].updateAttributeValue(key,attributeName,value);   
    }
    else {
        // error, relation name not found
        return false;
    }
}

vector< vector<string> > Database::getRowsWhere(string relName, string attrName, string value) {
    vector< vector<string> > rows;
    if (relationExists(relName)) {
        rows = relations[relName].getRowsWhere(attrName,value);
    }
    return rows;
}

bool Database::renameRelation(string currentName, string newName) {
    if (relationExists(newName)) {
        // error, newName already exists
        return false;
    }
    else {
        relations[newName] = relations[currentName];
        relations.erase(currentName);
        for (int i = 0; i < relationNames.size(); ++i) {
            if (relationNames[i] == currentName) {
                relationNames[i] = newName;
                break;
            }
        }
        return !relationExists(currentName) && relationExists(newName);
    }
}

bool Database::renameAttribute(string relName, string currentName, string newName) {
    if (relationExists(relName)) {
        return relations[relName].renameAttribute(currentName, newName);
    }
    else {
        // error, relation does not exist
        return false;
    }
}

Relation Database::projection(string relName, vector<string> attrNames) {
    Relation relation;
    if (relationExists(relName)) {
        relation = relations[relName];
        for (int i = 0; i < relations[relName].attributeNames.size(); ++i) {
            if (find(attrNames.begin(), attrNames.end(), relations[relName].attributeNames[i]) == attrNames.end()) {
                // name found, so add it to relation
                bool success = relation.removeAttribute(relations[relName].attributeNames[i]);
            }
        }
    }
    else {
        // relName doesn't exist
    }
    return relation;
}

 bool Database::unionCompatible(string relName1, string relName2) {
    if (relationExists(relName1) && relationExists(relName2)) {
        return relations[relName1].attributeNames == relations[relName2].attributeNames;
    }
    else {
        return false;
    }
 }

Relation Database::relationUnion(string relName1, string relName2) {
    Relation relation;
    if (relationExists(relName1) && relationExists(relName2) && unionCompatible(relName1,relName2)) {
        // both relations exist in the database and are union compatible
        vector< vector<string> > relVector1 = relations[relName1].getAllRows();
        vector< vector<string> > relVector2 = relations[relName2].getAllRows();

        relVector1.insert(relVector1.end(), relVector2.begin(), relVector2.end());
        sort(relVector1.begin(), relVector1.end());
        relVector1.erase(unique( relVector1.begin(), relVector1.end() ), relVector1.end());

        Relation relation1 = relations[relName1];
        for (int i = 0; i < relation1.attributeNames.size(); ++i) {
            relation.addAttribute(relation1.attributeNames[i], relation1.attributes[relation1.attributeNames[i]].type, relation1.attributes[relation1.attributeNames[i]].length);
        }

        for (int i = 0; i < relVector1.size(); ++i) {
            relation.addRow(relVector1[i]);
        }
    }
    return relation;
}

Relation Database::relationDifference(string relName1, string relName2) {
    Relation relation;
    if (relationExists(relName1) && relationExists(relName2) && unionCompatible(relName1,relName2)) {
        vector< vector<string> > relVector1 = relations[relName1].getAllRows();
        vector< vector<string> > relVector2 = relations[relName2].getAllRows();
        vector< vector<string> > diffVector;

        Relation relation1 = relations[relName1];
        for (int i = 0; i < relation1.attributeNames.size(); ++i) {
            relation.addAttribute(relation1.attributeNames[i], relation1.attributes[relation1.attributeNames[i]].type, relation1.attributes[relation1.attributeNames[i]].length);
        }

        for (int i = 0; i < relVector1.size(); ++i) {
            bool remove = false;
            for (int j = 0; j < relVector2.size(); ++j) {
                remove = remove || (relVector1[i] == relVector2[j]);
            }
            if (!remove) {
                relation.addRow(relVector1[i]);
            }
        }

    }
    return relation;
}

Relation Database::relationCrossProduct(string relName1, string relName2) {
    Relation relation;
    if (relationExists(relName1) && relationExists(relName2)) {
        vector< vector<string> > relVector1 = relations[relName1].getAllRows();
        vector< vector<string> > relVector2 = relations[relName2].getAllRows();

        Relation relation1 = relations[relName1];
        for (int i = 0; i < relation1.attributeNames.size(); ++i) {
            relation.addAttribute(relation1.attributeNames[i], relation1.attributes[relation1.attributeNames[i]].type, relation1.attributes[relation1.attributeNames[i]].length);
        }
        Relation relation2 = relations[relName2];
        for (int i = 0; i < relation2.attributeNames.size(); ++i) {
            relation.addAttribute(relation2.attributeNames[i], relation2.attributes[relation2.attributeNames[i]].type, relation2.attributes[relation2.attributeNames[i]].length);
        }

        for (int i = 0; i < relVector1.size(); ++i) {
            vector<string> row;
            for (int j = 0; j < relVector2.size(); ++j) {
                row.insert(row.end(), relVector1[i].begin(), relVector1[i].end());
                row.insert(row.end(), relVector2[j].begin(), relVector2[j].end());
                relation.addRow(row);
                row.clear();
            }
        }
    }
    return relation;
}
