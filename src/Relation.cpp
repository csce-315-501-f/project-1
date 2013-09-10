#include "Relation.h"

bool Relation::attributeExists(string name) {
    if (attributes.find(name) == attributes.end()) {
        // error, attribute not found in relation
        return false;
    }
    else {
        return true;
    }
}

bool Relation::addAttribute(string name, string type, int length) {
    if (attributes.find(name) == attributes.end()) {
        // attribute name does not exist in `attributes`
        Attribute attribute(type,length);
        attributes[name] = attribute;

        int length = attributes.begin()->second.getSize();
        for (int i = 0; i < length; ++i) {
            attributes[name].addValue("");
        }
        if (attributes[name].getSize() == length) {
            attributeNames.push_back(name);
            return true;
        }
        else return false;
    }
    else {
        // error, attribute name already exists in attributes
        return false;
    }
}

int Relation::addRow(vector<string> row) {
    if (row.size() != attributeNames.size()) {
        // error, passed row length differs from relation row length
        // return false;
        return -1;
    }
    int insertedKey;
    for (int i = 0; i < attributeNames.size(); ++i) {
        string name = attributeNames[i];
        insertedKey = attributes[name].addValue(row[i]);
        if (insertedKey == -1) {
            // error, failed to add value, remove previously inserted values
            for (int j = i; j > 0; --j) {
                name = attributeNames[j];
                attributes[name].removeLast();
            }
            return -1;
        }
    }
    return insertedKey;
}

void Relation::removeRow(int key) {
    for (int i = 0; i < attributeNames.size(); ++i) {
        attributes[attributeNames[i]].removeValue(key);
    }
}

bool Relation::removeAttribute(string name) {
    if (attributes.find(name) == attributes.end()) {
        // error, attribute not found in relation
        return false;
    }
    attributes.erase(name);
    // remove(attributeNames.begin(), attributeNames.end(), name);
    vector<string> newAttrNames;
    for (int i = 0; i < attributeNames.size(); ++i) {
        if (attributeNames[i] != name) {
            newAttrNames.push_back(attributeNames[i]);
        }
    }
    attributeNames = newAttrNames;
    return !attributeExists(name);
}

vector<string> Relation::getRow(int key) {
    vector<string> row;

    int length = attributes.begin()->second.getSize();
    if (length <= key) {
        // error, out of index
        return row;
    }

    for (int i = 0; i < attributeNames.size(); ++i) {
        string value = attributes[attributeNames[i]].getValue(key);
        row.push_back(value);
    }
    return row;
}

vector< vector<string> > Relation::getRowsWhere(string attributeName, string value) {
    vector< vector<string> > rows;
    if (attributes.find(attributeName) == attributes.end()) {
        // error, attribute does not exist
        return rows;
    }
    vector<int> foundRowKeys = attributes[attributeName].findValue(value);
    
    for (int i = 0; i < foundRowKeys.size(); ++i) {
        vector<string> row = getRow(foundRowKeys[i]);
        rows.push_back(row);
    }
    return rows;
}

vector< vector<string> > Relation::getAllRows() {
    vector< vector<string> > rows;
    int length = attributes.begin()->second.getSize();
    for (int i = 0; i < length; ++i) {
        if (!attributes[attributeNames[0]].isEmpty()) {
            vector<string> row = getRow(i);
            rows.push_back(row);
        }
    }
    return rows;
}

bool Relation::updateAttributeValue(int key, string attributeName, string value) {
    if (attributes.find(attributeName) == attributes.end()) {
        // error, attribute does not exist
        return false;
    }
    bool success = attributes[attributeName].updateValue(key, value);
    return success;
}

void Relation::show() {
    for (int i = 0; i < attributeNames.size(); ++i) {
        cout << attributeNames[i] << "  ";
    }
    cout << endl;

    int length = attributes.begin()->second.getSize();
    for (int i = 0; i < length; ++i) {
        // cout << i << '\t';
        for (int j = 0; j < attributeNames.size(); ++j) {
            string value = attributes[attributeNames[j]].getValue(i);
            cout << value;
            for (int k = 0; k < attributeNames[j].size() - value.size(); ++k) {
                cout << ' ';
            }
            cout << "  ";
        }
        cout << endl;
    }
}

bool Relation::renameAttribute(string currentName, string newName) {
    if (attributeExists(newName)) {
        // error, newName already exists
        return false;
    }
    else {
        attributes[newName] = attributes[currentName];
        attributes.erase(currentName);
        for (int i = 0; i < attributeNames.size(); ++i) {
            if (attributeNames[i] == currentName) {
                attributeNames[i] = newName;
                break;
            }
        }
        return !attributeExists(currentName) && attributeExists(newName);
    }
}

Relation Relation::operator=(Relation b) {
    attributes = b.attributes;
    attributeNames = b.attributeNames;
    return *(this);
}
