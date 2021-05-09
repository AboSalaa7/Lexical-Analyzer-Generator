#include <iostream>
#include <bits/stdc++.h>
#include "Classes/NFA.h"
#include "Classes/parser.h"
int main() {
    parser* gr = new parser("regex.txt");


    gr->getFullNFA();
   /* vector<string> weights = gr->get_weights();
    for(int i=0; i< weights.size(); i++)
    {
        cout << "\t" <<weights[i] << endl;
    }
    cout << "Expression" << endl;
    vector<string> expNames = gr->get_expressions();
    for(int i=0; i< expNames.size(); i++) {
        cout << "\t" << expNames[i] << endl;
    }
    */
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

