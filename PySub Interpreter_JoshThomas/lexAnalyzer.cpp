#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "lexAnalyzer.h"
#include "interface.h"

using namespace std; 

set<char> arithC = { '+','-','*','/','%' }; // Sets of all symbol-based operators
set<string> arith = { "**","//" };
set<char> assignC = { '=' };
set<string> assign = { "+=","-=","*=","/=","%=","//=","**=","&=","|=","^=",">>=","<<=" };
set<char> compC = { '>','<' };
set<string> comp = { "==","!=",">=","<="};

void LexicalAnalyzer::readTokens(programType programCode)
{
	tokenType codePairs;
	for (string line : programCode) // Reads all lines from program code
	{
		tokenLineType linePair;
			
		for (int i = 0; i < line.size(); i++) // Reads all characters from each line
		{
			pairType tokenPair;
			int next_index = i + 1;
			string lit = "";
			char litC = '\0';
			
			if (isdigit(line[i])) // Checks if digit
			{
					lit += line[i];
					while (isdigit(line[next_index])) // Adds digits to lit if applicable
					{
						lit += line[next_index];
						next_index++;
					}
					i = next_index -= 1;
					tokenPair = make_pair(lit, categoryType::NUMERIC_LITERAL); // Makes token pair
					linePair.push_back(tokenPair); // Stores it in line vector
			}

			else if (isalpha(line[i]) || line[i] == '_') // Checks if character can form LOGICAL_OP, KEYWORD, or IDENTIFIER
			{
				lit += line[i];
				while (isalpha(line[next_index]) || line[next_index] == '_')
				{
					lit += line[next_index];
					next_index++;
				}
				i = next_index;
				i--;
				if (lit == "and" || lit == "or" || lit == "not")
				{
					tokenPair = make_pair(lit, categoryType::LOGICAL_OP);
					linePair.push_back(tokenPair);
				}
				else if (lit == "as" || lit == "assert" || lit == "break" || lit == "class" || lit == "continue" || lit == "def" || lit == "del" ||
						 lit == "elif" || lit == "else" || lit == "except" || lit == "False" || lit == "finally" || lit == "for" || lit == "from" ||
						 lit == "global" || lit == "if" || lit == "import" || lit == "in" || lit == "input" || lit == "int" || lit == "is" || lit == "lambda" ||
						 lit == "None" || lit == "nonlocal" || lit == "pass" || lit == "print" || lit == "raise" || lit == "return" || lit == "True" || 
						 lit == "try" || lit == "while" || lit == "with" || lit == "yield") // Python Keywords
				{
					tokenPair = make_pair(lit, categoryType::KEYWORD);
					linePair.push_back(tokenPair);
				}
				else 
				{
					tokenPair = make_pair(lit, categoryType::IDENTIFIER);
					linePair.push_back(tokenPair);
				}
			}
			
			else if (line[i] == '\'')
			{
				lit += '\''; // Puts string in quotes
				while (line[next_index] != '\'') // Reads string
				{
					lit += line[next_index];
					next_index++;
					i = next_index;
				}
				lit += '\'';
				tokenPair = make_pair(lit, categoryType::STRING_LITERAL);
				linePair.push_back(tokenPair);
			}

			else if (line[i] == '\"')
			{
				lit += '\"';
				while (line[next_index] != '\"')
				{
					lit += line[next_index];
					next_index++;
					i = next_index;
				}
				lit += '\"';
				tokenPair = make_pair(lit, categoryType::STRING_LITERAL);
				linePair.push_back(tokenPair);
			}

			else if (line[i] == '(') // Individual Category Type Checkers (4 Below)
			{
				tokenPair = make_pair(line[i], categoryType::LEFT_PAREN);
				linePair.push_back(tokenPair);
			}

			else if (line[i] == ')')
			{
				tokenPair = make_pair(line[i], categoryType::RIGHT_PAREN);
				linePair.push_back(tokenPair);
			}

			else if (line[i] == ':')
			{
				tokenPair = make_pair(line[i], categoryType::COLON);
				linePair.push_back(tokenPair);
			}

			else if (line[i] == ',')
			{
				tokenPair = make_pair(line[i], categoryType::COMMA);
				linePair.push_back(tokenPair);
			}

			else if (line[i] == '#')
			{
				while (i < line.size()) // Reads in comment
				{
					lit += line[i];
					i++;
				}
				tokenPair = make_pair(lit, categoryType::COMMENT);
				linePair.push_back(tokenPair);
			}

			else if (isspace(line[i]) && (i == 0)) // Checks for indents at the start of each line
			{
			lit += '\''; // Visualize Indents
			while (isspace(line[i]))
			{
				lit += ' ';
				i++;
			}
				lit += '\'';
				tokenPair = make_pair(lit, categoryType::INDENT);
				linePair.push_back(tokenPair);
				i--;
			}
			
			else if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%' || line[i] == '=' ||
			line[i] == '&' || line[i] == '|' || line[i] == '^' || line[i] == '>' || line[i] == '<' || line[i] == '!') // Forms Together Operator
		    {
				 next_index = i + 1;
				 lit += line[i]; // Stores multiple character operators
				 litC += line[i]; // Stores single character operators
				 bool check = true;

				 while (line[next_index] == '+' || line[next_index] == '-' || line[next_index] == '*' || line[next_index] == '/' ||
					 line[next_index] == '%' || line[next_index] == '=' || line[next_index] == '&' || line[next_index] == '|' ||
					 line[next_index] == '^' || line[next_index] == '>' || line[next_index] == '<' || line[next_index] == '!') // Keeps Forming Operator
				 {
						 lit += line[next_index];
						 next_index++;
				 }
				 i = next_index - 1;
					if (check == true)
					for (auto it : arith) // Checks each set for matching operator
					 {
						 if (lit == it)
						 {
							 tokenPair = make_pair(lit, categoryType::ARITH_OP);
							 linePair.push_back(tokenPair);
							 check = false;
							 break;
						 }
					 }
					if (check == true) // Stops once one is found
					 for (auto it : arithC)
					 {
						 if (litC == it && lit.size() == 1) // Only checks single characters if operator is a single character
						 {
							 tokenPair = make_pair(litC, categoryType::ARITH_OP);
							 linePair.push_back(tokenPair);
							 check = false;
							 break;
						 }
					 }
					if (check == true)
					 for (auto it : assign)
					 {
						 if (lit == it)
						 {
							 tokenPair = make_pair(lit, categoryType::ASSIGNMENT_OP);
							 linePair.push_back(tokenPair);
							 check = false;
							 break;
						 }
					 }
					if (check == true)
					 for (auto it : assignC)
					 {
						 if (litC == it && lit.size() == 1)
						 {
							 tokenPair = make_pair(litC, categoryType::ASSIGNMENT_OP);
							 linePair.push_back(tokenPair);
							 check = false;
							 break;
						 }
					 }
					if (check == true)
					 for (auto it : comp)
					 {
						 if (lit == it)
						 {
							 tokenPair = make_pair(lit, categoryType::RELATIONAL_OP);
							 linePair.push_back(tokenPair);
							 check = false;
							 break;
						 }
					 }
					if (check == true)
					 for (auto it : compC)
					 {
						 if (litC == it && lit.size() == 1)
						 {
							 tokenPair = make_pair(litC, categoryType::RELATIONAL_OP);
							 linePair.push_back(tokenPair);
							 check = false;
							 break;
						 }
					 }
				 }
			else if (!isspace(line[i])) // Everything else gets set to unknown
			{
			tokenPair = make_pair(line[i], categoryType::UNKNOWN);
			linePair.push_back(tokenPair);
			}
		}
		codePairs.push_back(linePair); // Line is put into entire code object
	}
	tokenInfo = codePairs; // Transfers code data
}

