#ifndef RELATION_TEST
#define RELATION_TEST

#include <iostream>
#include "../src/Relation.h"
#include <typeinfo>
#include <vector>
using namespace std;

class RelationTest {
    bool initTest();
    bool addAttributeTest();
    bool addRowTest();
    bool getRowTest();
    bool getRowsWhereTest();
    bool removeValueTest();
    bool updateAttributeValueTest();
    bool renameAttributeTest();
    bool getAllRowsTest();

public:
    void runAll();
};


#endif
