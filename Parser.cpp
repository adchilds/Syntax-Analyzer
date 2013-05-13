/**
 * Adam Childs, Bell Lopez, Tiffany Flor
 * April 17, 2013
 *
 * PLEDGED: The code in this program represents our own original work.
 */
#include <iostream>
#include <fstream>
#include <cstdlib> // for exit() function
#include <ctype.h>
#include "Parser.h"

using namespace std;

Parser::Parser()
{
	translation = "";
	temp_translation = "";
	temp_id = "";
	temp_return = "";
	temp_statements = "";
	temp_parameters = "";

	string s; // String to parse

	cout << "Please enter a valid string to parse: " << endl;
	getline(cin, s);
	str = s;

	cout << str << endl;
}

Parser::Parser(string s)
{
	translation = "";
	temp_translation = "";
	temp_id = "";
	temp_return = "";
	temp_statements = "";
	temp_parameters = "";

	ifstream ifs(s.c_str());
	if (ifs.is_open())
	{
		str.assign(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
		ifs.close();
	} else
		str = s;

	cout << str << endl;
}

void Parser::translated()
{
	cout << translation << endl;
}

int Parser::min(int a, int b, int c)
{
	if (a == -1 && b == -1 && c == -1)
		return -1;
	if (a == -1)
		a = 200000000;
	if (b == -1)
		b = 200000000;
	if (c == -1)
		c = 200000000;

	if (a < b && a < c)
		return a;
	else if (b < c)
		return b;
	return c;
}

string Parser::get_temp_token(string* s)
{
	// Check for " " and "\n"
	int space = s->find(" ");
	int tab = s->find("\t");
	int nl = s->find("\n");

	int v = min(space, tab, nl);

	string temp = "";
	if (v != string::npos) // Space, tab, or newline found
	{
		temp = s->substr(0, v);
		*s = s->substr((v+1), s->length());

		// Ensure that the string is not empty, if it is, return the last token
		if (s->length() == 0)
		{
//			cout << temp << endl;
			return temp;
		}
		// Check for more spaces, tabs, and newlines
		char t = lookahead();
		while(isspace(t))
		{
			// Remove t from str
			*s = s->substr(1, s->length());

			if (s->length() > 0)
				t = lookahead();
			else
				break;
		}
	} else { // Space, tab, or newline NOT found
		temp = *s;
		*s = "";
	}

//	cout << temp << endl;
	return temp;
}

string Parser::getToken()
{
	// Check for " " and "\n"
	int space = str.find(" ");
	int tab = str.find("\t");
	int nl = str.find("\n");

	int v = min(space, tab, nl);

	string temp = "";
	if (v != string::npos) // Space, tab, or newline found
	{
		temp = str.substr(0, v);
		str = str.substr((v+1), str.length());

		// Ensure that the string is not empty, if it is, return the last token
		if (str.length() == 0)
		{
//			cout << temp << endl;
			return temp;
		}
		// Check for more spaces, tabs, and newlines
		char t = lookahead();
		while(isspace(t))
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

//	cout << temp << endl;
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
	uses();
	obj();
}

void Parser::uses()
{
	string token = getToken();
	if (token == "use")
	{
		translation.append("import ");
		use();
		uses();
	} else
		str = token + " " + str;
}

void Parser::obj()
{
	string token = getToken();
	if (token == "obj")
	{
		translation.append("class ");
		id();
		translation.append(temp_id); // Class name
		token = getToken();
		if (token == "->")
		{
			translation.append("{ "); // Start class
			token = getToken();
			while (token == "func" || token == "any" || token == "def")
			{
				str = token + " " + str;
				if (token == "any" || token == "def")
					dec();
				else
					funclist();
				token = getToken();
			}
			str = token + " " + str;
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in object definition." << endl;
				exit(1);
			}
		} else {
			cout << "Missing -> in object definition." << endl;
			exit(1);
		}
	} else {
		cout << "Object definition missing." << endl;
		exit(1);
	}
	translation.append("}"); // End class
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
	} else {
		// Check if it's an assignment statement
		string temp = getToken();
		if (temp == "=")
		{
			str = temp + " " + str;
			str = token + " " + str;
			assign();
		} else {
			cout << "No valid statement found." << endl;
			exit(1);
		}
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
	// if nothing put token back
	else {
		// Check for assignment statement
		string temp = getToken();
		if (temp == "=")
		{
			str = temp + " " + str;
			str = token + " " + str;
			assign();
		} else {
			str = temp + " " + str;
			str = token + " " + str;
		}
	}
}

void Parser::stmlist()
{
	string token = getToken();
	if (token == "if" || token == "for" || token == "foreach" || token == "while" || token == "exec"
			|| token == "return")
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
		// Check if it's an assignment statement
		string temp = getToken();
		if (temp == "=")
		{
			str = temp + " " + str;
			str = token + " " + str;
			assign();
			stmlist();
		}
		// Check if it's a method call
		else if (temp == "(" || temp == ".")
		{
			str = temp + " " + str;
			str = token + " " + str;
			method();
			stmlist();
		} else {
			str = temp + " " + str;
			str = token + " " + str;
		}
	}
}

