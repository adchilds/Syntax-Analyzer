/**
 * Adam Childs
 * Feb. 26, 2013
 *
 * PLEDGED: The code in this program represents my own original work.
 */
#include <iostream>
#include <cstdlib> // for exit() function
#include "Parser.h"

using namespace std;

Parser::Parser(string s)
{
	str = s;
}

string Parser::getToken()
{
	// Check for " " and "\n"
	string::size_type v = str.find(" ");
	if (v == string::npos) // space not found, so look for \n
		v = str.find("\n");

	string temp = "";
	if (v != string::npos) // Space or newline found
	{
		temp = str.substr(0, v);
		str = str.substr((v+1), str.length());
	} else { // Space or newline NOT found
		temp = str;
		str = "";
	}
	return temp;
}

void Parser::parse()
{
	program();
}

void Parser::program()
{
	obj();
}

void Parser::obj()
{
	string token = getToken();
	cout << token << endl;
	if (token == "obj")
	{
		id();
		token = getToken();
		cout << token << endl;
		if (token == "->")
		{
			stmlist();
			token = getToken();
			cout << token << endl;
			if (token != ";")
			{
				cout << "; is missing in object definition." << endl;
				exit(1);
			}
		} else {
			cout << "-> is missing in object definition." << endl;
			exit(1);
		}
	} else {
		cout << "Object definition missing." << endl;
		exit(1);
	}
}

void Parser::stm()
{
	string token = getToken();
	if (token == "if")
	{
		str = token + " " + str;
		if1();
	}
	else if (token == "for")
	{
		str = token + " " + str;
		for1();
	}
	else if (token == "foreach")
	{
		str = token + " " + str;
		foreach();
	}
	else if (token == "while")
	{
		str = token + " " + str;
		while1();
	}
	else if (token == "exec")
	{
		str = token + " " + str;
		exec();
	}
	else if (token == "return")
	{
		str = token + " " + str;
		return1();
	}
	else if (token == "use")
	{
		str = token + " " + str;
		use();
	} else {
		cout << "No valid statement found." << endl;
		exit(1);
	}
}

void Parser::stmlist()
{
	string token = getToken();
	if (token == "if" || token == "for" || token == "foreach" || token == "while" || token == "exec"
			|| token == "return" || token == "use")
	{
		str = token + " " + str;
		stm();
		stmlist();
	} else {
		str = token + " " + str;
	}
}

void Parser::if1()
{
	string token = getToken();
	if (token == "if")
	{
		exp();
		token = getToken();
		if (token == "->")
		{
			stmlist();
			token = getToken();
			if (token == ";")
			{
				token = getToken();
				if (token == "elseif")
				{
					str = token + " " + str;
					elseif();
				}
				else if (token == "else")
				{
					str = token + " " + str;
					else1();
				}
				str = token + " " + str;
			} else {
				cout << "Missing ; in if statement." << endl;
				exit(1);
			}
		} else {
			cout << "Missing -> in if statement." << endl;
			exit(1);
		}
	}
}

void Parser::elseif()
{

}

void Parser::else1()
{

}

void Parser::for1()
{

}

void Parser::foreach()
{

}

void Parser::while1()
{

}

void Parser::exec()
{

}

void Parser::return1()
{

}

void Parser::use()
{

}

void Parser::exp()
{

}

void Parser::id()
{
	string token = getToken();

	cout << token << endl;
}


bool Parser::num(string s)
{
	string valid = "0123456789";

	if (s.length() == 0)
		return false;

	if (string::npos != valid.find(s))
		return true;
	return false;
}

bool Parser::var(string s)
{
	string valid = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if (s.length() == 0)
		return false;

	if (string::npos != valid.find(s))
		return true;
	return false;
}

void Parser::print()
{
	cout << str << endl;
}
