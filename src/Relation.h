#ifndef RELATION
#define RELATION

#include "Attribute.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class Relation {
    friend class Database;
private:
    map<string, Attribute> attributes; /**< Map of attribute names mapped to attributes */
    vector<string> attributeNames;     /**< Holds the attribute names in order of insertion */
public:
    /**
     * attributeExists
     * @param string name of attribute
     * @return bool attribute exists in relation
     */
    bool attributeExists(string name);

    /**
     * addAttribute
     * Sets all row values to ""
     * @param string name of attribute to add
     * @param string type of attribute to add
     * @param int length of attribute to add
     * @return bool success of adding attribute
     */
    bool addAttribute(string name, string type, int length);

     /**
     * addRow
     * @param vector<string> row of values to add, must be same length as number of attributes
     * @return int key of added row, -1 if not added
     */
    int addRow(vector<string> row);

    /**
     * removeRow
     * @param int key of row to remove
     */
    void removeRow(int key);

    /**
     * removeAttribute
     * @param string name of attribute to remove
     * @return bool success of removing attribute
     */
    bool removeAttribute(string name);

    /**
     * getRow
     * @param int key of row to get
     * @return vector<string> row found at key, or empty vector if not found
     */
    vector<string> getRow(int key);

    /**
     * getRowsWhere
     * @param string attributeName
     * @param string value
     * @return vector< vector<string> > containing rows where attributeName held value, or empty if none found
     */
    vector< vector<string> > getRowsWhere(string attributeName, string value);

    /**
     * getAllRows
     * @return vector< vector<string> > all rows in relation
     */
    vector< vector<string> > getAllRows();

    /**
     * updateAttributeValue
     * @param int key
     * @param string attributeName
     * @param string value
     * @return success of updating value at key in attributeName
     */
    bool updateAttributeValue(int key, string attributeName, string value);

    /**
     * show
     * Prints relation to command line
     */
    void show();

    /**
     * renameAttribute
     * @param string currentName attribute to rename
     * @param string newName new name for attribute
     * @return success of rename
     */
    bool renameAttribute(string currentName, string newName);

    /**
     * Relation = operator
     */
    Relation operator=(Relation b);
};



#endif
