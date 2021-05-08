
#ifndef LEXICAL_AANALYZER_GENERATOR_NFA_H
#define LEXICAL_AANALYZER_GENERATOR_NFA_H
#include <bits/stdc++.h>
using namespace std;

struct edge;
struct state {
    int num;
    vector<edge> children;
    bool is_accepted;
    string accepted_token;
    bool visited;
};

struct edge {
    string weight;
    state from;
    state to;
};



class NFA
{
public:

    NFA();

    void set_start_state(state start);
    state get_start_state();

    void set_end_state(state end);
    state get_end_state();

    void set_NFA_size(int s);
    int get_NFA_size();
    void clear_visited();

    //lina
    vector<string> weights;
    vector<state> states;
    NFA create_NFA(string token);
    NFA kleene_closure(NFA a);
    NFA concatenate(NFA& a, NFA& b);
    NFA renamestates(NFA a, NFA b);
    NFA Union(NFA& a, NFA& b);
    state get_start();
    void set_start(state a);
    state get_end();
    void set_end(state a);
    void set_NFA(NFA a);
    void print_NFA();
    int Findindex_states(NFA a, state find);
    int count_weights();

    virtual ~NFA();

private:
    state start;
    state end;
    int NFA_size =0 ;
};




#endif //LEXICAL_AANALYZER_GENERATOR_NFA_H
