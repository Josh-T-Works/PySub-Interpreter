#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <set>
#include "expevaluator.h"

using namespace std;

int expEvaluator::itop(string full)
{
	int y = 0;
	bool a = true; // Bool switches to test syntax
	bool isOpen = false;
	bool digit = false;
	bool op = false;
	bool alpha = false;
	bool par = false;
	int paren = 0;

	stack<pair<string, categoryTypeO>> stackV;
	vector<vPair> iPairs; // Infix Vecotr
	vector<vPair> pPairs; // Postfix Vector 

	map<categoryTypeO, std::string> mapO; // Map to utilize category types as strings
	mapO.insert(std::pair<categoryTypeO, std::string>(categoryTypeO::LEFT_PAREN, "LEFT_PAREN"));
	mapO.insert(std::pair<categoryTypeO, std::string>(categoryTypeO::RIGHT_PAREN, "RIGHT_PAREN"));
	mapO.insert(std::pair<categoryTypeO, std::string>(categoryTypeO::bOPERATOR, "BINARY_OPERATOR"));
	mapO.insert(std::pair<categoryTypeO, std::string>(categoryTypeO::uOPERATOR, "UNARY_OPERATOR"));
	mapO.insert(std::pair<categoryTypeO, std::string>(categoryTypeO::OPERAND, "OPERAND"));

	for (int x = 0; x < full.size(); x++) // Scans each character of the expression
	{
		vPair vP;
		string lit = "";
		int next_index = x + 1;

		if (full[x] == '(')
		{
			if (par == false) // If it's not directly next to parentheses
			{
				isOpen = true; // Notes open parenthesis
				vP = make_pair(full[x], categoryTypeO::LEFT_PAREN); // Makes token pair
				iPairs.push_back(vP); // Stores it in line vector
				paren++;
				digit = false; // Switch statements to determine what kind of character was analyzed
				op = false;
				alpha = false;
			}
			else a = false;
		}

		if (isdigit(full[x])) // Checks if digit
		{
			if (digit == false && par == false) // Can't be outside and next to closed parentheses and can't be next to another digit
			{
				digit = true;
				lit += full[x];
				while (isdigit(full[next_index])) // Adds digits to lit if applicable
				{
					lit += full[next_index];
					next_index++;
				}
				x = next_index -= 1;

				vP = make_pair(lit, categoryTypeO::OPERAND); // Makes token pair
				iPairs.push_back(vP); // Stores it in line vector
				op = false;
				alpha = false;
				par = false;
			}
			else a = false;
		}

		if (full[x] == '+' || full[x] == '-' || full[x] == '*' || full[x] == '/' || full[x] == '%')
		{
			if (op == false) // If it is not next to another operator
			{
				op = true;
				vP = make_pair(full[x], categoryTypeO::bOPERATOR); // Makes token pair
				iPairs.push_back(vP); // Stores it in line vector
				digit = false;
				alpha = false;
				par = false;
			}
			else a = false;
		}


		if (full[x] == '>' || full[x] == '<' || full[x] == '!' || full[x] == '=') // Forms Together Operator
		{
			if (op == false) // If it is not next to another operator
			{
				op = true;
				lit += full[x];
				if (full[x] == '=' && full[next_index] != '=')
				{
					a = false;
				}
				if (full[next_index] == '=') // Two character variables
				{
					lit += full[next_index];
					next_index++;
				}
				x = next_index - 1;
				vP = make_pair(lit, categoryTypeO::bOPERATOR); // Makes token pair
				iPairs.push_back(vP); // Stores it in line vector
				digit = false;
				alpha = false;
				par = false;
			}
			else a = false;
		}

		if (isalpha(full[x]))
		{
			if (alpha == false || full[x] == 'n') // If not next to another logical operator
			{
				alpha = true;
				digit = false;
				op = false;
				par = false;
				lit += full[x];
				while (isalpha(full[next_index]))
				{
					lit += full[next_index];
					next_index++;
				}
				x = next_index;
				x--;
				if (lit == "and" || lit == "or")
				{
					vP = make_pair(lit, categoryTypeO::bOPERATOR); // Makes token pair
					iPairs.push_back(vP); // Stores it in line vector
				}
				else if (lit == "not")
				{
					vP = make_pair(lit, categoryTypeO::uOPERATOR); // Makes token pair
					iPairs.push_back(vP); // Stores it in line vector
				}
				else
				{
					vP = make_pair(lit, categoryTypeO::IDENTIFIER); // Makes token pair
					iPairs.push_back(vP); // Stores it in line vector
				}
			}
			else a = false;
		}

		if (full[x] == ')')
		{
			if (paren > 0 && op == false && alpha == false) // If open parentheses exist & not closing with operator as last element
			{
				par = true;
				vP = make_pair(full[x], categoryTypeO::RIGHT_PAREN); // Makes token pair
				iPairs.push_back(vP); // Stores it in line vector
				paren--;
				isOpen = false;
				digit = false;
			}
			else a = false;
		}
	}
	if (isOpen || op || alpha) // If parenthesis are left open or if operators are not appropriately used
	{
		a = false;
	}

	if (a == true) // Convert infix to postfix if no syntax errors are found
	{
		for (auto x : iPairs)
		{
			bool b = true;
			//cout << x.first << " : " << mapO[x.second] << endl;
			if (x.second == categoryTypeO::OPERAND)
			{
				pPairs.push_back(x);
			}

			else if (x.second == categoryTypeO::IDENTIFIER)
			{
				bool c = false;
				vPair vP;
				for (auto iter : symbolTable)
				{
					if (iter.first == x.first)
					{
						vP = make_pair(iter.second, categoryTypeO::OPERAND);
						pPairs.push_back(vP);
						bool c = true;
					}
				}
				if (c == false)
				{
					error();
				}
			}

			else if (x.second == categoryTypeO::LEFT_PAREN)
			{
				stackV.push(x);
			}

			else if (x.second == categoryTypeO::RIGHT_PAREN)
			{
				while (stackV.top().second != categoryTypeO::LEFT_PAREN)
				{
					pPairs.push_back(stackV.top());
					stackV.pop();
				}
				stackV.pop();
			}

			else if (x.second == categoryTypeO::bOPERATOR || x.second == categoryTypeO::uOPERATOR)
			{
				while (b == true)
				{
					if (!stackV.empty())
					{
						if (stackV.top().second != categoryTypeO::LEFT_PAREN)
						{
							if (prec(stackV.top().first) >= prec(x.first))
							{
								pPairs.push_back(stackV.top());
								stackV.pop();
							}
							else b = false;
						}
						else b = false;
					}
					else b = false;
				}
				stackV.push(x);
			}
			else error();
		}
		while (!stackV.empty())
		{
			pPairs.push_back(stackV.top());
			stackV.pop();
		}

		/*for (auto x : pPairs)
		{
			cout << x.first << " ";
		}
		cout << endl;*/

		post = pPairs; // Postfix data transferred to object for epos()

		return epos();
	}
	else return error();
}

