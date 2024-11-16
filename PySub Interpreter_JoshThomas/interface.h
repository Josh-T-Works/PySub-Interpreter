#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "lexAnalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"
//#include "Header.h"

class Interface
{
private:
	typedef std::vector<std::string> programType;
	programType programCode;
	LexicalAnalyzer lexAnalysis; // Initialize Object of the Lex Class
	Interpreter pysubi;
	expEvaluator symbolTable;

	typedef std::pair<std::string, categoryType> pairType;
	typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;
	typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;
	tokenLineType conData;

public:
	void startInterface(); //All used function definitions
	void quit();
	void reset();
	void help(std::string arg);
	void read(std::string file);
	void show(std::string arg);
	void clear();
	void error();
	void command();
	void print(std::string arg);
};

#endif