#include <iostream>
#include <bits/stdc++.h>

#include "table.h"
#include "CFGParser.h"
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


    t.create_table();



    return 0;

}

