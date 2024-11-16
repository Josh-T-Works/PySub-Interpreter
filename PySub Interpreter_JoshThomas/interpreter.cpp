#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include "interpreter.h"
#include "lexAnalyzer.h"
#include "expEvaluator.h"
#include "interface.h"

using namespace std;
bool st = false; //Bool switches
bool test = false;
bool read = false;
bool print = false;
bool charin = false;
int parenL = 0;
string id = "";
int var = 0;
bool v = true;
bool IF = false; //switches to check which conditionals are used
bool ELIF = false;
bool IFR = false;
bool ELIFR = false;
bool WHILE = false;
bool WHILEEQU = false;
bool whileEval = false;

bool syntax = true;
Interpreter::tokenLineType whileCondition; // containers to store line data for while loops
Interpreter::tokenLineType statement = {};
vector<Interpreter::tokenLineType> statements = {};
bool g = false; // Bools to check which conditionals should be used
bool h = false; 
bool j = false;
bool s = true;
int k = 0;
int l = 0;


int res = 0;
bool integer = false;
stack<string> vars; //Stacks for variable and keyword tracking
stack<string> kWords;
expEvaluator expEval;
Interface intF;
bool e = false; //Tracks state of the while loop
bool f = false;

void Interpreter::run(tokenType tc, expEvaluator& symT) //expEvaluator expEvaluation
{
	k = tc.size();
	l = 0;
	whileCondition.clear();
	statements.clear();
	e = false;
	f = false;
	g = false;
	h = false;
	j = false;
	charin = false;
	for (auto iter : tc) {
		interpretLine(iter, symT);
		l++;
	}
	e = false;
	f = false;
	cin.sync();
	cin.ignore();
}

