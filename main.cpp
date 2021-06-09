#include <iostream>
#include <bits/stdc++.h>
#include "NFA.h"
#include "parser.h"
#include "table.h"
#include "CFGParser.h"

int main() {
    parser* gr = new parser("regex.txt","input.txt");

    gr->getFullNFA();

    cout << "Expression" << endl;
    NFA final,final2;
    final = final2.create_NFA("");
   /* state temp;
    temp.num = final.get_end().num;
    temp.visited = final.get_end().visited;
    temp.children = final.get_end().children;
    temp.accepted_token = final.get_end().accepted_token;
    temp.is_accepted = false;*/

    vector<string> expNames = gr->get_expressions();
    cout << expNames.size() << endl;
    unordered_map<string,NFA> allExpressions = gr->get_allnfa();
    cout<<"heyyy"<<endl;
    for(int i=0; i< expNames.size(); i++) {
        cout << i<<endl;
        NFA g = allExpressions[expNames[i]];
        final = final.Union(final,g);
        final.print_NFA();

    }
    //NFA final = combineallExpressions, expNames);
    final.print_NFA();
    DFA dfa;
    dfa.nfa = final;
    dfa.create_DFA();
    cout<<"dfa"<<endl;
    dfa.print_DFA();
    cout<<"end_dfa"<<endl;
    dfa.minimization();
    cout<<"minimized"<<endl;
    dfa.print_DFA();
    gr->dfa =dfa;
    gr->readOpFile();

   /* CFGParser* gr = new CFGParser("grammar.txt");
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
    }*/

    return 0;

}
/*
 #include "state.h"
using namespace std;
class graph
{
public:
    graph();
    void set_start_state(state start);
    state get_start_state();
    void set_end_state(state end);
    state get_end_state();
    void set_graph_size(int s);
    int get_graph_size();
    void clear_visited();
    virtual ~graph();
private:
    state start;
    state end;
    int graph_size =0 ;
};
 */
