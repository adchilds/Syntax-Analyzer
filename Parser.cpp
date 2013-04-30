/**
 * Adam Childs, Bell Lopez, Tiffany Flor
 * April 17, 2013
 *
 * PLEDGED: The code in this program represents our own original work.
 */
#include <iostream>
#include <fstream>
#include <cstdlib> // for exit() function
#include "Parser.h"

using namespace std;

Parser::Parser()
{
	string s; // String to parse

	cout << "Please enter a valid string to parse: " << endl;
	getline(cin, s);
	str = s;

	cout << str << endl;
}

Parser::Parser(string s)
{
	ifstream ifs(s.c_str());
	if (ifs.is_open())
	{
		str.assign(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
		ifs.close();
	} else
		str = s;

	cout << str << endl;
}

string Parser::getToken()
{
	// Check for " " and "\n"
	string::size_type v = str.find(" ");
	if (v == string::npos) // space not found, so look for \n
	{
		v = str.find("\n");
		if (v == string::npos) // newline not found, look for \t
			v = str.find("\t");
	}

	string temp = "";
	if (v != string::npos) // Space, tab, or newline found
	{
		temp = str.substr(0, v);
		str = str.substr((v+1), str.length());

		// Ensure that the string is not empty, if it is, return the last token
		if (str.length() == 0)
			return temp;

		// Check for more spaces, tabs, and newlines
		char t = lookahead();
		while(t == ' ' || t == '\n' || t == '\t')
		{
			// Remove t from str
			str = str.substr(1, str.length());

			if (str.length() > 0)
				t = lookahead();
			else
				break;
		}
	} else { // Space, tab, or newline NOT found
		temp = str;
		str = "";
	}

	return temp;
}

char Parser::lookahead()
{
	return str.at(0);
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
			funclist();
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

void Parser::funclist()
{
	string token = getToken();
	// check if function
	if (token == "func")
	{
		str = token + " " + str;
		function();
	}
	// check if dec
	else if (token == "any" || token == "def")
	{
		str = token + " " + str;
		dec();
	}
	// check if array
	else if (token == "arr")
	{
		str = token + " " + str;
		array();
	}
	// check if use
	else if (token == "use")
	{
		cout << "IN USE" << endl;
		str = token + " " + str;
		use();
	}
	// if nothing put token back
	else {
		str = token + " " + str;
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
	}
	else if (token == "any" || token == "def")
	{
		str = token + " " + str;
		dec();
		stmlist();
	}
	else if (token == "arr")
	{
		str = token + " " + str;
		array();
		stmlist();
	}
	else {
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
	string token = getToken();
        if (token == "elseif")
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

void Parser::else1()
{
	string token = getToken();
	if (token == "else")
	{
		token = getToken();
		if (token == "->")
		{
			stmlist();
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in else statement." << endl;
				exit(1);
			}
		} else {
			cout << "Missing -> in else statement." << endl;
		}
	}
}

void Parser::for1()
{
	string token = getToken();
	if (token == "for")
	{
		var();
		token = getToken();
		if (token == "=")
		{
			exp();
			token = getToken();
			if (token == "(")
			{
				inc();
				token = getToken();
				if (token == ")")
				{
					exp();
					token = getToken();
					if (token == "->")
					{
						stmlist();
						token = getToken();
						if (token != ";")
						{
							cout << "Missing ; in for statement." << endl;
							exit(1);
						}
					} else {
						cout << "Missing -> in for statement." << endl;
						exit(1);
					}
				} else {
					cout << "Missing ) in for statement." << endl;
					exit(1);
				}
			} else {
				cout << "Missing ( in for statement." << endl;
				exit(1);
			}
		} else {
			cout << "Missing = in for statement." << endl;
			exit(1);
		}
	}
}

void Parser::inc()
{
	string token = getToken();
	if (token == "+" || token == "-")
	{
		exp();
	} else {
		cout << "Invalid increment value." << endl;
		exit(1);
	}
}

void Parser::foreach()
{
	string token = getToken();
	if (token == "foreach")
	{
		var();
		token = getToken();
		if (token == "=")
		{
			exp();
			token = getToken();
			if (token == "in")
			{
				stmlist();
			} else {
				cout << "Missing \"in\" in foreach." << endl;
				exit(1);
			}
		} else {
			cout << "Missing = in foreach." << endl;
			exit(1);
		}
	}
}

void Parser::while1()
{
	string token = getToken();
	if (token == "while")
	{
		exp();
		token = getToken();
		if (token == "->")
		{
			stmlist();
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in while." << endl;
				exit(1);
			}
		} else {
			cout << "Missing -> in while." << endl;
			exit(1);
		}
	}
}

void Parser::exec()
{
	string token = getToken();
	if (token == "exec")
	{
		token = getToken();
		if (token == "->")
		{
			stmlist();
			token = getToken();
			if (token == ";")
			{
				token = getToken();
				if (token == "while")
				{
					exp();
					token = getToken();
					if (token != ";")
					{
						cout << "Missing ; in exec statement." << endl;
						exit(1);
					}
				} else {
					cout << "Missing \"while\" in exec statement." << endl;
					exit(1);
				}
			} else {
				cout << "Missing ; in exec statement." << endl;
				exit(1);
			}
		} else {
			cout << "Missing -> in exec statement." << endl;
			exit(1);
		}
	}
}

void Parser::return1()
{
	string token = getToken();
	if (token == "return")
	{
		token = getToken();
		if (token != ";")
		{
			str = token + " " + str;
			val();
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in return statement" << endl;
				exit(1);
			}
		}
	}
}

void Parser::use()
{
	string token = getToken();
	if (token == "use")
	{
		import();
		token = getToken();
		if (token != ";")
		{
			cout << "TOKEN: " << token << endl;
			cout << "Missing ; in use statement." << endl;
			exit(1);
		}
	}
}

void Parser::import()
{
	idword();
	string token = getToken();
	if (token == ".")
	{
		import();
	} else {
		str = token + " " + str;
	}
	cout << "IMPORT FINE!" << endl;
}

void Parser::exp()
{
	// HARDDDDD!
}

void Parser::var()
{
	string token = getToken();
	if (token == "any")
	{
		id();
		token = getToken();
		if (token == "=")
		{
			val();
		} else {
			str = token + " " + str;
		}
	}
	else if (token == "def")
	{
		id();
		token = getToken();
		if (token == "=")
		{
			val();
		} else {
			cout << "Missing = in def statement." << endl;
			exit(1);
		}
	} else {
		cout << "Incorrect syntax for variable declaration in function parameters." << endl;
		exit(1);
	}
}

void Parser::dec()
{
	string token = getToken();
        if (token == "any")
        {
                id();
                token = getToken();
                if (token == "=")
                {
                        val();
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in variable declaration." << endl;
				exit(1);
			}
                }
		else if (token != ";")
		{
                        cout << "Incorrect syntax for variable declaration." << endl;
			exit(1);
                }
        }
        else if (token == "def")
        {
                id();
                token = getToken();
                if (token == "=")
                {
                        val();
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in constant declaration." << endl;
				exit(1);
			}
                } else {
                        cout << "Missing = in constant declaration." << endl;
                        exit(1);
                }
        } else {
                cout << "Incorrect syntax for variable declaration." << endl;
                exit(1);
        }
}

void Parser::val()
{
	string token = getToken();
//	HARD!
}

void Parser::id()
{
	string token = getToken();

	cout << token << endl;
}

void Parser::array()
{

}

void Parser::idword()
{

}

void Parser::function()
{

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

bool Parser::character(string s)
{
	string valid = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

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
