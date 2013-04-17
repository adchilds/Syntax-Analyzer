all: syntax_analyzer

syntax_analyzer: main.o parser.o
	g++ main.o parser.o -o syntax_analyzer

main.o: main.cpp
	g++ -c main.cpp

parser.o: Parser.cpp
	g++ -c Parser.cpp

clean:
	rm -rf *o
