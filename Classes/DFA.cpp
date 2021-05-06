#include "DFA.h"
#include <stack>
int Find_index_states(State u,D_State e_state){
    for (int i=0;i<e_state.sub_states.size();i++) {
       if( e_state.sub_states[i].num== u.num)
           return i;

    }
    return -1;
}
D_State move( D_State T, string symbol ,NFA nfa,int q){
    D_State res;
    for (int i=0; i<T.sub_states.size(); i++){
        State t = T.sub_states[i];
        for (int k=0;k<nfa.states.size();k++){
            if (nfa.states[k].num==t.num){
                t = nfa.states[k];
                break;

            }
        }
        for (int j=0; j<t.children.size(); j++){
             if (t.children[j].weight == symbol){
                State u = t.children[j].to;

               if(Find_index_states(u,res)== -1){
                    res.sub_states.push_back(u);

               }

             }

        }

    }

    return res;

}


D_State e_closuree(  State state,NFA nfa) {

    stack<State> s;
    D_State e_state;
    s.push(state);
    e_state.sub_states.push_back(state);
    while(!s.empty()){
        State t = s.top();
        s.pop();
         for (int k=0;k<nfa.states.size();k++){
            if (nfa.states[k].num==t.num){
                t = nfa.states[k];
                break;
            }
        }
        for (int i = 0; i<t.children.size(); i++){
            State u = t.children[i].to;
            if(Find_index_states(u,e_state)== -1&& t.children[i].weight=="e"){
            e_state.sub_states.push_back(u);
                s.push(u);
            }
        }
    }

    return e_state;
}
bool check_same(D_State a, D_State b){
    bool found=false;
    for (int i=0;i<a.sub_states.size();i++){
        found=false;
        for (int j=0;j<b.sub_states.size();j++){
            if (a.sub_states[i].num==b.sub_states[j].num){
                found=true;
            }
        }
        if (!found )
            return false;
    }
    for (int i=0;i<b.sub_states.size();i++){
        found=false;
        for (int j=0;j<a.sub_states.size();j++){
            if (a.sub_states[i].num==b.sub_states[j].num){
                found=true;
            }
        }
        if (!found )
            return false;
    }
    return true;
}

void DFA::create_DFA(){
    NFA nfa = this->nfa;
    State nfa_start = nfa.get_start();
    D_State start = e_closuree ( nfa.get_start(),nfa);
    start.num = last_num++;
    start.is_visited = false;
    start.is_accepted = false;

    this->set_start(start);
    this->states.push_back(start);

    id = check_unvisited();
    this->states.at(id).is_visited=true;
    D_State unvisited = this->states[id];

    while(id!=-1){
        this->states.at(id).is_visited=true;
        D_State unvisited = this->states[id];

        vector<string> symbols ={"a","b"};
        //vector<string> symbols = parser.get_symbols();
        for (int i=0; i<symbols.size(); i++){
            int q=0;
            if (i==1)
                q=1;

            D_State move_to=move( unvisited,symbols[i],this->nfa,id);
            if (move_to.sub_states.size()==0){
                this->dead_states.push_back(move_to);

            }
            else{
            D_Edge edge = {symbols[i], unvisited, move_to};
            D_State U = e_closure(move_to,edge);

            }

        }

 id = check_unvisited();

    }

}

int DFA::check_unvisited(){
    vector<D_State> D_states = this->states;
    for (int i=0; i<D_states.size(); i++){
        if (D_states[i].is_visited == false){
            return i;

        }

    }
    return -1;

}

void DFA::print_DFA(){

    for (int i=0;i<this->states.size();i++) {
        cout << this->states[i].num << " : ";
        vector<D_Edge> edges = this->states[i].children;
        for (int j=0;j<this->states[i].children.size();j++) {
            cout <<this->states[i].children[j].to.num<<","<<this->states[i].children[j].weight<< " ";
        }
        cout << endl;
    }

}

