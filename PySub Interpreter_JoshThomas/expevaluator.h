#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H

#include <map>
#include "lexAnalyzer.h"
//#include "Header.h"
//#include "interface.h"

enum class categoryTypeO // Similar to LexAnalyzer with modifications
{
	LEFT_PAREN,
	RIGHT_PAREN,
	OPERAND,
	bOPERATOR,
	uOPERATOR,
	IDENTIFIER
};

class expEvaluator
{
private:
	typedef std::map<std::string, std::string>symbolTableType;
	typedef std::pair<std::string, categoryTypeO> vPair; // Pairs to be put into infix vector
	std::vector<vPair> post; // Object to store infix and postfix vectors
	symbolTableType symbolTable; //static
	std::string vars;
public:
	int itop(std::string full); // Puts infix exp into vector, then converts to postfix
	int epos(); // Evaluates postfix vector
	int error(); // Activates whenever incorrect syntax is used
	void tClear(); // Clears Table Data
	int prec(std::string x); // Function to return precedence of operator
	void tableS(std::string left, std::string right); // Function to store variable and value of assignment expressions
	std::string retV(std::string left); // Retrieves appropriate variable value
	std::string clearV(std::string left);
	void print(); // Prints out table variables
	std::string vCheck(std::string id, expEvaluator &symT);
};
//expEvaluator::symbolTableType expEvaluator::symbolTable;
//expEvaluator::symbolTable = std::map<std::string, std::string>();

#endif

