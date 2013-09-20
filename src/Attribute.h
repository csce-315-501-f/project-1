/**
 * @mainpage CSCE 315-501 -- Group F -- Project 1
 *
 * @section contributors Contributors
 * Christopher Murray, Kevin Vece, Juan Reyes
 * 
 * @section gen_info General Information
 * <p>The program is an academic management application that will be able to keep track of class
 * schedules, assignments, and important dates for the course. Students from elementary school all the way
 * to college could use the program to stay organized in their classes. With this application the users will be
 * able to avoid missed deadlines and keep track of all their course work. The program could also be modified
 * to later entertain projects for companies and small businesses.</p>
 * <p>The data that this application uses will be stored in a custom made Database Management
 * System, that will be designed in parts. The Database Management System will include an API that will
 * control how the data is stored, a Query Language that will define how to issue commands, and a Parser that
 * will translate the queries into function calls that will manipulate data within the database.</p>
 *
 * @subsection location Project Location
 * This project can be found on <a href="https://github.com/csce-315-501-f/project-1">GitHub</a>.
 * @subsection compiler Compiler
 * This project was compiled, debugged, and tested using the g++ compiler.
 * @subsection source_code Source Code
 * The source code for this project is located in the src/ directory.
 * @subsection test_code Test Code
 * The unit tests for this project are located in the tests/ directory.
 *
 * @section test_info Test Compilation Information
 * The tests were compiled and tested with the included Makefile. To compile and run the tests do the following (from project root):
 * <pre>
 * <code>cd tests/
 * make all
 * ./mainTest</code></pre>
 *
 * @section db_func_info Database Algebraic Functions
 * <table>
 * <tr><th>Operation</th><th>Function in Database</th>
 * <tr><td>create table</td><td>Database::addRelation</td></tr>
 * <tr><td>insert into</td><td>Database::addRow</td></tr>
 * <tr><td>delete</td><td>Database::removeRelation<br />Database::removeAttribute<br />Database::removeRow</td></tr>
 * <tr><td>selection</td><td>Database::getRowsWhere</td></tr>
 * <tr><td>projection</td><td>Database::projection</td></tr>
 * <tr><td>renaming</td><td>Database::renameRelation<br />Database::renameAttribute</td></tr>
 * <tr><td>set union</td><td>Database::relationUnion</td></tr>
 * <tr><td>set difference</td><td>Database::relationDifference</td></tr>
 * <tr><td>cross product</td><td>Database::relationCrossProduct</td></tr>
 * <tr><td>update</td><td>Database::updateAttributeValue</td></tr>
 * <tr><td>show</td><td>Database::show</td></tr>
 * </table>
 */

#ifndef ATTRIBUTE
#define ATTRIBUTE

#include <vector>
#include <queue>
#include <algorithm>
#include <string>
using namespace std;

/** 
 * @brief Attribute Class represents one column in a relation.
 * The Attribute class represents one column in a database relation. It includes all the values for that column in a relation, along with the types of values that can be stored there.
 */
class Attribute{
    friend class Relation;
    friend class Database;
private:
    vector<string> values; /**< Holds the attribute values, with the vector key being the row number */
    string type;           /**< The type of the attribute, available types are `string` and `integer` */
    int length;            /**< If the type is `VARCHAR` this this is its max length */

    queue<int> freeKeys;   /**< Holds the free keys in the array if a value has been removed */

public:
    /**
     * Initializes type to "" and length to 0
     */
    Attribute() : type(""), length(0) {}

    /**
     * Normal Attribute Constructor
     * @param attrType The type of the attribute
     * @param len The length of the attribute
     */
    Attribute(string attrType, int len);

    /** 
     * If a value for `key` is passed, then the value at that key will be checked to see if it is empty. If a key is not passed, then each value in the Attribute will be checked if they are empty.
     * @param key Key of value to check
     * @return True if the value, or Attribute, is empty
     */
    bool isEmpty(int key = -1);

    /** 
     * @param len New length for Attribute values
     */
    void setLength(int len) { length = len; }

    /** 
     * @return Type of the Attribute
     */
    string getType() { return type; }

    /**
     * Gets the number of values currently stored in the Attribute.
     * @return Number of values
     */
    int getSize() { return values.size(); }

    /** 
     * Gets a value stored at a given key
     * @param key Key to retrieve value from
     * @return Value at given key
     */
    string getValue(int key) { return values[key]; }
    
    /** 
     * Finds all keys where given value appears
     * @param value Value to find
     * @param op Operator for comparing value
     * @return Keys where value appears
     */
    vector<int> findValue(string value, string op = "==");

    /** 
     * Removes a value from the Attribute
     * @param key Key of value to remove
     */
    void removeValue(int key);

    /** 
     * Removes the last value in the Attribute
     */
    void removeLast() { values.pop_back(); }

    /** 
     * Adds a value to the Attribute. If no key given, the value is inserted at the first available empty position.
     * @param val Value to add
     * @param key Key to insert value at
     * @return Key where the value was inserted, or -1 if the value was not inserted.
     */
    int addValue(string val, int key = -1);
    
    /** 
     * Update the value at a given key
     * @param key Key whhere the value to update is
     * @param val What to update the value to
     * @return Whether the value was successfully updated
     */
    bool updateValue(int key, string val);
};

#endif
