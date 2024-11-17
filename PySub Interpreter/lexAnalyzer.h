#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <map>

enum class categoryType
{
    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN
};

class LexicalAnalyzer
{
private:
    typedef std::pair<std::string, categoryType> pairType;
    typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;
    typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;
    tokenType tokenInfo; // Object to transfer data to different functions
    typedef std::vector<std::string> programType; //Transfer program data from Interface.cpp

public:
    void showT(); // All Lexical Function Declarations
    void clearT();
    void readTokens(programType programCode);
    LexicalAnalyzer::tokenType getT();
};

#endif