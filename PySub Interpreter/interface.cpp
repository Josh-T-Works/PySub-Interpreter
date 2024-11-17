#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "interface.h"
#include "lexAnalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"
//#include "Header.h"

using namespace std;
string line;
char chline;
bool table = false;
string cmd = "";
string arg = "";
expEvaluator expEvaluation;
vector<string> lines;
vector<string> conditions;
bool a = true;

void Interface::startInterface()
{
	cout << "Welcome to the Interface! Type a command to get started, or type 'help' for information." << endl;
	command();
}

void Interface::command() // Main hub for command entry, provides the ">>>"
{
	string full = "";
	string left = "";
	string right = "";
	string line;
	bool addToCmd = true;
	table = false;
	cmd.clear();
	arg.clear();
	cout << ">>> ";
	getline(cin, line); // Gets command

	for (auto ch : line)
	{
		full += ch;
	}

	for (auto ch : line) // Finds command and argument if any
	{
		if (ch == ')')
		{
			break;
		}
		else if (ch == '(')
		{
			addToCmd = false;
		}
		else if (addToCmd)
		{
			cmd += ch;
		}
		else
		{
			arg += ch;
		}
	}
	//cout << cmd << endl;
	//cout << arg << endl;
	if (cmd == "quit")  // Checks if command is valid vv
	{
		quit();
	}
	if (cmd == "help")
	{
		help(arg);
	}
	if (cmd == "read")
	{
		read(arg);
	}
	if (cmd == "show")
	{
		show(arg);
	}
	if (cmd == "clear")
	{
		clear();
	}
	if (cmd == "run")
	{
		//expEvaluation.tClear(); //Clears unwanted variables that could confuse the program
		pysubi.run(lexAnalysis.getT(), expEvaluation); //New line to call function which generates script output
		command();
	}
	if (cmd == "print")
	{
		vector<string> v1 = { full };
		lexAnalysis.readTokens(v1); 
		pysubi.interpretLine(lexAnalysis.getT()[0], expEvaluation);
		cout << endl;
		command();
	}
	if (cmd == "retV") //Function for displaying stored table data
	{
	    cout << expEvaluation.retV(arg) << endl;
		command();
	}
	if (full == "+" || full == "-" || full == "*" || full == "/" || full == "%" || full == ">" || full == ">=" ||
		full == "<" || full == "<=" || full == "!=" || full == "==" || full == "and" || full == "or" || full == "not")
	{
		cout << "Precedence: " << expEvaluation.prec(full) << endl; //Return precedence of one operator
		command();
	}
	for (int x = 0; x < full.size(); x++) // Equation for determining variable and value
	{
		if (full.size() <= 1)
		{
			error();
		}
		if (full[x] == '=' && full[x - 1] != '=' && full[x - 1] != '!' && full[x + 1] != '=' && full[x-1] != '<' && full[x-1] != '>')
		{
			for (int y = 0; y < x; y++) //Reads in string for variable until '=' is encountered
			{
				if (!isspace(full[y]))
				{
					left += full[y];
				}
				if (isalpha(full[y]))
				{
					table = true;
				}
			}
			x++;
			for (x = x; x < full.size(); x++) //Everything else is the value
			{
				if (!isspace(full[x]))
				right += full[x];
			}
			if (table == true)
			{
				vector<string> v1 = { right };
				lexAnalysis.readTokens(v1);
				expEvaluation.tableS(left, pysubi.interpretLine(lexAnalysis.getT()[0], expEvaluation));
			}
			else error();
			command();
		}
	}
	if (isdigit(full[0]) || full[0] == '(' || full[0] == 'n') //Checks if user enters expression
	{
		cout << expEvaluation.itop(full) << endl;
		command();
	}
	//if (!isEmpty(full))
	int x = 0;
	string con = "";
	while (full[x] != ' ')
	{
		con += full[x];
		x++;
	}
	if (con == "while")
	{
		con = "";
		x++;
		while (full[x] != ':')
		{
			con += full[x];
			x++;
		}

		vector<string> v2 = { con };
		lexAnalysis.readTokens(v2);

		cout << "... ";
		getline(cin, line); // Gets content of while loop

		while (line != "")
		{
			lines.push_back(line);
			cout << "... ";
			getline(cin, line);
		}

		conData = lexAnalysis.getT()[0];

		while (pysubi.interpretLine(conData, expEvaluation) == "1")
		{
			for (int i = 0; i < lines.size(); i++)
			{
				vector<string> v2 = { lines[i] };
				lexAnalysis.readTokens(v2);
				pysubi.interpretLine(lexAnalysis.getT()[0], expEvaluation);
			}
		}
		command();
	}
	if (con == "if")
	{
		lexAnalysis.clearT();
		while (line != "")
		{
			lines.push_back(line);
			cout << "... ";
			getline(cin, line);
		}
		
		lexAnalysis.readTokens(lines);
		pysubi.run(lexAnalysis.getT(), expEvaluation);
		command();
	}
	error();
}
void Interface::quit() // Quits the interpreter
{
	cout << "You have quit the interpreter.";
	exit(0);
}

