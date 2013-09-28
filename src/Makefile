CC=g++

all: planner parse parsetree

planner: Relation.o Attribute.o Database.o Parse.o Integration.h Planner.cpp
	$(CC) Relation.o Attribute.o Database.o Parse.o Planner.cpp -o planner
Relation.o: Relation.cpp Relation.h
	$(CC) -c Relation.cpp -o Relation.o
Attribute.o: Attribute.cpp Attribute.h
	$(CC) -c Attribute.cpp -o Attribute.o
Database.o: Database.cpp Database.h
	$(CC) -c Database.cpp -o Database.o

parse: Parser.o ParserTest.cpp
	$(CC) Parser.o ParserTest.cpp -o parse
parsetree: Parse.o ParseTest.cpp
	$(CC) Parse.o ParseTest.cpp -o parsetree
Parser.o: Parser.h Parser.cpp
	$(CC) -c Parser.cpp -o Parser.o
Parse.o: Parse.h Parse.cpp
	$(CC) -c Parse.cpp -o Parse.o

clean:
	rm -f parse parsetree *.o planner
