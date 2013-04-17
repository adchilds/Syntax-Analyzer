/**
 * Adam Childs, Bell Lopez, Tiffany Flor
 * April 15, 2013
 *
 * PLEDGED: The code in the program represents our own original work.
 */
#include <iostream>
#include <string>
#include "Parser.h"

using namespace std;		// uses standard library space

int main(int argc, const char* argv[])
{
	string s; // String to parse

	if (argc > 1) // File supplied
	{
		cout << "FILE SUPPLIED: " << argv[1] << endl;
		cout << "Argument count: " << (argc-1) << endl;
	} else { // String supplied
		cout << "Please enter a valid string to parse:" << endl;
		getline(cin, s);

		Parser p(s);
		p.parse();
	}

	return 0;
}
