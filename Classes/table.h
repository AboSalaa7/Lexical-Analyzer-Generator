#ifndef PARSER_TABLE_H
#define PARSER_TABLE_H
#include <bits/stdc++.h>
using namespace std;



class table
{
public:

    void get_first();
    void get_follow();
    vector<vector<vector<string>>> productions;
    vector <string> nons;
    map<string, vector<string>> firsts;
    map<string, vector<string>> follows;
    typedef multimap<string, string>::iterator MMAPIterator;






private:
    vector<string> find_productions(string start);
    int find_ix(string prod);
    bool check_eps(string prod);
    bool check_all_eps(vector<string> prod);
    void update_productions(string x);
    void call(int q);

};









#endif  //PARSER_TABLE_H

