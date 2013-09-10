#include "../src/Database.h"
#include "DatabaseTest.h"

bool DatabaseTest::initTest() {
    Database database;
    return typeid(database) == typeid(Database);
}

bool DatabaseTest::addRelationTest() {
    Database database;
    bool success = database.addRelation("testRelation");
    return success;
}

bool DatabaseTest::addAttributeTest() {
    Database database;
    bool successAddRelation = database.addRelation("testRelation");
    bool successAddAttribute = database.addAttribute("testRelation", "testStringAttribute", "string", 10);
    return successAddAttribute && successAddRelation;
}

bool DatabaseTest::removeRelationTest() {
    Database database;
    database.addRelation("testRelation");
    bool successAddRelation = database.relationExists("testRelation");
    database.removeRelation("testRelation");
    bool successRemoveRelation = !database.relationExists("testRelation");
    return successAddRelation && successRemoveRelation;
}

bool DatabaseTest::removeAttributeTest() {
    Database database;
    database.addRelation("testRelation");
    database.addAttribute("testRelation", "testStringAttribute", "string", 10);
    bool successAddAttribute = database.attributeExists("testRelation","testStringAttribute");
    database.removeAttribute("testRelation","testStringAttribute");
    bool successRemoveAttribute = !database.attributeExists("testRelation","testStringAttribute");
    return successAddAttribute && successRemoveAttribute;
}

bool DatabaseTest::addRowTest() {
    Database database;
    database.addRelation("testRelation");
    database.addAttribute("testRelation", "testStringAttribute", "string", 10);
    vector<string> row;
    row.push_back("testAttr");
    bool success = database.addRow("testRelation",row) != -1;
    return success;
}

bool DatabaseTest::getRowsWhereTest() {
    Database database;
    database.addRelation("testRelation");
    database.addAttribute("testRelation", "testStringAttribute", "string", 10);
    vector<string> row;
    row.push_back("testAttr");
    database.addRow("testRelation",row) != -1;
    vector< vector<string> > rows = database.getRowsWhere("testRelation","testStringAttribute","testAttr");
    return rows[0] == row;
}

bool DatabaseTest::renameRelationTest() {
    Database database;
    bool successAddRelation = database.addRelation("testRelation");
    bool successRenameRelation = database.renameRelation("testRelation","renamedRelation");
    return successAddRelation && successRenameRelation;
}

bool DatabaseTest::renameAttributeTest() {
    Database database;
    database.addRelation("testRelation");
    database.addAttribute("testRelation","testIntegerAttribute","integer",0);
    bool successAddAttribute = database.attributeExists("testRelation", "testIntegerAttribute");
    bool successRenameAttribute = database.renameAttribute("testRelation", "testIntegerAttribute", "renamedIntegerAttribute");
    return successAddAttribute && successRenameAttribute;
}

void DatabaseTest::projectionTest() {
    Database database;
    
    // setup database projection will be taken of
    database.addRelation("testRelation");
    database.addAttribute("testRelation","testIntegerAttribute1","integer",0);
    database.addAttribute("testRelation","testIntegerAttribute2","integer",0);
    vector<string> row;
    row.push_back("1");
    row.push_back("2");
    database.addRow("testRelation",row);

    // do projection
    vector<string> rows;
    rows.push_back("testIntegerAttribute2");
    Relation projection = database.projection("testRelation",rows);

    cout << endl;
    cout << "/**** Original Relation ****/" << endl;
    database.show("testRelation");

    cout << "/**** Projection with \"testIntegerAttribute2\"****/" << endl;
    projection.show();
}

void DatabaseTest::relationUnionTest() {
    Database database;
    // setup relations union will be taken of
    database.addRelation("testRelation1");
    database.addAttribute("testRelation1","testIntegerAttribute1","integer",0);
    database.addAttribute("testRelation1","testIntegerAttribute2","integer",0);
    
    database.addRelation("testRelation2");
    database.addAttribute("testRelation2","testIntegerAttribute1","integer",0);
    database.addAttribute("testRelation2","testIntegerAttribute2","integer",0);

    vector<string> row1;
    row1.push_back("1");
    row1.push_back("2");

    vector<string> row2;
    row2.push_back("1");
    row2.push_back("2");

    vector<string> row3;
    row3.push_back("3");
    row3.push_back("4");

    vector<string> row4;
    row4.push_back("4");
    row4.push_back("5");

    database.addRow("testRelation1",row1);
    database.addRow("testRelation1",row3);
    database.addRow("testRelation2",row2);
    database.addRow("testRelation2",row4);

    cout << endl;
    cout << "/**** Relation 1 ****/" << endl;
    database.show("testRelation1");

    cout << endl;
    cout << "/**** Relation 2 ****/" << endl;
    database.show("testRelation2");

    Relation relation = database.relationUnion("testRelation1", "testRelation2");
    cout << endl;
    cout << "/**** 1 u 2 ****/" << endl;
    relation.show();
}

