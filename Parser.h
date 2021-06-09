
#ifndef LEXICAL_AANALYZER_GENERATOR_PARSER_H
#define LEXICAL_AANALYZER_GENERATOR_PARSER_H
#include "DFA.h"

class parser
{
public:
    parser(string File);
    NFA* getFullNFA();
    NFA* clone(NFA* g);
    vector<string> get_weights();
    vector<string> get_expressions();
    void readOpFile();
    void getOpFile();

    virtual ~parser();


protected:

private:
    string RegexFile;
    vector<string> Regex;
    int statenum = 0;
    unordered_map<string,NFA> definitions;
    unordered_map<string,NFA> expressions;
    vector<string> expNames;
    vector<string> weights;
    vector<string> opFileRegex;
    string parseLine(string line);
    DFA dfa;

    void getNFA(string line);
    void readRegexFile(string path);
    void addOperation(char c, stack<string>* in, stack<string>* post);
    bool isOperation(char c);
    bool isOperation(string c);
    string removeSpaces(string str);
    vector<string> getTokens(string str2);
    stack<string> getPostfix(vector<string> tokens);
    bool isBinaryOp(string c);




};



#endif //LEXICAL_AANALYZER_GENERATOR_PARSER_H