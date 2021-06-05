#include <iostream>
#include <bits/stdc++.h>

#include "table.h"
#include "CFGParser.h"
int main() {
    /*
    parser* gr = new parser("regex.txt","output.txt");


    gr->getFullNFA();

    cout << "Expression" << endl;
    NFA final,final2;
    final = final2.create_NFA("");
    state temp;
    temp.num = final.get_end().num;
    temp.visited = final.get_end().visited;
    temp.children = final.get_end().children;
    temp.accepted_token = final.get_end().accepted_token;
    temp.is_accepted = false;

    vector<string> expNames = gr->get_expressions();
    cout << expNames.size() << endl;
    unordered_map<string,NFA> allExpressions = gr->get_allnfa();
    for(int i=0; i< expNames.size(); i++) {
        cout << i;
        NFA g = allExpressions[expNames[i]];
        final = final.Union(final,g);

    }

    DFA dfa;
    dfa.nfa=final;
    dfa.create_DFA();
    //dfa.print_DFA();
    dfa.minimization();
    //dfa.print_mini();
    cout<<"\n*\n*\n*\n*"<<endl;
    dfa.print_DFA();

    */
    /*
    table t;

    vector<vector<string>> a = {{"T","E'"}};
    vector<vector<string>> b = {
                                {"'+'","T","E'"},
                                {"''"}  };
    vector<vector<string>> c = {{"F","T'"}};
    vector<vector<string>> d = {
                                {"'*'","F","T'"},
                                {"''"}
                                };

    vector<vector<string>> e = {
                                {"'('","E","')'"},
                                {"'id'"},
                                {"''"}
                                };




    t.productions.push_back(a);
    t.productions.push_back(b);
    t.productions.push_back(c);
    t.productions.push_back(d);
    t.productions.push_back(e);

    t.nons ={"E","E'","T","T'","F"};

/*
vector<vector<string>> a = {
                                {"A","'b'","S"},
                                {"'e'"},
                                {"''"} };
    vector<vector<string>> b = {{"'a'"},
                               {"'c'","A","'d'"}  };

    t.productions.push_back(a);
    t.productions.push_back(b);

    t.nons ={"S","A"};
    */
/*
    t.get_first();
    t.get_follow();
    cout<<"-----"<<t.firsts.size()<<endl;
    cout<<"-----"<<t.follows.size()<<endl;


cout<<"firsts"<<endl;
for(auto it = t.firsts.cbegin(); it != t.firsts.cend(); ++it)
{
    cout << it->first << " : |" ;
    for (int i=0;i<it->second.size();i++)
        cout<<it->second[i]<<"|";
    cout<<endl;
}

cout<<"follows"<<endl;
for(auto it = t.follows.cbegin(); it != t.follows.cend(); ++it)
{
    cout << it->first << " : |" ;
    for (int i=0;i<it->second.size();i++)
        cout<<it->second[i]<<"|";
    cout<<endl;
}
*/

 CFGParser* gr = new CFGParser("grammar.txt");
    gr->parseFile();


  table t;


    for (int i=0;i<gr->prules.size();i++){
        t.nons.push_back(gr->prules[i].lhs);
        vector<vector<string>> prods =gr->prules[i].rhs;
        t.productions.push_back(prods);


}

    t.get_first();
    t.get_follow();
    cout<<"-----"<<t.firsts.size()<<endl;
    cout<<"-----"<<t.follows.size()<<endl;


cout<<"firsts"<<endl;
for(auto it = t.firsts.cbegin(); it != t.firsts.cend(); ++it)
{
    cout << it->first << " : |" ;
    for (int i=0;i<it->second.size();i++)
        cout<<it->second[i]<<"|";
    cout<<endl;
}

cout<<"\n\nfollows"<<endl;
for(auto it = t.follows.cbegin(); it != t.follows.cend(); ++it)
{
    cout << it->first << " : |" ;
    for (int i=0;i<it->second.size();i++)
        cout<<it->second[i]<<"|";
    cout<<endl;
}

    return 0;

}