string Interpreter::interpretLine(tokenLineType tl, expEvaluator& symT)
{
	int count = 0; // Tracks position in iter
	st = false;
	s = false;
	read = false;
	syntax = true;
	parenL = 0;
	int res = 0;
	var = 0;
	bool d = true;
	string prev = "";
	string equ = "";
	string input = "";
	string cond = "";
	string condition = "";
	
	for (int index = 0; index < tl.size(); index++) //Checks for appropriate parentheses
	{
		if (tl[index].first == "(")
		{
			parenL++;
		}
		if (tl[index].first == ")")
		{
			if (parenL != 0)
			{
				parenL--;
			}
			else syntax = false;
		}
		if (tl[index].first == "while") //stores while loop condition
		{
			index++;
			while (tl[index].first != ":")
			{
				whileCondition.push_back(tl[index]);
					index++;
			}
			d = false;
			e = true;
		}
	}
	if (parenL != 0)
	{
		syntax = false;
	}
	if (syntax == false) //Returns error for invalid parentheses
	{
		cout << "Error: Invalid Syntax" << endl;
		intF.command();
	}
	else if (d == true) { // not the 'while' line
		for (int index = 0; index < tl.size(); index++)
		{
			if (v == false && tl[index].second == categoryType::INDENT) //if condition is false in a conditional
			{
				break;
			}

			if (tl[index].second == categoryType::INDENT && e == true) // takes in statements connected to while loop
			{
				if (whileCondition.size() != 0)
				{
					statement.clear();
					while (index < tl.size())
					{
						statement.push_back(tl[index]);
						index++;
					}
					statements.push_back(statement);
					f = true;
				}
			}
			if (e == true && l == k-1) //Evaluates all stored statements for while loop at the appropriate moment
			{
					f = false;
					e = false;
					if (whileCondition.size() != 0)
					{
						while (interpretLine(whileCondition, symT) == "1")
						{
							for (int i = 0; i < statements.size(); i++)
							{
								interpretLine(statements[i], symT);
							}
						}
					}
			}
			if (e == true && tl[0].second != categoryType::INDENT) // Same as above, but if there is something after while statement
			{
				if (tl[0].first != "while")
				{
					f = false;
					e = false;
					if (whileCondition.size() != 0)
					{
						while (interpretLine(whileCondition, symT) == "1")
						{
							for (int i = 0; i < statements.size(); i++)
							{
								interpretLine(statements[i], symT);
							}
						}
					}
				}
			}
			if (index < tl.size())
			{
				auto i = tl[index];
				if (i.second == categoryType::COMMENT) { //Ignore comments
					break;
				}
				if (i.second == categoryType::KEYWORD) { //Switches on appropriate bools based on keyword
					kWords.push(i.first); //Stores keyword
					if (i.first == "print")
					{
						print = true;
					}
					if (i.first == "if") //switching on bools according to conditionals encountered
					{
						read = true;
						IF = true;
						g = true;
						h = false;
						j = false;
					}
					if (i.first == "elif") // ^
					{
						if (IF == true && IFR == false)
						{
							read = true;
							ELIF = true;
							g = false;
							h = true;
							j = false;
						}
						else v = false;
					}
					if (i.first == "else") // ^
					{
						if (IF == true && IFR == false && ELIFR == false)
						{
							read = true;
							g = false;
							h = false;
							j = true;
						}
						else v = false;
					}
					/*if (i.first == "while" && whileCondition.size() == 0) 
					{
						read = true;
						WHILE = true;
					}*/
				}
				if (i.second == categoryType::STRING_LITERAL) {
					if (kWords.top() == "print" || kWords.top() == "input") { //Prints whatever is inside quotes
						for (int x = 0; x < i.first.size(); x++)
						{
							if (x != 0 && x != i.first.size() - 1) {
								cout << i.first[x];
								st = true;
							}
						}
						if (kWords.top() == "input") //Requests imput after string is done printing
						{
							cout << " ";
							cin >> input;
							charin = true;
							equ += input; // adds input to string that goes through expEvaluator
							st = false;
							
							symT.tableS(vars.top(), input); // Assigns stored variable with input for table
						}
					}
					else equ += i.first;
				}
				if (i.second == categoryType::LEFT_PAREN) {
					parenL++;
				}
				if (i.second == categoryType::COMMA) { //Appropriate spacing when comma is read
					if (equ != "")
					{
						cout << symT.itop(equ);
						equ = "";
					}
					cout << " ";
				}
				if (i.second == categoryType::IDENTIFIER) { //Sees if variable is assigned to value
					if (index + 1 < tl.size())
					{
						if (symT.retV(i.first) != "ERROR" && tl[index + 1].first != "=")
						{
							equ += symT.retV(i.first); //Puts value into string for expE
							read = true;
						}
						else vars.push(i.first);
					}
					else equ += symT.retV(i.first);
				}
				if (i.second == categoryType::ARITH_OP) //Puts value into string for expE v v
				{
					equ += i.first;
				}
				if (i.second == categoryType::RELATIONAL_OP)
				{
					equ += i.first;
				}
				if (i.second == categoryType::NUMERIC_LITERAL)
				{
					equ += i.first;
					read = true;
				}
				if (read == true)
				{
					if (count == tl.size() - 1) // Once at the end of the line
					{
						if (!vars.empty())
						{
							
							id = to_string(symT.itop(equ)); //Puts in final value and variable for table
							symT.tableS(vars.top(), id);
							vars.pop();
							
						}
						if (g == true) // More switches to note which conditionals are valid to use
						{
							if (equ.size() != 0)
							{
								if (to_string(symT.itop(equ)) == "0")
								{
									v = false;
									IFR = false;
								}
								else
								{
									v = true;
									IFR = true;
								}
							}
							else
							{
								v = true;
								IFR = true;
							}
						}
						if (h == true) // ^
						{
							if (equ.size() != 0)
							{
								if (to_string(symT.itop(equ)) == "0")
								{
									v = false;
									ELIFR = false;
								}
								else
								{
									v = true;
									ELIFR = true;
								}
							}
							else
							{
								v = true;
								ELIFR = true;
							}
						}
						if (WHILE == true) // ^
						{
							v = true;
						}
						if (print == true) // Same, but prints the result if the switch is on
						{
							id = "";
							id = to_string(symT.itop(equ));
							cout << id;
							st = true;
						}
						read = false;
					}
				}
				if (i.second == categoryType::RIGHT_PAREN) { //Pops off keywords when they are no longer in use
					/*if (parenL == 0)
					{
						symT.error();
						break;
					}*/
					parenL--;
					if (!kWords.empty())
					{
						if (kWords.top() == "int")
						{
							kWords.pop();
							integer = false;
						}
						else if (kWords.top() == "print")
						{
							kWords.pop();
							print = false;
						}
						else if (kWords.top() == "input")
						{
							kWords.pop();
						}
					}
				}
				if (i.second == categoryType::ASSIGNMENT_OP)
				{
					if (i.first != "=")
					{
						equ += i.first;
					}
					else read = true;
				}
				count++;
				if (i.second == categoryType::UNKNOWN)
				{
					symT.error();
				}
			}
		}

		if (st == true) //If something is printed, make a new line (neatness)
		{
			cout << endl;
		}
		if (equ.size() != 0)
		{
			return to_string(expEval.itop(equ));
		}
		if (parenL != 0)
		{
			symT.error();
		}
	}
}