D_State DFA::e_closure(D_State state,D_Edge edge) {

    stack<State> s;
    D_State e_state;

    for (int i=0;i< state.sub_states.size(); i++)
        s.push(state.sub_states[i]);

    e_state.sub_states = state.sub_states;
    while(!s.empty()){
        State t = s.top();
        s.pop();
        for (int k=0;k<this->nfa.states.size();k++){
            if (this->nfa.states[k].num==t.num){
                t = nfa.states[k];
                break;

            }
        }
        for (int i = 0; i<t.children.size(); i++){
            State u = t.children[i].to;
            if( t.children[i].weight=="e" &&  Find_index_states(u,e_state)==-1){
                e_state.sub_states.push_back(u);
                s.push(u);
            }
        }
    }
    int r=0;

   e_state=check_if_exist(e_state,edge);
         return e_state;



    }

D_State DFA::check_if_exist(D_State e_state, D_Edge edge){
     bool match;
    for (int i=0; i<this->states.size(); i++){
        D_State s = states[i];
         match=false;
        if (s.sub_states.size() == e_state.sub_states.size()){
            for (int j=0; j<e_state.sub_states.size();j++){
                for (int k=0; k<e_state.sub_states.size(); k++){
                    if (e_state.sub_states[j].num == s.sub_states[k].num)
                        match = true;
                }
                if (match == false){
                    match=false;
                    break;

                }
            }
            if (match) {
                e_state.num=i;
                edge.to.num=e_state.num;
                edge.to=e_state;
                this->states.at(id).children.push_back(edge);
                break;
            }
        }
    }
    if (match == false){
        for (int i=0; i<e_state.sub_states.size(); i++){
            if (e_state.sub_states[i].is_accepted== true)
                e_state.is_accepted = true;

        }
        e_state.is_visited= false;
        e_state.num = last_num++;
        edge.to.num=e_state.num;
        this->states.at(id).children.push_back(edge);

        this->states.push_back(e_state);


}

    return e_state;
}


void DFA::set_start(D_State s){
    this->start=s;

}


int main() {

      NFA a,b,c,d,e;
    a.create_NFA("a");
    b.create_NFA("b");
    a= a.Union(a,b);
    a=a.kleene_closure(a);
   // a.print_NFA();
    c.create_NFA("a");
    d.create_NFA("b");
    c.concatenate(c,d);
    e.create_NFA("b");
    c.concatenate(c,e);
    a.concatenate(a,c);
    a.print_NFA();

    DFA dfa;
    dfa.nfa=a;
    dfa.create_DFA();
    dfa.print_DFA();



}

/*
     State a,b ;
    //s.num=0;
   // s.is_accepted=false;
    NFA A,B,C;
    //nfa2.print_NFA();
    A=A.self_loop(a,"a");
    B=B.self_loop(b,"b");
    A.print_NFA();
    B.print_NFA();



    //nfa.print_NFA();

    A=A.expand(A,B,"a");
    A.print_NFA();
    cout<<endl;
 // cout<<"main nfa_start.children_num : "<<A.get_start().children.size()<<endl;



    A.states[1].children.push_back({"a",A.states[1],A.states[0]});
    A.print_NFA();
    cout<<endl;

    State c;
    c.num=2;
    A.states.push_back(c);
    A.states[0].children.push_back({"b",A.states[0],c});
    A.set_start(A.states[0]);
    //cout<<"imp"<<A.states[0].children.size()<<endl;
    A.print_NFA();
    cout<<endl;

    A.states[2].children.push_back({"b",A.states[2],A.states[0]});
    A.print_NFA();
    cout<<endl;

    A.states[2].children.push_back({"b",A.states[2],A.states[1]});
    A.print_NFA();
    cout<<endl;
   // cout<<"main nfa_start.children_num : "<<A.get_start().children.size()<<endl;

   // cout<<"create DFA: while nfa_start : "<<A.get_start().children.size()<<endl;

    DFA dfa ;
    dfa.nfa=A;


    dfa.create_DFA();
   // cout<<"create DFA: while nfa_start : "<<A.get_start().children.size()<<endl;

    dfa.print_DFA();


}
*/




