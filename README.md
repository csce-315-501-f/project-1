Christopher Murray, Kevin Vece, Juan Reyes  
CSCE 315-501
Project 1
========

Test Compilation Instructions
-----------------------------
The tests were compiled and tested with the included Makefile using the g++ compiler.
To compile and run the tests do the following (from project root):
<pre><code>cd tests/
make all
mainTest</code></pre>

Sample Output from the above commands:
<pre><code>ccmurray93@MacAir [~/Development/CSCE-315/project-1/tests]$make all
g++ AttributeTest.cpp ../src/Attribute.cpp RelationTest.cpp ../src/Relation.cpp DatabaseTest.cpp ../src/Database.cpp mainTest.cpp -o mainTest
ccmurray93@MacAir [~/Development/CSCE-315/project-1/tests]$./mainTest 
***************************
* Testing Attribute Class *
***************************
initTest:                          Passed
insertWithoutKeyTest:              Passed
insertWithKeyTest:                 Passed
insertWithValTooLongTest:          Passed
removeTest:                        Passed
updateTest:                        Passed
emptyAttributeTest:                Passed
emptyValueTest:                    Passed
findValueTest:                     Passed

**************************
* Testing Relation Class *
**************************
initTest:                  Passed
addAttributeTest:          Passed
addRowTest:                Passed
getRowTest:                Passed
getRowsWhereTest:          Passed
removeValueTest:           Passed
updateAttributeValueTest:  Passed
renameAttributeTest:       Passed
getAllRowsTest:            Passed

**************************
* Testing Database Class *
**************************
initTest:              Passed
addRelationTest:       Passed
addAttributeTest:      Passed
removeRelationTest:    Passed
removeAttributeTest:   Passed
addRowTest:            Passed
getRowsWhereTest:      Passed
renameRelationTest:    Passed
renameAttributeTest:   Passed
projectionTest:        

------------------------------------------------------------

/**** Original Relation ****/
testIntegerAttribute1  testIntegerAttribute2  
1                      2                      
/**** Projection with "testIntegerAttribute2"****/
testIntegerAttribute2  
2                      

------------------------------------------------------------
relationUnionTest:     

/**** Relation 1 ****/
testIntegerAttribute1  testIntegerAttribute2  
1                      2                      
3                      4                      

/**** Relation 2 ****/
testIntegerAttribute1  testIntegerAttribute2  
1                      2                      
4                      5                      

/**** 1 u 2 ****/
testIntegerAttribute1  testIntegerAttribute2  
1                      2                      
3                      4                      
4                      5                      
------------------------------------------------------------

relationDifferenceTest:

/**** Relation 1 ****/
testIntegerAttribute1  testIntegerAttribute2  
1                      2                      
3                      4                      

/**** Relation 2 ****/
testIntegerAttribute1  testIntegerAttribute2  
1                      2                      
4                      5                      

/**** 1 - 2 ****/
testIntegerAttribute1  testIntegerAttribute2  
3                      4                      

/**** 2 - 1 ****/
testIntegerAttribute1  testIntegerAttribute2  
4                      5                      
------------------------------------------------------------

relationCrossProductTest:

/**** Relation 1 ****/
testIntegerAttribute1  testStringAttribute1  
1                      testString1           
2                      testString2           

/**** Relation 2 ****/
testStringAttribute2  testIntegerAttribute2  
testString3           3                      
testString4           4                      

/**** 1 * 2 ****/
testIntegerAttribute1  testStringAttribute1  testStringAttribute2  testIntegerAttribute2  
1                      testString1           testString3           3                      
1                      testString1           testString4           4                      
2                      testString2           testString3           3                      
2                      testString2           testString4           4                      

/**** 2 * 1 ****/
testStringAttribute2  testIntegerAttribute2  testIntegerAttribute1  testStringAttribute1  
testString3           3                      1                      testString1           
testString3           3                      2                      testString2           
testString4           4                      1                      testString1           
testString4           4                      2                      testString2           
------------------------------------------------------------</code></pre>
