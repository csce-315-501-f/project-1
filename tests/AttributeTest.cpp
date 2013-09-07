#include "AttributeTest.h"

/**************************************************
 * Object Initialization Tests
 **************************************************/
bool AttributeTest::initTest() {
    Attribute attribute("string",10);
    return typeid(attribute) == typeid(Attribute);
}

bool AttributeTest::insertWithoutKeyTest() {
    Attribute attribute("string",10);
    int insertedKey = attribute.addValue("test value");
    return "test value" == attribute.getValue(insertedKey);
}

bool AttributeTest::insertWithKeyTest() {
    Attribute attribute("string",12);
    int insertedKey0 = attribute.addValue("test value 1");
    int insertedKey1 = attribute.addValue("test value 2");
    int insertedKey2 = attribute.addValue("test value 3");
    int insertedKey3 = attribute.addValue("test value 4");

    attribute.removeValue(2);

    int insertedKey = attribute.addValue("test value",2);
    if (insertedKey == -1) return false;
    else return "test value" == attribute.getValue(2);
}

bool AttributeTest::insertWithValTooLongTest() {
    Attribute attribute("string",5);
    int insertedKey = attribute.addValue("test value");
    return insertedKey == -1;
}

bool AttributeTest::removeTest() {
    Attribute attribute("string",10);
    int insertedKey = attribute.addValue("test value");
    attribute.removeValue(insertedKey);
    return attribute.getValue(insertedKey) == "";
}

bool AttributeTest::updateTest() {
    Attribute attribute("string",10);
    int insertedKey = attribute.addValue("test value");
    bool success = attribute.updateValue(insertedKey, "updated");
    return success && attribute.getValue(insertedKey) == "updated";
}

bool AttributeTest::emptyAttributeTest() {
    Attribute emptyAttribute;
    bool empty = emptyAttribute.isEmpty() == true;
    Attribute notEmptyAttribute("string",10);
    bool notEmpty = notEmptyAttribute.isEmpty() == false;
    return empty && notEmpty;
}

bool AttributeTest::emptyValueTest() {
    Attribute attribute("string",10);
    int insertedKey = attribute.addValue("test value");
    bool notEmpty = attribute.isEmpty(insertedKey) == false;
    attribute.removeValue(insertedKey);
    bool empty = attribute.isEmpty(insertedKey) == true;
    return notEmpty && empty;
}

bool AttributeTest::findValueTest() {
    Attribute attribute("string",10);
    int insertedKey = attribute.addValue("test value");
    int foundKey = attribute.findValue("test value").front();
    return insertedKey == foundKey;
}

/**************************************************
 * Function to run all tests
 **************************************************/
void AttributeTest::runAll() {
    cout << "***************************" << '\n'
         << "* Testing Attribute Class *" << '\n'
         << "***************************" << endl;
    cout << "initTest:                          " << (initTest()?"Passed":"Failed") << endl;
    cout << "insertWithoutKeyTest:              " << (insertWithoutKeyTest()?"Passed":"Failed") << endl;
    cout << "insertWithKeyTest:                 " << (insertWithKeyTest()?"Passed":"Failed") << endl;
    cout << "insertWithValTooLongTest:          " << (insertWithValTooLongTest()?"Passed":"Failed") << endl;
    cout << "removeTest:                        " << (removeTest()?"Passed":"Failed") << endl;
    cout << "updateTest:                        " << (updateTest()?"Passed":"Failed") << endl;
    cout << "emptyAttributeTest:                " << (emptyAttributeTest()?"Passed":"Failed") << endl;
    cout << "emptyValueTest:                    " << (emptyValueTest()?"Passed":"Failed") << endl;
    cout << "findValueTest:                     " << (findValueTest()?"Passed":"Failed") << endl;
}
