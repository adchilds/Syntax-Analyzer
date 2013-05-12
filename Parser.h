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
		 * This constructor is used for parsing a string
		 * from the command line, entered at runtime.
		 */
		Parser();

		/**
		 * The Parser class contains helper functions that
		 * verify that the given string is formatted
		 * correctly for the language's grammar.
		 *
		 * This constructor is used for parsing a string
		 * from a file with the ".name" extension.
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
		 * Finds the minimum value between three integers.
		 *
		 * @return the smaller value
		 */
		int min(int, int, int);

		/**
		 * Gets the next available token in the string. Essentially
		 * takes the characters from index 0 to the first occurrence
		 * of whitespace.
		 */
		string getToken();

		string get_temp_token();

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

		void uses();

		void use();

		void import();

		void assign();

		void exp();

		void arithexp();

		void boolexp();

		void stringexp();

		void T();

		void U();

		void X();

		void V();

		void Z();

		void arithval();

		void var();

		void dec();

		void num();

		void integer();

		void float1();

		void character();

		/**
		 * Checks to see if the given symbol(s) match a valid symbol
		 */
		bool is_symbol(string);

		void stringword();

		void string1();

		void id();

		void parameters();

		void function();

		void array();

		void dimens();

		void multiarr();

		void method();

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

		/**
		 * Checks to see if the given string matches a valid symbol
		 * for a character.
		 */
		bool symbols(string);

		/**
		 * Checks to see if the given string has the format of a
		 * string allowed by the grammar.
		 */
		bool is_string(string);

		/**
		 * Checks to see if the given string is an integer.
		 *
		 * @param string: the string to match against
		 */
		bool is_integer(string);

		/**
		 * Checks to see if the given string is a float.
		 *
		 * @param string: the string to match against
		 */
		bool is_float(string);

		/**
		 * Checks to see if the given string is a boolean
		 * value (<code>true</code> or <code>false</code>
		 *
		 * @param string: the string to match against
		 */
		bool is_boolean(string);

		string str; // The string to be parsed

		string translation; // The string that holds the Java equivalent of our code

		string temp_translation; // Holds temporary translation values

		string temp_id; // Holds the temporary id of a variable
};

#endif
