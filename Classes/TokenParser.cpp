
#include "TokenParser.h"

void TokenParser::fillTokens(vector<string> &lines, string inputFile) {
    ifstream inFile;
    inFile.open (inputFile);
    string line;
    while(!inFile.eof())
    {
        getline(inFile,line);
        if(line.size()>0)
        {
            lines.push_back(line);
        }
    }
}

string TokenParser::getNextToken(int index) {
    return this->tokens[index];
}

string TokenParser::MatchToken() {
    return std::__cxx11::string();
}

TokenParser::TokenParser(std::string outputFile) {
    this->outputFile = outputFile;
}

void TokenParser::Parse() {
    currentState.push(nonTerminals[0]);
    for(int i = 0 ; i < this->tokens.size() ; i ++) {
        string tok = getNextToken(i);
        string currentTop = this->currentState.top();
        vector<string>::iterator it;
        it = std::find(nonTerminals.begin(), nonTerminals.end(), currentTop);
        if (it != nonTerminals.end()) {
            // Case 3
            // non terminal on top of stack


            int terminalNo = it - nonTerminals.begin();
            cout << this->nonTerminals[terminalNo] << endl;
            vector<string>::iterator itTerminal;
            itTerminal = std::find(terminals.begin(), terminals.end(), tok);
            if(itTerminal != terminals.end()){
                //token found on table


                int tokenNo = itTerminal - terminals.begin();
                if(parsingTable[terminalNo][tokenNo].compare("") == 0){
                    //error
                    // no entry in the table

                } else{
                    // replace nonTerminal with terminals in the stack

                    replaceNonTerminals(currentState,parsingTable[terminalNo][tokenNo]);
                    i--;
                    continue;
                }
            }
        } else if (currentTop.compare(tok) == 0) {
            //Case 1
            //match
            cout << "MATCH: "<< tok<< endl;
            continue;
        } else {
            // error
            cout << "fatal error :D" << endl;
        }
    }
}

void TokenParser::fillVectors(vector<string> non, vector<string> Terminals, vector<vector<string>> t) {
    this->nonTerminals= non;
    this-> terminals = Terminals;
    this-> parsingTable = t;
}

void TokenParser::replaceNonTerminals(stack<string> &states, string tableEntry) {
    stack<string> reversed;
    string del = "'";
    int start = 0;
    int end = tableEntry.find(del);
    while (end != -1) {
        if(tableEntry.substr(start, end - start).size() > 0){
            reversed.push(tableEntry.substr(start, end - start));
        }
        start = end + del.size();
        end = tableEntry.find(del, start);
    }
    while(reversed.size() > 0 ){
        string x = reversed.top();
        states.push(x);
        reversed.pop();
    }
}
