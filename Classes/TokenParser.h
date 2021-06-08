
#ifndef SYNTAX_ANALYZER_TOKENPARSER_H
#define SYNTAX_ANALYZER_TOKENPARSER_H
#include <bits/stdc++.h>
#include <iostream>
#include <regex>
#include <stack>
#include <fstream>
#include <sstream>
#include "table.h"
using namespace std;


class TokenParser {
public:
    vector<string> tokens;
    vector<string> nonTerminals;
    vector<string> terminals;
    stack<string> currentState;
    string outputFile;
    vector<vector<string>> parsingTable;

    TokenParser(std::string outputFile);
    void fillTokens(vector<string> &lines, string inputFile);
    string getNextToken(int index);
    string MatchToken();
    void Parse();
    void fillVectors(vector<string> non,vector<string> Terminals,vector<vector<string>> t);
    void replaceNonTerminals(stack<string> &states,string tableEntry);
};


#endif //SYNTAX_ANALYZER_TOKENPARSER_H
