#ifndef RELATION
#define RELATION

#include "Attribute.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

/** 
 * @brief Relation Class represents one table in the database.
 * The Relation Class represents one table in a database. It includes a vector of Attribute class objects that represent the columns in the relation.
 */
class Relation {
    friend class Database;
private:
    map<string, Attribute> attributes; /**< Map of Attribute names mapped to Attributes */
    vector<string> attributeNames;     /**< Holds the Attribute names in order of insertion */
public:
    /**
     * Checks if the given Attribute exists in the Relation
     * @param name Name of Attribute to check
     * @return True if Attribute exists in Relation
     */
    bool attributeExists(string name);

    /**
     * Adds an Attribute to the Relation. If other Attributes exist, this also inserts the value "" for each row that there is already a value in other Attributes.
     * @param name Name of Attribute to add
     * @param type Type of Attribute to add
     * @param length Length of Attribute to add
     * @return True if Attribute was successfully added to the Relation
     */
    bool addAttribute(string name, string type, int length);

     /**
     * Adds a row to the Relation
     * @param row Row of values to add. Must be same length as number of Attributes in the Relation.
     * @return Key of added row, -1 if Attribute was not added.
     */
    int addRow(vector<string> row);

    /**
     * Removes the specified row from the Relation
     * @param key Key of row to remove
     */
    void removeRow(int key);

    /**
     * Removes an Attribute from the Relation
     * @param name Name of attribute to remove
     * @return True if Attribute was successfully removed
     */
    bool removeAttribute(string name);

    /**
     * Gets the specified row
     * @param key Key of row to get
     * @return Row found at given key, or empty vector if row not found.
     */
    vector<string> getRow(int key);

    /**
     * Gets rows where a given Attribute has a given value
     * @param attributeName Attribute to search
     * @param value Value to find in given Attribute
     * @param op Operator for value comparison (>,<,=>,=<,==)
     * @return Rows where the given value was found in the given Attribute
     */
    vector< vector<string> > getRowsWhere(string attributeName, string value, string op = "==");

    /**
     * Gets all rows in the Relation
     * @return All rows in the Relation
     */
    vector< vector<string> > getAllRows();

    /**
     * Updates a given Attributes value
     * @param key of value to update
     * @param attributeName Attribute to update value in
     * @param value Value to update current value to
     * @return True if value was updated successfully
     */
    bool updateAttributeValue(int key, string attributeName, string value);

    /**
     * Prints the Relation to command line
     */
    void show();

    /**
     * Renames a given Attribute
     * @param currentName The Attribute to rename
     * @param newName New name for the Attribute
     * @return True if Attribute was successfully renamed
     */
    bool renameAttribute(string currentName, string newName);

    /**
     * Assignment Operator for Relation class
     */
    Relation operator=(Relation b);
};



#endif
