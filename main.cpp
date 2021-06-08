#include <iostream>
#include <bits/stdc++.h>

#include "Classes/table.h"
#include "Classes/CFGParser.h"
#include "Classes/TokenParser.h";
int main()
{


    CFGParser* gr = new CFGParser("grammar.txt");
    gr->parseFile();


    table t;


    for (int i=0; i<gr->prules.size(); i++)
    {
        t.nons.push_back(gr->prules[i].lhs);
        vector<vector<string>> prods =gr->prules[i].rhs;
        t.productions.push_back(prods);


    }

    t.get_first();
    t.get_follow();


    cout<<"firsts"<<endl;
    for(auto it = t.firsts.cbegin(); it != t.firsts.cend(); ++it)
    {
        cout << it->first << " : |" ;
        for (int i=0; i<it->second.size(); i++)
            cout<<it->second[i]<<"|";
        cout<<endl;
    }

    cout<<"\n\n\nfollows"<<endl;
    for(auto it = t.follows.cbegin(); it != t.follows.cend(); ++it)
    {
        cout << it->first << " : |" ;
        for (int i=0; i<it->second.size(); i++)
            cout<<it->second[i]<<"|";

        cout<<endl;
    }


    cout << "here we create table" << endl;
    t.create_table();
    cout << " Creating Table Done" << endl;
    TokenParser *tp = new TokenParser("output.txt");
    tp->fillTokens(tp->tokens,"input.txt");
    tp->fillVectors(t.nons,t.terminals,t.parse_table);
    tp->Parse();


    return 0;

}