void Interface::error() // Response to invalid commands
{
	cout << "Error: No matching commands found. Try again." << endl;
	command();
}

void Interface::reset() // Leaving help menu notification
{
	cout << "You are now back to the interpreter. Please enter a command." << endl;
	command();
}

void Interface::help(string arg) // Initiates help menu and checks help commands
{
	if (arg == "")
	{
		cout << "Type 'commands' to see a list of all commands. Type 'exit' to exit help interface." << endl; // Help Menu
		while (line != "exit")
		{
			cout << "help> ";
			getline(cin, line);

			if (line == "commands")
			{
				cout << "quit, help, read, show, run, clear, misc" << endl;
			}
			else if (line == "quit")
			{
				cout << "quits the interpreter" << endl;
			}
			else if (line == "help")
			{
				cout << "provides info on commands; type help(<command>) to get specific command help in the main interface" << endl;
			}
			else if (line == "read")
			{
				cout << "clears stored program lines, then reads and stores new program lines" << endl
					<< "Ex: read(test1.py)" << endl;
			}
			else if (line == "show")
			{
				cout << "shows stored program lines" << endl;
			}
			else if (line == "clear")
			{
				cout << "clears shored program lines" << endl;
			}
			else if (line == "run")
			{
				cout << "runs the code of the file that was read" << endl;
			}
			else if (line == "misc")
			{
				cout << "Miscellaneous Commands:" << endl
					<< "Variable assignment: Set a variable equal to an equation, and its value will be saved" << endl
					<< "    Ex: x=5 , z=(4+5)/3, myvar=3+9-4%6/2" << endl << endl
					<< "\"retV\": Returns the value of a variable (must be assigned first)" << endl
					<< "    Ex: retV(x), retV(z), retV(myvar)" << endl << endl
					<< "Operators: Type in an operator and see its precedence in equation calculations" << endl
					<< "    Ex: +, and, %, *, or" << endl;
			}
			else if (line != "exit")
			{
				cout << "Error: Not a recognized command. Please try again." << endl;
			}
		}
		reset();
	}
	else if (arg == "commands") // Gives specific help commands
	{
		cout << "quit, help, read, show, run, clear, misc" << endl;
	}
	else if (arg == "quit")
	{
		cout << "quits the interpreter" << endl;
	}
	else if (arg == "help")
	{
		cout << "provides info on commands; type help(<command>) to get specific command help in the main interface" << endl;
	}
	else if (arg == "read")
	{
		cout << "clears stored program lines, then reads and stores new program lines" << endl
			<< "Ex: read(test1.py)" << endl;
	}
	else if (arg == "show")
	{
		cout << "shows stored program lines" << endl;
	}
	else if (arg == "clear")
	{
		cout << "clears shored program lines" << endl;
	}
	else if (arg == "run")
	{
		cout << "runs the code of the file that was read" << endl;
	}
	else if (arg == "misc")
	{
		cout << "Miscellaneous Commands:" << endl
			<< "Variable assignment: Set a variable equal to an equation, and its value will be saved" << endl
			<< "    Ex: x=5 , z=(4+5)/3, myvar=3+9-4%6/2" << endl << endl
			<< "\"retV\": Returns the value of a variable (must be assigned first)" << endl
			<< "    Ex: retV(x), retV(z), retV(myvar)" << endl << endl
			<< "Operators: Type in an operator and see its precedence in equation calculations" << endl
			<< "    Ex: +, and, %, *, or" << endl;
	}
	else cout << "Error: Not a recognized command. Please try again." << endl;
	command();
}

void Interface::read(string file) // Reads file inputted
{
	ifstream in;

	in.open(file);
	if (!in)
	{
		cout << "Error opening file. Try again" << endl;
		command();
	}

	programCode.clear();
	//expEvaluation.tClear();

	while (true) //Stores file lines
	{
		getline(in, line);
		if (!in) {
			break;
		}
		//cout << line << endl; // Testing
		programCode.push_back(line);
	}
	lexAnalysis.readTokens(programCode); // Token Data Collection
	cout << "File Read Successfully" << endl;
	command();
}

void Interface::show(string arg) // Prints numbered lines of file
{
	if (arg == "tokens")
	{
		lexAnalysis.showT(); // Shows Tokens
	}
	else if (arg == "variables")
	{
		expEvaluation.print();
	}
	else
	{
		int x = 0;
		for (auto y : programCode)
		{
			cout << "[" << x << "] " << y << endl;
			x++;
		}
	}
	command();
}

void Interface::clear() // Clears saved lines
{
	lexAnalysis.clearT(); // Clears Token Data
	programCode.clear();
	expEvaluation.tClear(); // Clears table data
	cout << "File Cleared" << endl;
	cout << "Table Cleared" << endl;
	command();
}

