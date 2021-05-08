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
    vector<vector<D_State>> Partition;

    void create_DFA();
    D_State check_if_exist(D_State e_state,D_Edge edge);
    D_State e_closure(  D_State state,D_Edge edge);
    void print_DFA();
    void set_start(D_State s);
    void minimization();
    void print_mini();

private:
    D_State start;
    D_State end;
};


extern int last_num=0;
extern int id=0;


#endif //LEXICAL_ANALYZER_GENERATOR_NFA_H
