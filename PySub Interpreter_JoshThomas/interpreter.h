#ifndef INTERPRETER_H
#define INTERPRETER_H

#include<iostream>
#include<string>
#include<vector>
#include "expevaluator.h"
#include "lexAnalyzer.h"

class Interpreter
{
private:
	 // Definitions for tokens
	typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;
	typedef std::map<std::string, std::string>symbolTableType;
public:
	typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;
	void run(tokenType tc, expEvaluator &symT); //Main function for interpreter.cpp, takes in token info and table by reference
	std::string interpretLine(tokenLineType tl, expEvaluator& symT);
	
};

#endif
