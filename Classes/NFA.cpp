#include <bits/stdc++.h>
#include "NFA.h"


NFA::NFA()
{
    //Constructor
    start.num = -1;
    start.is_accepted = false;
    start.accepted_token = "";
    start.visited = false;
    end.num = -1;
    end.is_accepted = false;
    end.accepted_token = "";
    end.visited = false;
}

/*
 * set start state of this NFA
 */
void NFA::set_start_state(state start)
{
    this->start = start;
}

/*
 * get start state of this NFA
 */
state NFA::get_start_state()
{
    return this->start;
}

/*
 * set end state of this NFA
 */
void NFA::set_end_state(state end)
{
    this->end = end;
}

/*
 * get end state of this NFA
 */
state NFA::get_end_state()
{
    return end;
}

/*
 * set the size (# of states) of this NFA
 */
void NFA::set_NFA_size(int s)
{
    this->NFA_size = s;
}

/*
 * get size (# of states) of this NFA
 */
int NFA::get_NFA_size()
{
    return NFA_size;
}

void NFA::clear_visited()
{
    state oldst = get_start_state();
    queue<state> current;
    current.push(oldst);
    while(!current.empty())
    {
        state tmp = current.front();
        tmp.visited = false;
        current.pop();
        vector<edge>* children = &tmp.children;
        for(int i=0; i < children->size(); i++)
        {
            edge e = (*children)[i];
            state to = e.to;
            if(to.visited)
            {
                current.push(to);
            }
        }
    }
}

//lina
extern int lastnum = 0;


NFA NFA::create_NFA(string token) {
    state start,end;
    start.is_accepted = false;
    end.is_accepted = true;

    start.num = lastnum++;
    end.num = lastnum++;



    edge edge = { token, start, end };


    start.children.push_back(edge);

    this->set_start(start);
    this->set_end(end);

    this->states.push_back(start);
    this->states.push_back(end);

    return *this;
}

NFA NFA::kleene_closure(NFA a) {

    state start = a.get_start();
    state end = a.get_end();
    state start_new,end_new;

    start.is_accepted = false;
    end.is_accepted = false;
    start_new.is_accepted = false;
    end_new.is_accepted = true;

    start_new.num = lastnum++;
    end_new.num = lastnum++;

    edge edge1 = { "", start_new, start};
    edge edge2 = { "", end, end_new };
    edge edge3 = { "", end, start };
    edge edge4 = { "", start_new, end_new};

    start_new.children.push_back(edge1);
    start_new.children.push_back(edge4);
    end.children.push_back(edge2);
    end.children.push_back(edge3);

    this->set_start(start_new);
    this->set_end(end_new);

    this->states.push_back(start_new);
    this->states.push_back(end_new);
    int is = Findindex_states(a, start);
    int ie = Findindex_states(a, end);
    this->states.at(is) = start;
    this->states.at(ie) = end;

    return *this;
}


NFA NFA::positiveClosure(string token){
    NFA result;
    result.create_NFA(token);
    edge edge = { token, result.get_end(), result.get_end() };
    result.get_end().children.push_back(edge);
    return result;
}


void print_NFAs(NFA s) {
    state start = s.get_start();
    state end = s.get_end();
    cout << "start  = "<< start.num<<endl;
    cout << "end  = "<< end.num<<endl;

    for (vector<state>::iterator it = s.states.begin(); it != s.states.end(); ++it) {
        cout << it->num << ": ";
        vector<edge> edges = it->children;
        for (vector<edge>::iterator jit = edges.begin(); jit != edges.end(); ++jit) {
            state t = jit->to;
            cout << t.num << " ";
            cout << "(weight = " <<jit->weight << "), ";
        }
        cout << endl;
    }
}


NFA NFA::concatenate(NFA& a, NFA& b) {

    NFA newb = renamestates(a,b);

    state enda = a.get_end();
    state startb = newb.get_start();
    int endanum = enda.num;
    enda.children = startb.children;
    enda.is_accepted = false;

    int iea = Findindex_states(a, a.get_end());
    a.states.at(iea) = enda;

    int isb = Findindex_states(newb, startb);
    newb.states.erase(newb.states.begin() + isb);
    print_NFAs(a);
    cout<<"done"<<endl;
    print_NFAs(newb);
    cout<<"done"<<endl;

    this->states.resize(a.states.size() + newb.states.size());
    cout<<this->states.size()<<"done"<<endl;
    //std::copy(.begin(), x.end(), v.begin());
    std::copy(newb.states.begin(), newb.states.end(), this->states.begin() + a.states.size());
/*
    if(newb.states.size() == 2) {
        this->states.push_back(newb.states.at(1));
    }
    else{
        this->states.insert( a.states.end(), newb.states.begin() , newb.states.end() );
    }
    */
    cout<<"done"<<endl;
    this->set_start(a.get_start());
    this->set_end(newb.get_end());
    print_NFA();
    return *this;
}


