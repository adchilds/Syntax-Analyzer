all: syntax_analyzer

syntax_analyzer: main.o Parser.o
	g++ main.o Parser.o -o syntax_analyzer

main.o: main.cpp
	g++ -c main.cpp

Parser.o: Parser.cpp
	g++ -c Parser.cpp

clean:
	rm -rf *o
