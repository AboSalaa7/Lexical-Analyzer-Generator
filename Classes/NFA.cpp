#include "NFA.h"
int lastnum = 0;
NFA NFA::create_NFA(string token) {
    State start,end;
    start.is_accepted = false;
    end.is_accepted = true;

    start.num = lastnum++;
    end.num = lastnum++;


    Edge edge = { token, start, end };

    start.children.push_back(edge);
    this->set_start(start);
    this->set_end(end);

    this->states.push_back(start);
    this->states.push_back(end);

    cout << "done create\n";

    return *this;
}

NFA NFA::kleene_closure(NFA a) {

     State start = a.get_start();
     State end = a.get_end();
     State start_new,end_new;

     start.is_accepted = false;
     end.is_accepted = false;
     start_new.is_accepted = false;
     end_new.is_accepted = true;

    start_new.num = lastnum++;
    end_new.num = lastnum++;

     Edge edge1 = { "e", start_new, start};
     Edge edge2 = { "e", end, end_new };
     Edge edge3 = { "e", end, start };
     Edge edge4 = { "e", start_new, end_new};

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

NFA NFA::concatenate(NFA& a, NFA& b) {

  NFA newb;
  if(&a == &b) {
    newb = renameStates(a,b);
  }
  else {
    newb = b;
  }

   State enda = a.get_end();
   cout << "enda: "<< enda.num <<endl;
   State startb = newb.get_start();
   cout << "startb: "<< startb.num <<endl;
   int endanum = enda.num;
   enda.children = startb.children;
   enda.is_accepted =false;

   int iea = Findindex_states(a, a.get_end());
   a.states.at(iea) = enda;

   int isb = Findindex_states(newb, startb);
   newb.states.erase(newb.states.begin() + isb);

   if(newb.states.size() == 2) {
       this->states.push_back(newb.states.at(1));
   }
   else{
       this->states.insert( a.states.end(), newb.states.begin() , newb.states.end() );
   }
   this->set_start(a.get_start());
   this->set_end(newb.get_end());

   return *this;
}

NFA NFA::renameStates(NFA a, NFA b) {
    int state1was,state1is, state2;
    NFA res = b;
    int count = lastnum;
    cout << "lsatnum: "<< count <<endl;
    for (int i = 0; i < res.states.size(); i++) {
        state1was = res.states[i].num;
        cout << "was: "<< res.states[i].num << endl;
        res.states[i].num = count++;
        state1is =  res.states[i].num;

        cout << "is: "<< res.states[i].num << endl;
         for (int j = 0; j < res.states.size(); j++) {        //states
            for (int k = 0; k < res.states[j].children.size(); k++) {     //from j to k
                res.states[j].children.at(k).from.num = res.states[j].num;
                state2 = res.states[j].children.at(k).to.num;
                if(state2 == state1was) {
                    cout << "found : "<< res.states[j].children.at(k).to.num << endl;
                    res.states[j].children.at(k).to.num = state1is;
                    cout << "found is : "<< res.states[j].children.at(k).to.num << endl;
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
    res.print_NFA();
    return res;
}

NFA NFA::Union(NFA& a, NFA& b) {

    NFA newb;
    if( &a == &b ) {
        newb = renameStates(a,b);
    }
    else {
        newb = b;
    }

    State starta = a.get_start();
    State enda = a.get_end();
    State startb = newb.get_start();
    State endb = newb.get_end();
    State start_new,end_new;
    enda.is_accepted = false;
    endb.is_accepted = false;
    start_new.is_accepted = false;
    end_new.is_accepted = true;
    start_new.num = lastnum++;
    end_new.num = lastnum++;

    cout << "enda: "<< enda.num <<endl;
    cout << "endb: "<< endb.num <<endl;
    cout << "startb: "<< startb.num <<endl;
    cout << "starta: "<< starta.num <<endl;

    Edge edge1 = { "e", start_new, starta};
    Edge edge2 = { "e", start_new, startb};
    Edge edge3 = { "e", enda, end_new };
    Edge edge4 = { "e", endb, end_new };

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

State NFA::get_start() {
    return this->start;
}

void NFA::set_start(State a){
    this->start=a;

}

State NFA::get_end() {
    return this->end;
}

void NFA::set_end(State a){
    this->end=a;

}

void NFA::set_NFA(NFA a){
    (*this) = a;
}


void NFA::print_NFA() {
    State start = this->get_start();
    State end = this->get_end();
    cout << "start  = "<< start.num<<endl;
    cout << "end  = "<< end.num<<endl;

    for (vector<State>::iterator it = states.begin(); it != states.end(); ++it) {
        cout << it->num << ": ";
        vector<Edge> edges = it->children;
        for (vector<Edge>::iterator jit = edges.begin(); jit != edges.end(); ++jit) {
            State t = jit->to;
            cout << t.num << " ";
        }
        cout << endl;
    }
}

int NFA::Findindex_states(NFA a, State find) {
    int count = 0;
    for (vector<State>::iterator it = a.states.begin(); it != a.states.end(); ++it) {
        cout<< "find "<<it->num <<endl;
       if(it->num == find.num) {
           return count;
       }
       count++;
    }
}
