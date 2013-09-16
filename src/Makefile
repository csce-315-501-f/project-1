CC=g++

parse: Parser.o ParserTest.cpp
	$(CC) Parser.o ParserTest.cpp -o parse
parsetree: Parse.o ParseTest.cpp
	$(CC) Parse.o ParseTest.cpp -o parsetree
Parser.o: Parser.h Parser.cpp
	$(CC) -c Parser.cpp -o Parser.o
Parse.o: Parse.h Parse.cpp
	$(CC) -c Parse.cpp -o Parse.o

clean:
	rm -f parse parsetree Parser.o Parse.o