void Parser::if1()
{
	string token = getToken();
	if (token == "if")
	{
		translation.append("if (");
		boolexp();
		translation.append(temp_translation);
		temp_translation = "";
		token = getToken();
		if (token == "->")
		{
			translation.append(") { ");
			token = getToken();
			string temp = getToken();
			while (token == "if" || token == "for" || token == "foreach" || token == "while" ||
				token == "exec" || token == "return" || token == "any" || token == "def" ||
				token == "arr" || temp == "=" || temp == "(" || temp == ".")
			{
				str = temp + " " + str;
				str = token + " " + str;
				stmlist();
				token = getToken();
				temp = getToken();
			}
			str = temp + " " + str;
			str = token + " " + str;
			token = getToken();
			if (token == ";")
			{
				translation.append(" } ");
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
				} else {
					str = token + " " + str;
				}
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
		translation.append("else if (");
		boolexp();
		translation.append(temp_translation);
		temp_translation = "";
		token = getToken();
		if (token == "->")
		{
			translation.append(") { ");
			token = getToken();
			string temp = getToken();
			while (token == "if" || token == "for" || token == "foreach" || token == "while" ||
					token == "exec" || token == "return" || token == "any" || token == "def" ||
					token == "arr" || temp == "=" || temp == "(" || temp == ".")
			{
				str = temp + " " + str;
				str = token + " " + str;
				stmlist();
				token = getToken();
				temp = getToken();
			}
			str = temp + " " + str;
			str = token + " " + str;
			token = getToken();
			if (token == ";")
			{
				translation.append(" } ");
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
				} else {
					str = token + " " + str;
				}
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
		translation.append("else ");
		token = getToken();
		if (token == "->")
		{
			translation.append("{ ");
			token = getToken();
			string temp = getToken();
			while (token == "if" || token == "for" || token == "foreach" || token == "while" ||
				token == "exec" || token == "return" || token == "any" || token == "def" ||
				token == "arr" || temp == "=" || temp == "(" || temp == ".")
			{
				str = temp + " " + str;
				str = token + " " + str;
				stmlist();
				token = getToken();
				temp = getToken();
			}
			str = temp + " " + str;
			str = token + " " + str;
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in else statement." << endl;
				exit(1);
			}
			translation.append(" } ");
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
		translation.append("for (");
		token = getToken(); // any or def
		string temp = getToken(); // name
		string temp2 = getToken(); // =
		string temp3 = getToken(); // variable
		// Put them back
		str = token + " " + temp + " " + temp2 + " " + temp3 + " " + str;
		if (is_integer(temp3))
			translation.append("int " + temp);
		else if (is_float(temp3))
		{
			cout << "Error: cannot loop over a floating point value." << endl;
			exit(1);
		}
		else if (is_boolean(temp3))
		{
			cout << "Error: cannot loop over a boolean value." << endl;
			exit(1);
		}
		else if (is_string(temp3))
		{
			cout << "Error: cannot loop over a string constant." << endl;
			exit(1);
		}
		var();
		token = getToken();
		if (token == "=")
		{
			translation.append(" = ");
			exp();
			translation.append(temp_translation + "; ");
			temp_translation = "";
			token = getToken();
			if (token == "(")
			{
				token = getToken();
				bool plus = true;
				if (token == "-")
					plus = false;
				str = token + " " + str;
				inc();
				string incr = temp_translation;
				temp_translation = "";
				token = getToken();
				if (token == ")")
				{
					// Condition
					translation.append(temp + " <= ");
					exp();
					translation.append(temp_translation + "; ");
					temp_translation = "";

					// Increment
					if (plus)
						translation.append(temp + "+=" + incr + ")");
					else
						translation.append(temp + "-=" + incr + ")");

					token = getToken();
					if (token == "->")
					{
						translation.append(" { ");
						token = getToken();
						string temp = getToken();
						while (token == "if" || token == "for" || token == "foreach" || token == "while" ||
							token == "exec" || token == "return" || token == "any" || token == "def" ||
							token == "arr" || temp == "=" || temp == "(" || temp == ".")
						{
							str = temp + " " + str;
							str = token + " " + str;
							stmlist();
							token = getToken();
							temp = getToken();
						}
						str = temp + " " + str;
						str = token + " " + str;
						token = getToken();
						if (token != ";")
						{
							cout << "Missing ; in for statement." << endl;
							exit(1);
						}
						translation.append(" } ");
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
		translation.append("for (");
		token = getToken(); // any or def
		string temp = getToken(); // name
		string temp2 = getToken(); // =
		string temp3 = getToken(); // variable
		// Put them back
		str = token + " " + temp + " " + temp2 + " " + temp3 + " " + str;
		if (is_integer(temp3))
			translation.append("int " + temp + " : ");
		else if (is_float(temp3))
			translation.append("float " + temp + " : ");
		else if (is_boolean(temp3))
			translation.append("boolean " + temp + " : ");
		else if (is_string(temp3))
			translation.append("String " + temp + " : ");
		var();
		token = getToken();
		if (token == "=")
		{
			exp();
			token = getToken();
			if (token == "in")
			{
				id();
				translation.append(temp_id + ")");
				temp_translation = "";
				token = getToken();
				if (token == "->")
				{
					translation.append(" { ");
					token = getToken();
					string temp = getToken();
					while (token == "if" || token == "for" || token == "foreach" || token == "while" ||
							token == "exec" || token == "return" || token == "any" || token == "def" ||
							token == "arr" || temp == "=" || temp == "(" || temp == ".")
					{
						str = temp + " " + str;
						str = token + " " + str;
						stmlist();
						token = getToken();
						temp = getToken();
					}
					str = temp + " " + str;
					str = token + " " + str;
					token = getToken();
					if (token != ";")
					{
						cout << "Missing ; in foreach." << endl;
						exit(1);
					}
					translation.append(" } ");
				} else {
					cout << "Missing -> in foreach." << endl;
					exit(1);
				}
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
		translation.append("while (");
		boolexp();
		translation.append(temp_translation);
		temp_translation = "";
		token = getToken();
		if (token == "->")
		{
			translation.append(") { ");
			token = getToken();
			string temp = getToken();
			while (token == "if" || token == "for" || token == "foreach" || token == "while" ||
					token == "exec" || token == "return" || token == "any" || token == "def" ||
					token == "arr" || temp == "=" || temp == "(" || temp == ".")
			{
				str = temp + " " + str;
				str = token + " " + str;
				stmlist();
				token = getToken();
				temp = getToken();
			}
			str = temp + " " + str;
			str = token + " " + str;
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in while." << endl;
				exit(1);
			}
			translation.append(" } ");
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
		translation.append("do { ");
		token = getToken();
		if (token == "->")
		{
			token = getToken();
			string temp = getToken();
			while (token == "if" || token == "for" || token == "foreach" || token == "while" ||
					token == "exec" || token == "return" || token == "any" || token == "def" ||
					token == "arr" || temp == "=" || temp == "(" || temp == ".")
			{
				str = temp + " " + str;
				str = token + " " + str;
				stmlist();
				token = getToken();
				temp = getToken();
			}
			str = temp + " " + str;
			str = token + " " + str;
			token = getToken();
			if (token == ";")
			{
				translation.append( " } ");
				token = getToken();
				if (token == "while")
				{
					translation.append("while (");
					boolexp();
					translation.append(temp_translation);
					temp_translation = "";
					token = getToken();
					if (token != ";")
					{
						cout << "Missing ; in exec statement." << endl;
						exit(1);
					}
					translation.append("); ");
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
			string temp = getToken();
			str = temp + " " + str;
			str = token + " " + str;
			if (temp == ";")
			{
				if (is_integer(token) || is_float(token) || is_boolean(token))
				{
					exp();
					temp_return = "return " + temp_translation + ";";
				} else {
					id();
					temp_return = "return " + temp_id + ";";
				}
				temp_statements.append(temp_return + " ");
			} else {
				exp();
				temp_return = "return " + temp_translation + ";";
				temp_statements.append(temp_return + " ");
			}
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in return statement" << endl;
				exit(1);
			}
		}
	}
	if (temp_return.length() <= 0)
	{
		temp_return = "return;";
		temp_statements.append(temp_return + " ");
	}
}

void Parser::use()
{
	string token = getToken();
	if (character(string(1, token[0])))
	{
		for (int i = 1; i < token.length(); i++)
		{
			if (!character(string(1, token[i])) && token[i] != '.')
			{
				cout << "Invalid import: " << token << endl;
				exit(1);
			}
		}
		translation.append(token);
	} else {
		cout << "Invalid import: " << token << endl;
		exit(1);
	}
	token = getToken();
	if (token != ";")
	{
		cout << "Missing ; in use statement." << endl;
		exit(1);
	}
	translation.append(token + " ");
}

void Parser::assign()
{
	id();

	string token = getToken();
	if (token == "=")
	{
		token = getToken();
		if (character(token) || token[0] == '\"')
		{
			cout << "PUTTING BACK: " << token << endl;
			str = token + " " + str; // Put back for stringexp -> string1
			stringexp();
			token = getToken();
			if (token != ";")
			{
				cout << "TOKEN: " << token << endl;
				cout << "Missing ; in assignment." << endl;
				exit(1);
			}
		} else {
			str = token + " " + str; // Put back for exp
			exp();
			token = getToken();
			if (token !=  ";")
			{
				cout << "Missing ; in assignment." << endl;
				exit(1);
			}
		}
	} else {
		cout << "Missing = in assignment." << endl;
		exit(1);
	}
}

void Parser::method()
{
	id();
	string t = temp_id;
	t = t.substr(0, t.length()-1);
	translation.append(t);
	temp_id = "";
	string token = getToken();
    if (token == "(")
	{
		translation.append("(");
		token = getToken();
		if (token == ")")
		{
			translation.append(")");
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in method call." << endl;
				exit(1);
			}
			translation.append("; ");
		} else {
			str = token + " " + str;
			parameters();
			token = getToken();
			if (token == ")")
			{
				translation.append(")");
				token = getToken();
				if (token != ";")
				{
					cout << "Missing ; in method call." << endl;
					exit(1);
				}
				translation.append("; ");
			}
		}
	}
	else if (token == ".")
	{
		translation.append(".");
		method();
	}
}

void Parser::exp()
{
	string token = getToken();
	if (is_integer(token) || is_float(token) || character(token))
	{
		temp_translation.append(token + " ");
		str = token + " " + str;
		arithexp();
	}
	else if (is_boolean(token) || character(token))
	{
		temp_translation.append(token);
		str = token + " " + str;
		boolexp();
	}
	else if (is_string(token) || character(token))
	{
		temp_translation.append(token);
		str = token + " " + str;
		stringexp();
	} else {
		cout << "Invalid expression: " << token << endl;
		exit(1);
	}
}

void Parser::T()
{
	string token = getToken();
	if (token == "+" || token == "-" || token == "*" || token == "/")
	{
		temp_translation.append(token + " ");
		exp();
	}
}

void Parser::U()
{
	string token = getToken();
	if (token == "==" || token == "!=" || token == ">"
		|| token == "<" || token == "<=" || token == ">=")
	{
		temp_translation.append(token + " ");
		exp();
	}
}

void Parser::V()
{
	string token = getToken();
	if (token == "+" || token == "-" || token == "*" || token == "/")
	{
		str = token + " " + str;
		T();
		V();
	} else
		str = token + " " + str;
}

void Parser::X()
{
	string token = getToken();
	if (token == "==" || token == "!=" || token == ">" || token == "<"
		|| token == ">=" || token == "<=")
	{
		str = token + " " + str;
		U();
		X();
	} else
		str = token + " " + str;
}

void Parser::Z()
{
	string token = getToken();
	if (token == "+" || token == "-" || token == "*" || token == "/")
	{
		T();
		V();
		U();
		X();
		Z();
	} else
		str = token + " " + str;
}

void Parser::arithexp()
{
	string token = getToken();
	if (is_integer(token) || is_float(token) || character(token))
	{
		V();
	} else {
		str = token + " " + str; // Put the token back (not an int or float)
		boolexp();
		T();
		V();
	}
}

void Parser::boolexp()
{
	string token = getToken();
	if (is_boolean(token) || character(token))
	{
		temp_translation.append( " " + token + " ");
		X();
		Z();
	} else {
		str = token + " " + str; // Put the token back (not a boolean)
		V();
		U();
		X();
		Z();
	}
}

void Parser::stringexp()
{
	string1();

	string token = getToken();
	if (token == "+")
		stringexp();
	else
		str = token + " " + str;
}

void Parser::var()
{
	string token = getToken();
	if (token == "any")
	{
		id();
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
			exp();
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

		// Variables
		bool f = false;
		string temp = "";
		while (true)
		{
			string t = get_temp_token(&temp_translation);
			if (t == "")
				break;
			if (is_float(t) || character(t))
			{
				f = true;
				temp = temp + t + " " + temp_translation;
				break;
			}
			temp.append(t + " ");
		}
		if (f)
			translation.append("Float " + temp_id + "= new Float( " + temp + "); ");
		else
			translation.append("Integer " + temp_id + "= new Integer( " + temp + "); ");
	}
	else if (token == "def")
	{
		id();
		token = getToken();
		if (token == "=")
		{
			exp();
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

		// Constants
		bool f = false;
		string temp = "";
		while (true)
		{
			string t = get_temp_token(&temp_translation);
			if (t == "")
				break;
			if (is_float(t) || character(t))
			{
				f = true;
				temp = temp + t + " " + temp_translation;
				break;
			}
			temp.append(t + " ");
		}
		if (f)
			translation.append("final Float " + temp_id + "= new Float( " + temp + "); ");
		else
			translation.append("final Integer " + temp_id + "= new Integer( " + temp + "); ");
	} else {
		cout << "Incorrect syntax for variable declaration." << endl;
		exit(1);
	}
	temp_translation = "";
}

void Parser::id()
{
	string token = getToken();

	if (character(string(1, token[0])))
	{
		for (int i = 1; i < token.length(); i++)
		{
			if (!character(string(1, token[i])) && !is_integer(string(1, token[i])) && token[i] != '_')
			{
				cout << "Invalid identifier: " << token << endl;
				exit(1);
			}
		}
	} else {
		cout << "Invalid identifier: " << token << endl;
		exit(1);
	}
	temp_id = token + " ";
}

void Parser::array()
{
	string token = getToken();

	if (token == "arr")
	{
		id();
		token = getToken();
		if (token == "->")
		{
			dimens();
			token = getToken();
			if (token != ";")
			{
				cout << "Missing ; in array definition." << endl;
				exit(1);
			}
		}
		else if (token == "[")
		{
			token = getToken();
			if (is_integer(token))
			{
				token = getToken();
				if (token == "]")
				{
					token = getToken();
					if (token != ";")
					{
						if (token == "[")
						{
							token = getToken();
							if (is_integer(token))
							{
								token = getToken();
								if (token == "]")
								{
									token = getToken();
									if (token != ";")
									{
										cout << "Missing ; in array declaration." << endl;
										exit(1);
									}
								} else {
									cout << "Missing ] in array declaration." << endl;
									exit(1);
								}
							} else {
								cout << "Expected integer in array size declaration." << endl;
								exit(1);
							}
						} else {
							cout << "Missing [ or ; in array declaration." << endl;
							exit(1);
						}
					}
				} else {
					cout << "Missing ] in array declaration." << endl;
					exit(1);
				}
			} else {
				cout << "Expected integer in array size declaration." << endl;
				exit(1);
			}
		} else {
			cout << "Missing [ in array declaration." << endl;
			exit(1);
		}
	}
}

void Parser::dimens()
{
	string token = getToken();

	if (is_integer(token) || is_float(token) || is_boolean(token))
	{
		str = token + " " + str;

		multiarr();

		token = getToken();
		if (token == "|")
			dimens();
		else
			str = token + " " + str;
	} else
		str = token + " " + str;

}

void Parser::multiarr()
{
	exp();

	string token = getToken();
	if (token == ",")
		multiarr();
	else
		str = token + " " + str;
}

void Parser::stringword()
{
// THIS NEEDS TO BE FIXED
// THIS GETS things after our string, such as ; (a valid symbol for a string)!
/*
	string token = getToken();
	if (character(token) || is_symbol(token))
		stringword();
	else
		str = token + " " + str;
*/
}

void Parser::string1()
{
	string token = getToken();
	if (token[0] == '\"')
	{
		stringword();
		if (token[token.length()-1] != '\"')
		{
			cout << "Missing closing \" in string declaration." << endl;
			exit(1);
		}
	} else {
		cout << "Missing opening \" in string declaration." << endl;
		exit(1);
	}
}

bool Parser::is_boolean(string s)
{
	if (s != "true" || s != "false")
		return false;
	return true;
}

void Parser::function()
{
	string token = getToken();

	if (token == "func")
	{
		id();
		token = getToken();
		if (token == "(")
		{
			parameters();
			token = getToken();
			if (token == ")")
			{
				token = getToken();
				if (token == "->")
				{
					token = getToken(); // Allow for multiple statements
					string temp = getToken();
					while (token == "if" || token == "for" || token == "foreach" || token == "while" ||
							token == "exec" || token == "return" || token == "any" || token == "def" ||
							token == "arr" || temp == "=" || temp == "(" || temp == ".")
					{
						str = temp + " " + str;
						str = token + " " + str;
						if (temp_return.length() > 0)
						{
							string val = get_temp_token(&temp_return);
							val = get_temp_token(&temp_return);
							if (is_integer(val) && val.length() > 0)
							{
								translation.append("public Integer " + temp_id + "( " + temp_parameters + " ) { ");
								stmlist();
								translation.append("} ");
							}
							else if (is_float(val) && val.length() > 0)
							{
								translation.append("public Float " + temp_id + "( " + temp_parameters + " ) { ");
								stmlist();
								translation.append("} ");
							}
							else if (is_boolean(val) && val.length() > 0)
							{
								translation.append("public Boolean " + temp_id + "( " + temp_parameters + " ) { ");
								stmlist();
								translation.append("} ");
							} else {
								translation.append("public void " + temp_id + "( " + temp_parameters + " ) { ");
								stmlist();
								translation.append("} ");
							}
						} else {
							translation.append("public void " + temp_id + "( " + temp_parameters + " ) { ");
							stmlist();
							translation.append("} ");
						}
						temp_id = "";
						temp_return = "";
						temp_parameters = "";
						temp_statements = "";
						token = getToken();
						temp = getToken();
					}
					str = temp + " " + str;
					str = token + " " + str;
					token = getToken();
					if (token != ";")
					{
						cout << "TOKEN: " << token << endl;
						cout << "Missing ; in function definition" << endl;
						exit(1);
					}
				} else {
					cout << "Missing -> in function definition" << endl;
					exit(1);
				}
			} else {
				cout << "Missing ) in function definition" << endl;
				exit(1);
			}
		} else {
			cout << "Missing ( in function definition" << endl;
			exit(1);
		}
	}
}

void Parser::parameters()
{
	string token = getToken();
	if (token == "any")
	{
		string temp = getToken();
		translation.append("Object " + temp);
		str = temp + " " + str;
		str = token + " " + str;
		var();
		token = getToken();
		if (token == ",")
		{
			translation.append(",");
			parameters();
		} else
			str = token + " " + str;
	} else
		str = token + " " + str;
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

	for (int i = 0; i < s.length(); i++)
		if (string::npos == valid.find(s[i]))
			return false;
	return true;
}

bool Parser::is_symbol(string s)
{
	string valid = " ,<.>/?;:\'\"\\|]}[{=+-_)(*&^%$#@!~`";

	if (s.length() == 0)
		return false;

	for (int i = 0; i < s.length(); i++)
		if (string::npos != valid.find(s[i]))
			return true;
	return false;
}

bool Parser::is_string(string s)
{
	if (s[0] != '\"' && s[s.length()-1] != '\"')
		return false;
	return true;
}

bool Parser::is_integer(string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (isdigit(s[i]) && string(1, s[i]).length() > 0)
			continue;
		return false;
	}
	return true;
}

bool Parser::is_float(string s)
{
	int period_count = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '.')
		{
			if (period_count < 1)
			{
				period_count++;
				continue;
			} else {
				return false;
			}
		}

		if (!isdigit(s[i]))
			return false;
	}

	if (period_count != 1)
		return false;
	return true;
}

void Parser::print()
{
	cout << str << endl;
}
