#ifndef ATTRIBUTE_TEST
#define ATTRIBUTE_TEST

#include <iostream>
#include "../src/Attribute.h"
#include <typeinfo>
using namespace std;

class AttributeTest {
    bool initTest();
    bool insertWithoutKeyTest();
    bool insertWithKeyTest();
    bool insertWithValTooLongTest();
    bool removeTest();
    bool updateTest();
    bool emptyAttributeTest();
    bool emptyValueTest();
    bool findValueTest();
public:
    void runAll();
};

#endif
