#ifndef DATABASE_TEST
#define DATABASE_TEST

class DatabaseTest {
    bool initTest();
    bool addRelationTest();
    bool addAttributeTest();
    bool removeRelationTest();
    bool removeAttributeTest();
    bool addRowTest();
    bool removeRowTest();
    bool getRowsWhereTest();
    void selectionTest();
    bool renameRelationTest();
    bool renameAttributeTest();
    void projectionTest();
    void relationUnionTest();
    void relationDifferenceTest();
    void relationCrossProductTest();
    void writeRelationTest();
public:
    void runAll();
};

#endif
