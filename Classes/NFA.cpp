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
                            //cout<<"create :before error1"<<endl;



    edge edge = { token, start, end };
                               // cout<<"create :before error2"<<endl;



    start.children.push_back(edge);
                              //  cout<<"create :before error3"<<endl;


    this->set_start(start);
                                //    cout<<"create :before error4"<<endl;

    this->set_end(end);

    this->states.push_back(start);
    this->states.push_back(end);
                             //   cout<<"create :before error5"<<endl;

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


    /*cout<<"start.num "<<start.num<<endl;
    cout<<"end.num "<<end.num<<endl;*/
    lastnum = end.num+1;
    //cout<<"lastnum"<<lastnum<<endl;
    start_new.num = lastnum++;
    end_new.num = lastnum++;
    /*cout<<"start_new.num "<<start_new.num<<endl;
    cout<<"end_new.num "<<end_new.num<<endl;*/



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

   // this->print_NFA();
    NFA renamed = renameallstates(*this);
    this->states = renamed.states;
    this->set_start(renamed.get_start());
    this->set_end(renamed.get_end());
    return *this;
}


NFA NFA::positiveClosure(NFA a){
   /* NFA result;
    result.create_NFA(token);
    edge edge = { token, result.get_end(), result.get_end() };
    result.get_end().children.push_back(edge);*/
    NFA b = a;
    state starta = a.get_start();
    state enda = a.get_end();
    state sec_state, end_new;

    starta.is_accepted = false;
    enda.is_accepted = false;
    sec_state.is_accepted = false;
    end_new.is_accepted = true;


    lastnum = enda.num+1;
    sec_state.num = lastnum++;
    end_new.num = lastnum++;


    edge edge1 = { "", sec_state, starta};
    edge edge2 = { "", enda, end_new };
    edge edge3 = { "", enda, starta };
    edge edge4 = { "", sec_state, end_new};

    sec_state.children.push_back(edge1);
    sec_state.children.push_back(edge4);
    enda.children.push_back(edge2);
    enda.children.push_back(edge3);


    int is = Findindex_states(a, starta);
    int ie = Findindex_states(a, enda);
    this->states.at(is) = starta;
    this->states.at(ie) = enda;
    this->states.push_back(sec_state);
    this->states.push_back(end_new);
    this->set_start(sec_state);
    this->set_end(end_new);


    b.concatenate(b,(*this));
    (*this)=b;

    NFA renamed = renameallstates(*this);
    this->states = renamed.states;
    this->set_start(renamed.get_start());
    this->set_end(renamed.get_end());
    return *this;
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
    this->states.at(iea) = enda;

    int isb = Findindex_states(newb, startb);
    newb.states.erase(newb.states.begin() + isb);

    insertingVectorb(newb);

    this->set_start(a.get_start());
    this->set_end(newb.get_end());

    NFA renamed = renameallstates(*this);
    this->states = renamed.states;
    this->set_start(renamed.get_start());
    this->set_end(renamed.get_end());
    return *this;
}


void NFA::insertingVectorb(NFA b) {
    for (int i = 0; i < b.states.size(); i++) {
        this->states.push_back(b.states.at(i));
    }
}


NFA NFA::renamestates(NFA a, NFA b) {
    int state1was,state1is, state2;
    NFA res = b;
    int count;
    state enda = a.get_end();
    state endb = b.get_end();
    if(endb.num > enda.num) {
        count = endb.num+1;
    }
    else {
        count = enda.num+1;
    }
    //cout<<"count : "<<count<<endl;
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


NFA NFA::renameallstates(NFA res) {
    int state1was,state1is, state2;
    int count = 0;
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
    //b.print_NFA();
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
   // newb.print_NFA();

 /*   cout<<"starta.num "<<starta.num<<endl;
    cout<<"enda.num "<<enda.num<<endl;
    cout<<"startb.num "<<startb.num<<endl;
    cout<<"endb.num "<<endb.num<<endl;*/
    if(endb.num > enda.num) {
        lastnum = endb.num+1;
    }
    else {
        lastnum = enda.num+1;
    }

   // cout<<"lastnum"<<lastnum<<endl;
    start_new.num = lastnum++;
    end_new.num = lastnum++;
   /* cout<<"start_new.num "<<start_new.num<<endl;
    cout<<"end_new.num "<<end_new.num<<endl;*/

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
    //a.print_NFA();
   //newb.print_NFA();
   // res.states.insert( res.states.end(), newb.states.begin() , newb.states.end() );

   /* this->states.reserve( res.states.size() + newb.states.size() ); // preallocate memory

    this->states.insert( this->states.end(), newb.states.begin(), newb.states.end() );*/
    insertingVectorb(newb);
    this->states.push_back(start_new);
    this->states.push_back(end_new);
   // this->states = res.states;

    this->set_start(start_new);
    this->set_end(end_new);

    //this->print_NFA();
    NFA renamed = renameallstates(*this);
   /* cout<<"renamed"<<endl;
    renamed.print_NFA();*/
    this->states = renamed.states;
    this->set_start(renamed.get_start());
    this->set_end(renamed.get_end());
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

int main() {
    NFA test,res,k;
    string a = "a";
    string b = "b";
    string c = "c";
    //aaa+b*bba+ccc*
    NFA x = test.create_NFA(a);
   // test.print_NFA();
    NFA y = res.create_NFA(b);
   // res.print_NFA();
    NFA z = k.create_NFA(c);
   // z.print_NFA();
   x.concatenate(x,y);
   x.positiveClosure(x);
   x.print_NFA();
   /*
    test.concatenate(test,test);
   // test.print_NFA();
    test.concatenate(test,x);
    test.print_NFA();
    res.kleene_closure(res);
    //res.print_NFA();
    res.concatenate(res,y);
    //res.print_NFA();
    res.concatenate(res,y);
   // res.print_NFA();
    res.concatenate(res,x);
    res.print_NFA();
    k.concatenate(k,z);
  //  k.print_NFA();
    z.kleene_closure(z);
   // z.print_NFA();
    k.concatenate(k,z);
    //k.print_NFA();

    test.Union(test,res);
    test.print_NFA();
    test.Union(test,k);
    test.print_NFA();
    */
    return 0;
}

