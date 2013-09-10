#ifndef DATABASE
#define DATABASE

#include "Relation.h"
#include <algorithm>
#include <vector>

using namespace std;

class Database {
private:
    map<string, Relation> relations; /**< Map of relation names to relations */
    vector<string> relationNames;    /**< Relation names in order of insertion */
public:
    /**
     * relationExists
     * @param string name of relation
     * @return bool true if relation exists in database
     */
    bool relationExists(string name);

    /**
     * attributeExists
     * @param string relName relation name
     * @param string attrName attribute name
     * @return bool true if attrName exists in relName
     */
    bool attributeExists(string relName, string attrName);

    /**
     * addRelation
     * @param string name relation name to add
     * @param bool success of adding relation
     */
    bool addRelation(string name);

    /**
     * addAttribute
     * @param string relName relation name
     * @param string attrName attribute name
     * @param string type attibute type
     * @param int length length of attribute
     * @return bool success of adding attribute to relName
     */
    bool addAttribute(string relName, string attrName, string type, int length);

    /**
     * removeAttribute
     * @param string relName relation name
     * @param string attrName attribute name
     * @return bool success of removing attrName from relName
     */
    bool removeAttribute(string relName, string attrName);

    /**
     * removeRelation
     * @param string name relation name
     * @return bool succes of removing relation name from database
     */
    bool removeRelation(string name);

    /**
     * show
     * Prints all relations if no relation name given
     * @param string name relation name
     */
    void show(string name = "");

    /**
     * addRow
     * @param string relName relation name
     * @param vector<string> row row to add
     * @return int key of added row
     */
    int addRow(string relName, vector<string> row);


    /**
     * removeRow
     * @param string relName relation name
     * @param int key row to remove
     */
    void removeRow(string relName, int key);

    /**
     * updateAttributeValue
     * @param string relName relation name
     * @param int key
     * @param string attributeName
     * @param string value
     * @return success of updating value of attributeName at key in relName
     */
    bool updateAttributeValue(string relName, int key, string attributeName, string value);

    /**
     * getRowsWhere (Selection Operation)
     * @param string relName relation name
     * @oaram string attrName attribute name
     * @param string value
     * @return vector< vector<string> > all rows in relname where attrName had value
     */
    vector< vector<string> > getRowsWhere(string relName, string attrName, string value);

    /**
     * renameRelation
     * @param string currentName current relation name
     * @param string newName new relation name
     * @return bool success of renaming relation
     */
    bool renameRelation(string currentName, string newName);

    /**
     * renameAttribute
     * @param string relName relation name
     * @param string currentName current attribute name
     * @param string newname new attribute name
     * @return bool success of renaming attribute
     */
    bool renameAttribute(string relName, string currentName, string newName);

    /**
     * projection (Projection Operation)
     * @param string relName relation name
     * @param vector<string> attrNames attribute names
     * @return Relation of the projection of attrNames onto relName
     */
    Relation projection(string relName, vector<string> attrNames);

    /**
     * unionCompatible
     * @param string relName1 first relation name
     * @param string relname2 second relation name
     * @return bool success if relations are union compatible
     */
    bool unionCompatible(string relName1, string relName2);

    /**
     * relationUnion
     * @param string relName1 first relation name
     * @param string relname2 second relation name
     * @return Relation of union of relName1 and relName2
     */
    Relation relationUnion(string relName1, string relName2);

    /**
     * relationDifference
     * @param string relName1 first relation name
     * @param string relname2 second relation name
     * @return Relation of difference of relName1 and relName2
     */
    Relation relationDifference(string relName1, string relName2);

    /**
     * relationCrossProduct
     * @param string relName1 first relation name
     * @param string relname2 second relation name
     * @return Relation of cross product of relName1 and relName2
     */
    Relation relationCrossProduct(string relName1, string relName2);
};

#endif