void DatabaseTest::relationDifferenceTest() {
    Database database;
    // setup relations difference will be taken of
    database.addRelation("testRelation1");
    database.addAttribute("testRelation1","testIntegerAttribute1","integer",0);
    database.addAttribute("testRelation1","testIntegerAttribute2","integer",0);
    
    database.addRelation("testRelation2");
    database.addAttribute("testRelation2","testIntegerAttribute1","integer",0);
    database.addAttribute("testRelation2","testIntegerAttribute2","integer",0);

    vector<string> row1;
    row1.push_back("1");
    row1.push_back("2");

    vector<string> row2;
    row2.push_back("1");
    row2.push_back("2");

    vector<string> row3;
    row3.push_back("3");
    row3.push_back("4");

    vector<string> row4;
    row4.push_back("4");
    row4.push_back("5");

    database.addRow("testRelation1",row1);
    database.addRow("testRelation1",row3);
    database.addRow("testRelation2",row2);
    database.addRow("testRelation2",row4);

    cout << endl;
    cout << "/**** Relation 1 ****/" << endl;
    database.show("testRelation1");

    cout << endl;
    cout << "/**** Relation 2 ****/" << endl;
    database.show("testRelation2");

    Relation relation1 = database.relationDifference("testRelation1", "testRelation2");
    cout << endl;
    cout << "/**** 1 - 2 ****/" << endl;
    relation1.show();

    Relation relation2 = database.relationDifference("testRelation2", "testRelation1");
    cout << endl;
    cout << "/**** 2 - 1 ****/" << endl;
    relation2.show();
}

void DatabaseTest::relationCrossProductTest() {
    Database database;
    // setup relations difference will be taken of
    database.addRelation("testRelation1");
    database.addAttribute("testRelation1","testIntegerAttribute1","integer",0);
    database.addAttribute("testRelation1","testStringAttribute1","string",20);
    
    database.addRelation("testRelation2");
    database.addAttribute("testRelation2","testStringAttribute2","string",20);
    database.addAttribute("testRelation2","testIntegerAttribute2","integer",0);

    // for testRelation1
    vector<string> row1;
    row1.push_back("1");
    row1.push_back("testString1");

    // for testRelation1
    vector<string> row2;
    row2.push_back("2");
    row2.push_back("testString2");

    // for testRelation2
    vector<string> row3;
    row3.push_back("testString3");
    row3.push_back("3");

    // for testRelation2
    vector<string> row4;
    row4.push_back("testString4");
    row4.push_back("4");

    database.addRow("testRelation1",row1);
    database.addRow("testRelation1",row2);
    database.addRow("testRelation2",row3);
    database.addRow("testRelation2",row4);

    cout << endl;
    cout << "/**** Relation 1 ****/" << endl;
    database.show("testRelation1");

    cout << endl;
    cout << "/**** Relation 2 ****/" << endl;
    database.show("testRelation2");

    Relation relation1 = database.relationCrossProduct("testRelation1", "testRelation2");
    cout << endl;
    cout << "/**** 1 * 2 ****/" << endl;
    relation1.show();

    Relation relation2 = database.relationCrossProduct("testRelation2", "testRelation1");
    cout << endl;
    cout << "/**** 2 * 1 ****/" << endl;
    relation2.show();
}

/**************************************************
 * Function to run all tests
 **************************************************/
void DatabaseTest::runAll() {
    cout << "**************************" << '\n'
         << "* Testing Database Class *" << '\n'
         << "**************************" << endl;
    cout << "initTest:              " << (initTest()?"Passed":"Failed") << endl;
    cout << "addRelationTest:       " << (addRelationTest()?"Passed":"Failed") << endl;
    cout << "addAttributeTest:      " << (addAttributeTest()?"Passed":"Failed") << endl;
    cout << "removeRelationTest:    " << (removeRelationTest()?"Passed":"Failed") << endl;
    cout << "removeAttributeTest:   " << (removeAttributeTest()?"Passed":"Failed") << endl;
    cout << "addRowTest:            " << (addRowTest()?"Passed":"Failed") << endl;
    cout << "getRowsWhereTest:      " << (getRowsWhereTest()?"Passed":"Failed") << endl;
    cout << "renameRelationTest:    " << (renameRelationTest()?"Passed":"Failed") << endl;
    cout << "renameAttributeTest:   " << (renameAttributeTest()?"Passed":"Failed") << endl;
    cout << "projectionTest:        " << endl;
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    projectionTest();
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "relationUnionTest:     " << endl;
    relationUnionTest();
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
    cout << "relationDifferenceTest:" << endl;
    relationDifferenceTest();
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
    cout << "relationCrossProductTest:" << endl;
    relationCrossProductTest();
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
}
