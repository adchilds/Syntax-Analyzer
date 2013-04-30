/**
 * Adam Childs, Bell Lopez, Tiffany Flor
 * April 15, 2013
 *
 * PLEDGED: The code in this program represents our own original work.
 */

#include <string>

#ifndef PARSER_H
#define PARSER_H

using namespace std;

class Parser
{
	public:
		/**
		 * The Parser class contains helper functions that
		 * verify that the given string is formatted
		 * correctly for the language's grammar.
		 *
		 * @param string: The string to parse
		 */
		Parser(string);

		/**
		 * The Parser class contains helper functions that
		 * verify if the given string is formatted
		 * correctly for the language's grammer.
		 *
		 * @param ifstream: The filename of the file to parse
		 */
		Parser(ifstream);

		/**
		 * Parses the Parser instance's string to make sure that it
		 * follows the specified grammar for the language.
		 */
		void parse();

		/**
		 * Prints the string associated with the Parser instance.
		 */
		void print();

	private:
		/**
		 * Gets the next available token in the string. Essentially
		 * takes the characters from index 0 to the first occurrence
		 * of whitespace.
		 */
		string getToken();

		char lookahead();

		// CFG functions
		void program();

		void obj();

		void stm();

		void funclist();

		void stmlist();

		void if1();

		void elseif();

		void else1();

		void for1();

		void inc();

		void foreach();

		void while1();

		void exec();

		void return1();

		void use();

		void import();

		void exp();

		void var();

		void dec();

		void val();

		void num();

		void integer();

		void float1();

		void character();

		void symbols();

		void idword();

		void stringword();

		void string1();

		void boolean();

		void id();

		void parameters();

		void function();

		void array();

		void dimens();

		void multiarr();


		/**
		 * Checks to see if the given number matches a valid value
		 * supplied by the context-free grammar.
		 *
		 * @param string: the value to match against
		 */
		bool num(string);

		/**
		 * Checks to see if the given string matches a valid variable
		 * identifier supplied by the context-free grammar.
		 *
		 * @param string: the variable to match against
		 */
		bool character(string);

		string str; // The string to be parsed
};

#endif