LexicalAnalyzer::tokenType LexicalAnalyzer::getT()
{
	return tokenInfo;
}

void LexicalAnalyzer::showT()
{
	map<categoryType, std::string> mapS; // Map to utilize category types as strings
	mapS.insert(std::pair<categoryType, std::string>(categoryType::KEYWORD, "KEYWORD"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::IDENTIFIER, "IDENTIFIER"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::STRING_LITERAL, "STRING_LITERAL"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::NUMERIC_LITERAL, "NUMERIC_LITERAL"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::ASSIGNMENT_OP, "ASSIGNMENT_OP"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::ARITH_OP, "ARITH_OP"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::LOGICAL_OP, "LOGICAL_OP"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::RELATIONAL_OP, "RELATIONAL_OP"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::LEFT_PAREN, "LEFT_PAREN"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::RIGHT_PAREN, "RIGHT_PAREN"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::COLON, "COLON"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::COMMA, "COMMA"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::COMMENT, "COMMENT"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::INDENT, "INDENT"));
	mapS.insert(std::pair<categoryType, std::string>(categoryType::UNKNOWN, "UNKNOWN"));

	int i = 0;
	int j = 0;
	cout << endl << "*** TOKEN INFORMATION ***" << endl << endl;
	for (tokenLineType tokenL : tokenInfo) // Displays all tokens and lines nunbered
	{
		cout << "Line #" << i << ":" << endl;
		i++;
		j = 0;
		for (pairType pairT : tokenL)
		{
			cout << "Token[" << j << "]:   " << pairT.first << " - " << mapS[pairT.second] << endl;
			j++;
		}
		cout << endl;
	}
}

void LexicalAnalyzer::clearT() // Clears stored token data
{
	tokenInfo.clear();
}