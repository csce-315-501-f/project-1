CC=g++

parse: Parser.o ParserTest.cpp
	$(CC) Parser.o ParserTest.cpp -o parse
Parser.o: Parser.h Parser.cpp
	$(CC) -c Parser.cpp -o Parser.o

clean:
	rm -f parse Parser.o
