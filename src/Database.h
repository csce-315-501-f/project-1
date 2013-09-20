#ifndef DATABASE
#define DATABASE

#include "Relation.h"
#include <algorithm>
#include <vector>
#include <fstream>
using namespace std;

/** 
 * @brief Database Class represents one database.
 * The Database Class represents one database that includes a vector of Relation class objects that represent the tables in the database. It also includes member functions that provide the functionality to perform operations on those relations.
 */
class Database {
private:
    map<string, Relation> relations; /**< Map of Relation names to Relations */
    vector<string> relationNames;    /**< Holds the Relation names in order of insertion */
public:
    /**
     * Checks if the given Relation exists in the Database
     * @param name Name of Relation to check
     * @return True if Relation exists in Database
     */
    bool relationExists(string name);

    /**
     * Checks if the given Attribute exists in the given Relation
     * @param relName Name of Relation to check
     * @param attrName Name of Attribute to check
     * @return True if Attribute exists in Relation
     */
    bool attributeExists(string relName, string attrName);

    /**
     * Adds a Relation to the Database
     * @param name Name of Relation to add
     * @param True if Relation was successfully added
     */
    bool addRelation(string name);

    /**
     * Adds an Attribute to the given Relation
     * @param relName Name of Relation to add Attribute to
     * @param attrName Name of Attribute
     * @param type Type of Attibute
     * @param length Length of Attribute
     * @return True if Attribute was successfully added to Relation
     */
    bool addAttribute(string relName, string attrName, string type, int length);

    /**
     * Removes an Attribute from the given Relation
     * @param relName Name of Relation to remove Attribute from
     * @param attrName Name of Attribute to remove
     * @return True if Attribute successfully removed from Relation
     */
    bool removeAttribute(string relName, string attrName);

    /**
     * Removes a given Relation from the Database
     * @param name Name of Relation to remove
     * @return True if Relation successfully removed from Database
     */
    bool removeRelation(string name);

    /**
     * Prints the Database, or given Relation to command line. Prints all Relations if no Relation name given.
     * @param name Name of Relation to show
     */
    void show(string name = "");

    /**
     * Adds a row to the given Relation
     * @param relName Name of Relation to add row to
     * @param row Row to add to Relation
     * @return Key of added row
     */
    int addRow(string relName, vector<string> row);


    /**
     * Removes a row from a Relation
     * @param relName Name of Relation to remove row from
     * @param key Key of row to remove from Relation
     */
    void removeRow(string relName, int key);

    /**
     * Updates an Attribute's value in a given Relation
     * @param relName Name of Relation
     * @param key Key of value to update
     * @param attributeName Name of Attribute to be updated
     * @param value Value to update old value to
     * @return True if the value was successfully updated
     */
    bool updateAttributeValue(string relName, int key, string attributeName, string value);

    /**
     * Selection Operation. Gets rows in Relation where a given value exists in a given Attribute.
     * @param relName Name of Relation to get from
     * @oaram attrName Name of Attribute to search
     * @param value Value to find
     * @return Rows in given Relation where the given value was found in the given Attribute
     */
    vector< vector<string> > getRowsWhere(string relName, string attrName, string value, string op = "==");

    /**
     * Renames the given Relation
     * @param currentName Current Relation name
     * @param newName New Relation name
     * @return True if Relation successfully renamed
     */
    bool renameRelation(string currentName, string newName);

    /**
     * Renames an Attribute in a given Relation
     * @param relName Name of Relation
     * @param currentName Current Attribute name
     * @param newName New Attribute name
     * @return True if Attribute in Relation was successfully renamed
     */
    bool renameAttribute(string relName, string currentName, string newName);

    /**
     * Projection Operation
     * @param relName Name of Relation
     * @param attrNames Names of Attributes for use in projection
     * @return Relation of the resulting projection
     */
    Relation projection(string relName, vector<string> attrNames);

    /**
     * Checks if the given Relations are union-compatible
     * @param relName1 Name of the first Relation
     * @param relname2 Name of the second Relation
     * @return True if the two given Relations are union-compatible
     */
    bool unionCompatible(string relName1, string relName2);

    /**
     * Union Operation.
     * @param relName1 Name of the first Relation
     * @param relname2 Name of the second Relation
     * @return Relation of the resulting union
     */
    Relation relationUnion(string relName1, string relName2);

    /**
     * Difference Operation.
     * @param relName1 Name of the first Relation
     * @param relname2 Name of the second Relation
     * @return Relation of the resulting difference
     */
    Relation relationDifference(string relName1, string relName2);

    /**
     * Cross/Cartesian Product Operation
     * @param relName1 Name of the first Relation
     * @param relName2 Name of the second Relation
     * @return Relation of the resulting cross/cartesian product
     */
    Relation relationCrossProduct(string relName1, string relName2);

    /**
     * Writes the given relation to a file
     * @param relName Name of the Relation to write
     */
    void writeRelation(string relName);
};

#endif
