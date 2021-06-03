#include <iostream>
#include <bits/stdc++.h>
#include "Classes/NFA.h"
#include "Classes/parser.h"
int main() {
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

