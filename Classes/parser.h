

#ifndef LEXICAL_AANALYZER_GENERATOR_PARSER_H
#define LEXICAL_AANALYZER_GENERATOR_PARSER_H
#include "NFA.h"
#include "DFA.h"

class parser
{
public:
    parser(string File, string opFile);
    NFA* getFullNFA();
    vector<string> get_expressions();
    unordered_map<string,NFA> get_allnfa();
    vector<string> getOpFile();
    void readOpFile();
    DFA dfa;

    virtual ~parser();


protected:

private:
    string RegexFile;
    string opFile;
    vector<string> Regex;
    int statenum = 0;
    unordered_map<string,NFA> definitions;
    unordered_map<string,NFA> expressions;
    vector<string> expNames;
    vector<string> weights;

    vector<string> opFileRegex;

    void getNFA(string line);
    void readRegexFile(string path);
    void addOperation(char c, stack<string>* in, stack<string>* post);
    bool isOperation(char c);
    bool isOperation(string c);
    string removeSpaces(string str);
    vector<string> getTokens(string str2);
    stack<string> getPostfix(vector<string> tokens);
    bool isBinaryOp(string c);

    string parseToken(string token);

};



#endif //LEXICAL_AANALYZER_GENERATOR_PARSER_H