NFA NFA::renamestates(NFA a, NFA b) {
    int state1was,state1is, state2;
    NFA res = b;
    int count = lastnum;
    for (int i = 0; i < res.states.size(); i++) {
        state1was = res.states[i].num;
        res.states[i].num = count++;
        state1is =  res.states[i].num;
        for (int j = 0; j < res.states.size(); j++) {        //states
            for (int k = 0; k < res.states[j].children.size(); k++) {     //from j to k
                res.states[j].children.at(k).from.num = res.states[j].num;
                state2 = res.states[j].children.at(k).to.num;
                if(state2 == state1was) {
                    res.states[j].children.at(k).to.num = state1is;
                    if(res.states[j].num == res.get_start().num)
                        res.set_start( res.states[j]);
                    if(res.states[j].num == res.get_end().num)
                        res.set_end( res.states[j]);
                }
                if(state1was == res.get_start().num)
                    res.set_start( res.states[i]);
                if(state1was == res.get_end().num)
                    res.set_end( res.states[i]);
            }
        }
    }
    lastnum = count;
    return res;
}




NFA NFA::Union(NFA& a, NFA& b) {

    NFA newb = renamestates(a,b);

    state starta = a.get_start();
    state enda = a.get_end();
    state startb = newb.get_start();
    state endb = newb.get_end();
    state start_new,end_new;
    enda.is_accepted = false;
    endb.is_accepted = false;
    start_new.is_accepted = false;
    end_new.is_accepted = true;
    start_new.num = lastnum++;
    end_new.num = lastnum++;

    edge edge1 = { "", start_new, starta};
    edge edge2 = { "", start_new, startb};
    edge edge3 = { "", enda, end_new };
    edge edge4 = { "", endb, end_new };

    start_new.children.push_back(edge1);
    start_new.children.push_back(edge2);
    enda.children.push_back(edge3);
    endb.children.push_back(edge4);

    int isa = Findindex_states(a, a.get_start());
    a.states.at(isa) = starta;
    int iea = Findindex_states(a, a.get_end());
    a.states.at(iea) = enda;
    int isb = Findindex_states(newb, newb.get_start());
    newb.states.at(isb) = startb;
    int ieb = Findindex_states(newb, newb.get_end());
    newb.states.at(ieb) = endb;

    NFA res = a;
    res.states.insert( res.states.end(), newb.states.begin() , newb.states.end() );
    res.states.push_back(start_new);
    res.states.push_back(end_new);
    this->states = res.states;

    this->set_start(start_new);
    this->set_end(end_new);
    return *this;
}

state NFA::get_start() {
    return this->start;
}

void NFA::set_start(state a){
    this->start=a;

}

state NFA::get_end() {
    return this->end;
}

void NFA::set_end(state a){
    this->end=a;

}

void NFA::set_NFA(NFA a){
    (*this) = a;
}


void NFA::print_NFA() {
    state start = this->get_start();
    state end = this->get_end();
    cout << "start  = "<< start.num<<endl;
    cout << "end  = "<< end.num<<endl;

    for (vector<state>::iterator it = states.begin(); it != states.end(); ++it) {
        cout << it->num << ": ";
        vector<edge> edges = it->children;
        for (vector<edge>::iterator jit = edges.begin(); jit != edges.end(); ++jit) {
            state t = jit->to;
            cout << t.num << " ";
            cout << "(weight = " <<jit->weight << "), ";
        }
        cout << endl;
    }
}




int NFA::Findindex_states(NFA a, state find) {
    int count = 0;
    for (vector<state>::iterator it = a.states.begin(); it != a.states.end(); ++it) {
        if(it->num == find.num) {
            return count;
        }
        count++;
    }
    return count;
}

int NFA::count_weights(){
    for (vector<state>::iterator it = states.begin(); it != states.end(); ++it) {
        vector<edge> edges = it->children;
        for (vector<edge>::iterator jit = edges.begin(); jit != edges.end(); ++jit) {
            if (std::find(weights.begin(), weights.end(), jit->weight) == weights.end()) {
                this->weights.push_back(jit->weight);
                cout<<jit->weight<<endl;
            }

        }
        cout << endl;
    }

    return this->weights.size();
}


/* Destructor */
NFA::~NFA()
{
    //don't forget Destructor in c++ => no garbage collector
}
