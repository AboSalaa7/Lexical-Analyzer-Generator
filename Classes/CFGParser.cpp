#include "CFGParser.h"

CFGParser::CFGParser(string file)
{
    this->grammerFile = file;
}


void CFGParser::parseFile() {
    ifstream inFile;
    inFile.open (this->grammerFile);
    string line;
    vector<string> rules;
    while(!inFile.eof())
    {
        while (getline(inFile, line, '#'))
        {
            line = trim(line);
            if(line.size() > 1)
                //cout<<line<<endl;
                rules.push_back(line);

        }
    }
    inFile.close();
    for (int i=0;i<rules.size();i++) {
        string delimiter1 = "=";
        prule pr;
        //lhs
        pr.lhs = rules[i].substr(0, rules[i].find(delimiter1));
        rules[i] = rules[i].substr(rules[i].find(delimiter1) + 1);
        pr.lhs = trim(pr.lhs);
        //rhs
        istringstream ss1(rules[i]);
        string token1;
        while (getline(ss1,token1,'|')) {
            token1 = trim(token1);
            //cout<<token1<<endl;
            istringstream ss2(token1);
            string token2;
            vector<string> rhs;
           // cout<<endl;
            while (getline(ss2,token2,' ')) {
                token2 = trim(token2);
               // cout<<token2<<endl;
                rhs.push_back(token2);
            }
            pr.rhs.push_back(rhs);
            //cout<<endl;
        }
        prules.push_back(pr);
    }
    //cout<<prules.size()<<endl;
   /* for (int i=0;i<prules.size();i++) {
        cout<<"lhs : "<<prules[i].lhs<<endl;
        cout<<"rhs : ";
       //cout<<prules[i].rhs.size()<<endl;
        for (int j=0;j<prules[i].rhs.size();j++) {
            for (int k=0;k<prules[i].rhs[j].size();k++) {
                cout<<prules[i].rhs[j][k]<<"    ";
            }
            cout<<"| ";
        }
        cout<<endl;

    }*/

}



string ltrim(const string &s)
{
    string WHITESPACE = " \n\r\t\f\v";
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}

string rtrim(const string &s)
{
    string WHITESPACE = " \n\r\t\f\v";
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string CFGParser::trim(const string &s) {
    return rtrim(ltrim(s));
}

CFGParser::~CFGParser()
{
}

