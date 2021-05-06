#ifndef LEXICAL_ANALYZER_GENERATOR_NFA_H
#define LEXICAL_ANALYZER_GENERATOR_NFA_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Edge;
struct State {
    int num;
    vector<Edge> children;
    bool is_accepted;
};
extern int lastnum =0;
struct Edge {
    string weight;
    State from;
    State to;
};


class NFA {
    public:
        vector<State> states;
        NFA create_NFA(string token);
        NFA kleene_closure(NFA a);
        NFA concatenate(NFA& a, NFA& b);
        NFA renameStates(NFA a, NFA b);
        NFA Union(NFA& a, NFA& b);
        State get_start();
        void set_start(State a);
        State get_end();
        void set_end(State a);
        void set_NFA(NFA a);
        void print_NFA();
        int Findindex_states(NFA a, State find);

 
    private:
        State start;
        State end;
};
 

 
#endif //LEXICAL_ANALYZER_GENERATOR_NFA_H