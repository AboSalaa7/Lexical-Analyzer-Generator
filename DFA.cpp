#include "DFA.h"
#include <stack>


int Find_index_states(state u,D_State e_state){
    for (int i=0;i<e_state.sub_states.size();i++) {
        if( e_state.sub_states[i].num== u.num)
            return i;
    }
    return -1;
}


D_State DFA::move( D_State T, string symbol ){

    D_State res;
    NFA nfa=this->nfa;
    for (int i=0; i<T.sub_states.size(); i++){
        state t = T.sub_states[i];
        for (int k=0;k<nfa.states.size();k++){
            if (nfa.states[k].num==t.num){
                t = nfa.states[k];
                break;
            }
        }

        for (int j=0; j<t.children.size(); j++){
            if (t.children[j].weight == symbol){
                state u = t.children[j].to;
                if(Find_index_states(u,res)== -1){
                    res.sub_states.push_back(u);
                }
            }
        }
    }
    return res;
}


D_State e_closuree(state state2, NFA nfa) {

    stack<state> s;
    D_State e_state;
    s.push(state2);
    e_state.sub_states.push_back(state2);
    while(!s.empty()){
        state t = s.top();
        s.pop();
        for (int k=0;k<nfa.states.size();k++){
            if (nfa.states[k].num==t.num){
                t = nfa.states[k];
                break;
            }
        }
        for (int i = 0; i<t.children.size(); i++){
            state u = t.children[i].to;
            if(Find_index_states(u,e_state)== -1&& t.children[i].weight==""){
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
extern int last_num=0;
extern int id=0;

void DFA::create_DFA(){
    NFA nfa = this->nfa;
    state nfa_start = nfa.get_start();
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

        vector<string> symbols ={"*","/"};
      //  vector<string> symbols = parser.get_symbols();
        for (int i=0; i<symbols.size(); i++){
            D_State move_to=move( unvisited,symbols[i]);
            D_Edge edge = {symbols[i], unvisited, move_to};
            D_State U = e_closure(move_to,edge);
        }
        id = check_unvisited();
    }
    cout<<"DFA create numberof states:"<<states.size()<<endl;
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
        if (this->states[i].is_accepted)
            cout<<this->states[i].accepted_token;
        vector<D_Edge> edges = this->states[i].children;
        for (int j=0;j<this->states[i].children.size();j++) {
            cout <<this->states[i].children[j].to.num<<","<<this->states[i].children[j].weight<< " ";
        }
        cout << endl;
    }

        cout<<this->states[3].accepted_token;
}

D_State DFA::e_closure(D_State Dstate,D_Edge edge) {

    stack<state> s;
    D_State e_state;

    for (int i=0;i< Dstate.sub_states.size(); i++)
        s.push(Dstate.sub_states[i]);

    e_state.sub_states = Dstate.sub_states;
    while(!s.empty()){
        state t = s.top();
        s.pop();
        for (int k=0;k<this->nfa.states.size();k++){
            if (this->nfa.states[k].num==t.num){
                t = nfa.states[k];
                break;

            }
        }
        for (int i = 0; i<t.children.size(); i++){
            state u = t.children[i].to;
            if( t.children[i].weight=="" &&  Find_index_states(u,e_state)==-1){
                e_state.sub_states.push_back(u);
                s.push(u);
            }
        }
    }
    if (e_state.sub_states.size()==0)
        this->dead_states.push_back(edge);
    else
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
                if (match == false)
                    break;
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
        e_state.is_accepted=false;
        vector<state> accept_states;
        for (int j=0;j<this->nfa.states.size();j++)
            if (this->nfa.states[j].is_accepted)
                accept_states.push_back(this->nfa.states[j]);
        for (int i=0; i<e_state.sub_states.size(); i++){
            for (int j=0;j<accept_states.size();j++){
                if (e_state.sub_states[i].num== accept_states[j].num){
                    e_state.is_accepted = true;
                    e_state.accepted_token = accept_states[j].accepted_token;

            }

            }
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

void DFA::minimization() {
    int weights = this->nfa.count_weights() - 1;
    //cout<<"weights : "<<weights<<endl;
    map <int, int> set;
    vector<D_State> empty;
    for(int i=0;i<2;i++) {
        Partition.push_back(empty);
    }
    //fill(Partition.begin(), Partition.end(), empty);
    for(int i=0;i<states.size();i++){
        if(states.at(i).is_accepted) {
            Partition[0].push_back(states.at(i));
            set.insert(pair<int,int>(states[i].num, 0));
        }
        else {
            Partition[1].push_back(states.at(i));
            set.insert(pair<int,int>(states[i].num, 1));
        }
    }
   // cout<<"2 part"<<endl;
   // print_mini();

    int partitionIndex = 2;

    bool loop = true;
	while(loop) {
        loop = false;
        for(int i=0;i<Partition.size();i++){
            for(int j=0;j<Partition[0].size();j++){
                vector <D_State> current;
                current.push_back(Partition[0].at(j));
               // cout<<endl;
                //cout<<"Current"<<endl;
               // print_current(current);
                int first_state = Partition[0].at(j).num;
                //cout<<endl;
                //cout<<"first_state : "<<first_state<<endl;

                for(int k = j+1 ; k < Partition[0].size() ; k++) {
                    int second_state = Partition[0].at(k).num;
                    //cout<<endl;
                    //cout<<"2nd_state : "<<second_state<<endl;
                    int count =0;
                    for(int l = 0 ; l < Partition[0].at(j).children.size() ; l++) {     //children of 1st state
                        D_Edge first_state_child =  Partition[0].at(j).children.at(l);
                        //cout<<"1st_state_child : "<<first_state_child.to.num<<endl;
                        for(int m = 0 ; m < Partition[0].at(k).children.size() ; m++) {     //children of 2nd state
                            D_Edge second_state_child =  Partition[0].at(k).children.at(m);
                           /* cout<<"2nd_state_child : "<<second_state_child.to.num<<endl;
                            cout<<endl;*/
                            if( (first_state_child.weight == second_state_child.weight) &&
                                (set[first_state_child.to.num] == set[second_state_child.to.num])){
                                count++;
                                //cout<<"yes compatible!!"<<endl;
                            }
                        }
                    }
                    //cout<<"count : "<<count<<endl;
                    if (count == weights) {
                        current.push_back(Partition[0].at(k));
                        Partition[0].erase(Partition[0].begin() + k);
                        k--;
                       /* cout<<endl;
                        cout<<"Current"<<endl;
                        print_current(current);
                        cout<<endl;
                        cout<<"Partition Current"<<endl;
                        print_mini();*/
                    }
                    else {
                        loop = true;
                    }
                }
                for (int k =0 ; k<current.size();k++)
                {
                    set[current[k].num] = partitionIndex;
                }
                Partition.push_back(current);
                Partition[0].erase(Partition[0].begin() + j);
                partitionIndex++;
                j--;
              //  cout<<partitionIndex<<" part"<<endl;
              //  print_mini();
            }
            Partition.erase(Partition.begin());
        }
    }

    for(int i = 0 ; i < states.size() ; i++) {
        int state = states[i].num;

        if (state!=-1) {
            states[i].num = set[state];
        }
     //   cout<<"set[state] : "<<set[state]<<endl;
      //  cout<<"state : "<<state<<endl;
        for(int j = 0 ; j < states[i].children.size() ; j++)
        {
            state = states[i].children.at(j).to.num;
            if (state!=-1) {
                states[i].children.at(j).to.num = set[state];
            }

        //    cout<<"child set[state] : "<<set[state]<<endl;
        //    cout<<"child state : "<<state<<endl;
        }
    }
    //remove duplicates
     for(int i=0; i<states.size(); i++)
    {
        for(int j=i+1; j<states.size(); j++)
        {
        /* If any duplicate found */
            if(states[i].num == states[j].num)
            {
                states.erase(states.begin() + j);
            }
        }
    }

}


void DFA::print_mini(){
    cout<<"\n\n\n\n\n"<<endl;
    for (int g=0;g<Partition.size();g++){
    for (int i=0;i<Partition[g].size();i++) {
        cout << g<<endl;
        cout << Partition[g][i].num << " : ";
        vector<D_Edge> edges = Partition[g][i].children;
        for (int j=0;j<Partition[g][i].children.size();j++) {
            cout <<Partition[g][i].children[j].to.num<<","<<Partition[g][i].children[j].weight<< " ";
        }
        cout << endl;
    }
    }
}

    D_State DFA::get_start(){
        return this->states[0];
    }






/*

int main() {


    NFA a,b,c,d,e,f;

    a.create_NFA("a");

    b.create_NFA("a");
    c.create_NFA("b");
    b.concatenate(b,c);
    d.create_NFA("b");
    b.concatenate(b,d);

    e.create_NFA("a");
    e=e.kleene_closure(e);
    f.create_NFA("b");
    f=f.positiveClosure(f);
    e.concatenate(e,f);

    a.Union(a,b);
    a.Union(a,e);

    a.print_NFA();
    DFA dfa;
    dfa.nfa=a;
    dfa.create_DFA();
    //dfa.print_DFA();
    dfa.minimization();
    dfa.print_mini();
    cout<<"\n\n\n\n\n"<<endl;
    dfa.print_DFA();






    return 0;
}
*/

