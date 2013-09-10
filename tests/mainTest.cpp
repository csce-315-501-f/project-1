#include <iostream>
#include "DatabaseTest.h"
#include "RelationTest.h"
#include "AttributeTest.h"

using namespace std;

int main() {

    AttributeTest attributeTest;
    attributeTest.runAll();

    cout << endl;

    RelationTest relationTest;
    relationTest.runAll();

    cout << endl;

    DatabaseTest databaseTest;
    databaseTest.runAll();

    return 0;
}
