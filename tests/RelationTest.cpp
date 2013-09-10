#include "RelationTest.h"

/**************************************************
 * Object Initialization Tests
 **************************************************/
bool RelationTest::initTest() {
    Relation relation;
    return typeid(relation) == typeid(Relation);
}

bool RelationTest::addAttributeTest() {
    Relation relation;
    bool newAddition = relation.addAttribute("testAttribute","integer",0) == true;
    bool repeatAddition = relation.addAttribute("testAttribute","integer",0) == false;
    return newAddition && repeatAddition;
}

bool RelationTest::addRowTest() {
    Relation relation;
    relation.addAttribute("testIntegerAttribute","integer",0);
    relation.addAttribute("testIntegerAttribute2","integer",0);
    vector<string> testRow;
    testRow.push_back("19");
    testRow.push_back("11");
    bool success = relation.addRow(testRow) != -1;

    return success;
}

bool RelationTest::getRowTest() {
    Relation relation;
    relation.addAttribute("testIntegerAttribute","integer",0);
    relation.addAttribute("testIntegerAttribute2","integer",0);
    vector<string> testRow;
    testRow.push_back("19");
    testRow.push_back("11");
    relation.addRow(testRow);
    vector<string> row = relation.getRow(0);
    bool success = row[0] == "19" && row[1] == "11";
    return success;
}

bool RelationTest::getRowsWhereTest() {
    Relation relation;
    relation.addAttribute("testIntegerAttribute","integer",0);
    relation.addAttribute("testIntegerAttribute2","integer",0);
    vector<string> testRow;
    testRow.push_back("19");
    testRow.push_back("11");
    relation.addRow(testRow);

    vector< vector<string> > rows = relation.getRowsWhere("testIntegerAttribute2","11");
    return rows[0][0] == "19" && rows[0][1] == "11";
}

bool RelationTest::removeValueTest() {
    Relation relation;
    relation.addAttribute("testIntegerAttribute","integer",0);
    relation.addAttribute("testIntegerAttribute2","integer",0);
    vector<string> testRow;
    testRow.push_back("19");
    testRow.push_back("11");
    int insertedKey = relation.addRow(testRow) ;
    relation.removeRow(insertedKey);
    vector< vector<string> > rows = relation.getRowsWhere("testIntegerAttribute2","11");
    return rows.empty();
}

bool RelationTest::updateAttributeValueTest() {
    Relation relation;
    relation.addAttribute("testIntegerAttribute","integer",0);
    vector<string> testRow;
    testRow.push_back("19");
    int insertedKey = relation.addRow(testRow);
    relation.updateAttributeValue(insertedKey,"testIntegerAttribute","8");
    vector<string> row = relation.getRow(insertedKey);
    bool success = "8" == row[0];
    return success;
}

bool RelationTest::renameAttributeTest() {
    Relation relation;
    relation.addAttribute("testIntegerAttribute","integer",0);
    bool successAddAttribute = relation.attributeExists("testIntegerAttribute");
    bool successRenameAttribute = relation.renameAttribute("testIntegerAttribute","renamedIntegerAttribute");
    return successAddAttribute && successRenameAttribute;
}

bool RelationTest::getAllRowsTest() {
    Relation relation;
    relation.addAttribute("testIntegerAttribute","integer",0);
    relation.addAttribute("testIntegerAttribute2","integer",0);
    vector<string> testRow;
    testRow.push_back("19");
    testRow.push_back("11");
    relation.addRow(testRow);
    vector< vector<string> > rows = relation.getAllRows();
    return rows[0] == testRow;
}

/**************************************************
 * Function to run all tests
 **************************************************/
void RelationTest::runAll() {
    cout << "**************************" << '\n'
         << "* Testing Relation Class *" << '\n'
         << "**************************" << endl;
    cout << "initTest:                  " << (initTest()?"Passed":"Failed") << endl;
    cout << "addAttributeTest:          " << (addAttributeTest()?"Passed":"Failed") << endl;
    cout << "addRowTest:                " << (addRowTest()?"Passed":"Failed") << endl;
    cout << "getRowTest:                " << (getRowTest()?"Passed":"Failed") << endl;
    cout << "getRowsWhereTest:          " << (getRowsWhereTest()?"Passed":"Failed") << endl;
    cout << "removeValueTest:           " << (removeValueTest()?"Passed":"Failed") << endl;
    cout << "updateAttributeValueTest:  " << (updateAttributeValueTest()?"Passed":"Failed") << endl;
    cout << "renameAttributeTest:       " << (renameAttributeTest()?"Passed":"Failed") << endl;
    cout << "getAllRowsTest:            " << (getAllRowsTest()?"Passed":"Failed") << endl;
}
