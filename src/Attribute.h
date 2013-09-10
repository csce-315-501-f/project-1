#ifndef ATTRIBUTE
#define ATTRIBUTE

#include <vector>
#include <queue>
#include <algorithm>
#include <string>
using namespace std;


class Attribute{
    friend class Relation;
    friend class Database;
private:

    vector<string> values; /**< Holds the attribute values, with the vector key being the row number */
    string type;           /**< The type of the attribute, available types are `string` and `integer` */
    int length;            /**< If the type is `string` this this is its max length, otherwise */

    queue<int> freeKeys;   /**< Holds the free keys in the array if a value has been removed */

  public:
    /**
     * Default constructor
     * Initializes type to "" and length to 0
     */
    Attribute() : type(""), length(0) {}

    /**
     * Normal Attribute Constructor
     * @param string attrType the type of the attribute
     * @param int len the length of the attribute
     */
    Attribute(string attrType, int len);

    /** 
     * isEmpty
     * @param int key key of value to check
     * @return bool if key == 1 then if Attribute is empty, otherwise if value at key is empty
     */
    bool isEmpty(int key = -1);

    /** 
     * setLength
     * @param int len
     */
    void setLength(int len) { length = len; }

    /** 
     * getType
     * @return string
     */
    string getType() { return type; }

    /** 
     * getSize
     * @return int number of values
     */
    int getSize() { return values.size(); }

    /** 
     * getValue
     * @param int key
     * @return string value at key
     */
    string getValue(int key) { return values[key]; }
    
    /** 
     * findValue
     * @param string value to find
     * @return vector<int> keys where value appears
     */
    vector<int> findValue(string value);

    /** 
     * removeValue
     * @param int key of value to remove
     */
    void removeValue(int key);

    /** 
     * removeLast
     */
    void removeLast() { values.pop_back(); }

    /** 
     * addValue
     * @param string val value to add
     * @param int key position to insert val
     * @return key where val insterted
     */
    int addValue(string val, int key = -1);
    
    /** 
     * updateValue
     * @param int key where to update
     * @param string val to update to
     * @return bool success of update
     */
    bool updateValue(int key, string val);
};

#endif
