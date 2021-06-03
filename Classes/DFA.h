
#ifndef LEXICAL_ANALYZER_GENERATOR_DFA_H
#define LEXICAL_ANALYZER_GENERATOR_DFA_H


#include <vector>
#include <string>
#include "NFA.h"

struct D_Edge;
struct D_State {
    int num;
    vector<D_Edge> children;
    vector<state> sub_states;
    string accepted_token;
    bool is_accepted;
    bool is_visited;
};
struct D_Edge {

    string weight;
    D_State from;
    D_State to;

};


class DFA {
public:
    NFA nfa;
    vector<D_State> states;
    vector<D_Edge> dead_states;
    vector<vector<D_State>> Partition;



    void create_DFA();
    int check_unvisited();
    D_State check_if_exist(D_State e_state,D_Edge edge);
    D_State e_closure(  D_State state,D_Edge edge);
    D_State move( D_State T, string symbol );
    void print_DFA();
    void set_start(D_State s);
    D_State get_start();
    void add_state(D_State s);
    void minimization();
    void print_current(vector<D_State> current);
    void print_mini();

private:
    D_State start;
    D_State end;
    int size;

};





#endif //LEXICAL_ANALYZER_GENERATOR_NFA_H