int expEvaluator::error() // Prompt is given when bas syntax is encountered
{
	cout << "Error: Invalid Syntax" << endl;
	return -1;
}

int expEvaluator::epos() 
{
	int final = 0;
	stack<int> posS;
	for (auto x : post)
	{
		int res = 0;
		int oper1 = 0;
		int oper2 = 0;
		
		// Postfix algorithm as described in pseudocode on Canvas

		if (x.second == categoryTypeO::OPERAND)
		{
			posS.push(stoi(x.first));
		}

		if (x.second == categoryTypeO::bOPERATOR)
		{
			oper2 = posS.top();
			posS.pop();
			oper1 = posS.top();
			posS.pop();

			if (x.first == "+") // Appropriate operator evaluations
				res = (oper1)+(oper2);
			if (x.first == "-")
				res = (oper1)-(oper2);
			if (x.first == "*")
				res = (oper1) * (oper2);
			if (x.first == "/")
				res = (oper1) / (oper2);
			if (x.first == "%")
				res = (oper1) % (oper2);
			if (x.first == ">")
				res = (oper1) > (oper2);
			if (x.first == ">=")
				res = (oper1) >= (oper2);
			if (x.first == "<")
				res = (oper1) < (oper2);
			if (x.first == "<=")
				res = (oper1) <= (oper2);
			if (x.first == "!=")
				res = (oper1) != (oper2);
			if (x.first == "==")
				res = (oper1) == (oper2);
			if (x.first == "and")
				res = (oper1) && (oper2);
			if (x.first == "or")
				res = (oper1) || (oper2);

			posS.push(res);
		}

		if (x.second == categoryTypeO::uOPERATOR)
		{
			oper1 = posS.top();
			posS.pop();

			if (x.first == "not")
				res = !(oper1);

			posS.push(res);
		}
	}
	final = posS.top();
	//cout << final << endl; // Returns and prints final answer
	return final; 
}

int expEvaluator::prec(std::string x) // Function to assess operator precedence 
{
	if (x == "*" || x == "/" || x == "%")
	{
		//cout << "5" << endl;
		return 5;
	}
	else if (x == "+" || x == "-")
	{
		//cout << "4" << endl;
		return 4;
	}
	else if (x == ">" || x == ">=" || x == "<" || x == "<=" || x == "!=" || x == "==")
	{
		//cout << "3" << endl;
		return 3;
	}
	else if (x == "not")
	{
		//cout << "2" << endl;
		return 2;
	}
	else if (x == "and")
	{
		//cout << "1" << endl;
		return 1;
	}
	else if (x == "or")
	{
		//cout << "0" << endl;
		return 0;
	}
}

void expEvaluator::tableS(std::string left, std::string right) // Assigns variables to appropriate values
{
	symbolTable[left] = right;
	//cout << "Variable Assigned" << endl;
	//return left + " = " + right;
}

string expEvaluator::retV(string left) // Retrieves value from inputted key
{
	/*string v = symbolTable[left];
	return v;*/
	if (symbolTable.find(left) != symbolTable.end())
	{
		//cout << symbolTable[left];
		return symbolTable[left];
	}
	else return "ERROR";
}

string expEvaluator::clearV(string left) // Retrieves value from inputted key
{
	/*string v = symbolTable[left];
	return v;*/
	if (symbolTable.find(left) != symbolTable.end())
	{
		symbolTable[left] = "";
		return "CLEARED";
	}
	else return "ERROR";
}

void expEvaluator::print()
{
	cout << "*** STORED VARIABLES ***" << endl;
	for (auto iter : symbolTable)
	{
		cout << iter.first << " = " << iter.second << endl;
	}
}

void expEvaluator::tClear() // Clears symbol table data
{
	symbolTable.clear();
}

string expEvaluator::vCheck(string id, expEvaluator &symT)
{
	if (symT.retV(id) != "ERROR")
	{
		return "GOOD";
	}
	else return "ERROR";
}
