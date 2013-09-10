CC=g++
ATTR_HEADERS=../src/Attribute.h AttributeTest.h
ATTR_SRC=AttributeTest.cpp ../src/Attribute.cpp
RELATION_HEADERS=../src/Relation.h RelationTest.h
RELATION_SRC=RelationTest.cpp ../src/Relation.cpp
DATABASE_HEADERS=../src/Database.h DatabaseTest.h
DATABASE_SRC=DatabaseTest.cpp ../src/Database.cpp
MAIN_TEST_FILE=mainTest.cpp
ATTR_TEST_FILE=attributeMainTest.cpp
RELATION_TEST_FILE=relationMainTest.cpp
DATABASE_TEST_FILE=databaseMainTest.cpp

all: $(ATTR_HEADERS) $(ATTR_SRC) $(RELATION_HEADERS) $(RELATION_SRC) $(DATABASE_HEADERS) $(DATABASE_SRC) $(MAIN_TEST_FILE)
	$(CC) $(ATTR_SRC) $(RELATION_SRC) $(DATABASE_SRC) $(MAIN_TEST_FILE) -o mainTest

attributeTest: $(ATTR_HEADERS) $(ATTR_SRC) $(MAIN_TEST_FILE)
	$(CC) $(ATTR_SRC) $(ATTR_TEST_FILE) -o attributeTest

relationTest: $(ATTR_HEADERS) $(ATTR_SRC) $(RELATION_HEADERS) $(RELATION_SRC) $(RELATION_TEST_FILE)
	$(CC) $(ATTR_SRC) $(RELATION_SRC) $(RELATION_TEST_FILE) -o relationTest

databaseTest: $(ATTR_HEADERS) $(ATTR_SRC) $(RELATION_HEADERS) $(RELATION_SRC) $(DATABASE_HEADERS) $(DATABASE_SRC) $(DATABASE_TEST_FILE)
	$(CC) $(ATTR_SRC) $(RELATION_SRC) $(DATABASE_SRC) $(DATABASE_TEST_FILE) -o databaseTest

clean:
	rm attributeTest relationTest databaseTest mainTest