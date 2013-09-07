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
    bool success = relation.addRow(testRow);

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
    bool successAddRow = relation.addRow(testRow);
    relation.removeRow(0);
    vector< vector<string> > rows = relation.getRowsWhere("testIntegerAttribute2","11");
    return rows.empty();
}

/**************************************************
 * Function to run all tests
 **************************************************/
void RelationTest::runAll() {
    cout << "**************************" << '\n'
         << "* Testing Relation Class *" << '\n'
         << "**************************" << endl;
    cout << "initTest:         " << (initTest()?"Passed":"Failed") << endl;
    cout << "addAttributeTest: " << (addAttributeTest()?"Passed":"Failed") << endl;
    cout << "addRowTest:       " << (addRowTest()?"Passed":"Failed") << endl;
    cout << "getRowTest:       " << (getRowTest()?"Passed":"Failed") << endl;
    cout << "getRowsWhereTest: " << (getRowsWhereTest()?"Passed":"Failed") << endl;
    cout << "removeValueTest:  " << (removeValueTest()?"Passed":"Failed") << endl;
}
