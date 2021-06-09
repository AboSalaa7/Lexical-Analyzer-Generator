
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
/*
string TokenParser::MatchToken() {
    return std::__cxx11::string();
}
*/
TokenParser::TokenParser(std::string outputFile) {
    this->outputFile = outputFile;
}

void TokenParser::Parse() {
    currentState.push(nonTerminals[0]);
    for(int i = 0 ; i < this->tokens.size() ; i ++) {

        if (currentState.size() ==0 )
            currentState.push(nonTerminals[0]);

        string tok = getNextToken(i);
        string currentTop = this->currentState.top();

        //cout << "toS: "<<currentTop << endl;

        vector<string>::iterator it;
        it = std::find(nonTerminals.begin(), nonTerminals.end(), currentTop);

        if (it != nonTerminals.end()) {
            // Case 3
            // non terminal on top of stack

            int terminalNo = it - nonTerminals.begin();
           // cout << ">>>>"<<this->nonTerminals[terminalNo] << endl;
            vector<string>::iterator itTerminal;
            itTerminal = std::find(terminals.begin(), terminals.end(), tok);

            if(itTerminal != terminals.end() ){
                //token found on table

                int tokenNo = itTerminal - terminals.begin();
                if(parsingTable[terminalNo][tokenNo].compare("") == 0){
                    //error
                    // no entry in the table

                } else{
                    // replace nonTerminal with terminals in the stack

                  if (parsingTable[terminalNo][tokenNo]!="''")
                    replaceNonTerminals(currentState,parsingTable[terminalNo][tokenNo]);
                else{
                    currentState.pop();
                    cout<<"HERE :"<<endl;

                }

                    i--;
                    continue;
                }
            }
        } else if (currentTop.compare(tok) == 0) {
            //Case 1
            //match
            cout << "MATCH: "<< tok<< endl;
            currentState.pop();


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
   /* //cout<<"replace tableEntry: "<<tableEntry<<endl;
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
    }*/

    vector <string> splits;
    stringstream check(tableEntry);
    string intermediate;

    while(getline(check, intermediate, ' '))
    {
        splits.push_back(intermediate);
       // cout<<"splits: "<<intermediate<<endl;
    }

   if (splits.size() >= 0 ) states.pop();
   for(int i=splits.size()-1;i>0;i--){
        if (splits[i][0]=='\'')
        splits[i]=splits[i].substr(1,splits[i].size()-2);
    states.push(splits[i]);
    cout<<"PUSH:::::"<<splits[i]<<endl;
   }
/*
    while(reversed.size() > 0 ){
        string x = reversed.top();
        states.push(x);
        cout<<"PUSH:::::::"<<x<<endl;
        reversed.pop();
    }*/

}

