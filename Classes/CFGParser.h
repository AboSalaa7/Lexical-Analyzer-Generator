#ifndef CFGPARSER_H
#define CFGPARSER_H
#include <bits/stdc++.h>
#include <iostream>
#include <regex>
#include <stack>
#include <fstream>
#include <sstream>
using namespace std;

struct prule {
    string lhs;
    vector<vector<string>> rhs;
};

class CFGParser
{
    public:
        CFGParser(string file);
        void parseFile();
        vector<prule> prules;

        virtual ~CFGParser();

    private:
        string grammerFile;
        string trim(const string &s);
};

#endif // CFGPARSER_H
