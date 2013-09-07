#ifndef RELATION
#define RELATION

#include "Attribute.h"
#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

class Relation {
private:
    // vector<Attribute> attributes;

    // maps attribute name to attribute
    map<string, Attribute> attributes;
    // holds the attribute names in order of insertion
    vector<string> attributeNames;

    queue<int> freeKeys;

public:
    /*
     * Adds a new attribute to vector
     * Sets values for keys in existing attributes
     * to "" in added attribute
     * returns true if attr added successfully
     */
    bool addAttribute(string name, string type, int length) {
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
            // error, attribute namealready exists in attributes
            return false;
        }
    }

    /*
     * Accepts a vector of strings that is the same length
     * as the number of attributes
     * returns true if row successfully added
     */
    bool addRow(vector<string> row) {
        if (row.size() != attributeNames.size()) {
            // error, passed row length differs from relation row length
            return false;
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
                return false;
            }
        }
        return true;
    }


    void removeRow(int key) {

        for (int i = 0; i < attributeNames.size(); ++i) {
            attributes[attributeNames[i]].removeValue(key);
        }

    }

    /*
     * Returns the row at key `key` as a vector of the values
     * or empty vector if not found
     */
    vector<string> getRow(int key) {
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

    /*
     * Returns rows where `attributeName` has `value`
     * or empty vector if not found
     */
    vector< vector<string> > getRowsWhere(string attributeName, string value) {
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


};

#endif
