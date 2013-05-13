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
	if (argc > 1) // File supplied
	{
		cout << "FILE SUPPLIED: " << argv[1] << endl;

		Parser p(argv[1]);
		p.parse();
		p.translated();
	} else { // String supplied
		Parser p;
		p.parse();
		p.translated();
	}

	return 0;
}
