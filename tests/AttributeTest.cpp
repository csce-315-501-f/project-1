#include "AttributeTest.h"
#include <typeinfo>

/**************************************************
 * Object Initialization Tests
 **************************************************/
bool AttributeTest::initIntegerTest() {
    Attribute<int> attribute;
    return typeid(attribute) == typeid(Attribute<int>);
}

bool AttributeTest::initStringTest() {
    Attribute<string> attribute;
    return typeid(attribute) == typeid(Attribute<string>);
}


/**************************************************
 * Function to run all tests
 **************************************************/
void AttributeTest::runAll() {
    cout << "***************************" << '\n'
         << "* Testing Attribute Class *" << '\n'
         << "***************************" << endl;
    cout << "initIntegerTest: " << (initIntegerTest()?"Passed":"Failed") << endl;
    cout << "initStringTest: " << (initStringTest()?"Passed":"Failed") << endl;
}
