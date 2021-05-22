#include <iostream>
#include <regex>
#include <stack>
#include "parser.h"

parser::parser(string RegexFile)
{
    this->RegexFile = RegexFile;
}

NFA* parser::getFullNFA()
{

    NFA g2;
    readRegexFile(this->RegexFile);
    for(int i = 0; i < Regex.size(); i++)
    {
        string line = Regex[i];
        getNFA(line);
    }
    for(int i=0; i< expNames.size(); i++)
    {
        NFA g2 = expressions[expNames[i]];
        state start;
        state st2 = g2.get_start_state();
        edge e1;
        e1.from = start;
        e1.to= st2;
        e1.weight = "";
        start.children.push_back(e1);
    }
   // setWeights(g);
    //   printNFA(g);
    return NULL;
}

void parser::readRegexFile(string path)
{
    ifstream infile;
    infile.open (path);
    string str;
    while(!infile.eof())
    {
        getline(infile,str);
        if(str.size()>0)
        {
            Regex.push_back(str);
        }
    }
    infile.close();
}

void parser::getNFA(string line)
{
    NFA result, resultBuilder;
    result = resultBuilder.create_NFA("");
    state temp;
    temp.num = result.get_end().num;
    temp.visited = result.get_end().visited;
    temp.children = result.get_end().children;
    temp.accepted_token = result.get_end().accepted_token;
    temp.is_accepted = false;
    result.set_end(temp);
    regex splitter1("\\{\\s*([^.]+)\\s*\\}");
    regex splitter2("\\[\\s*([^.]+)\\s*\\]");
    regex splitter3 ("\\s*(\\S+)\\s*(=|:)\\s*(.+)");
    regex spaceSplitter("(\\S+)");
    std::smatch match1;
    cout << line << endl;
    if(regex_match(line,splitter1))
    {
        cout << "matched {" << endl;
        regex_search(line,match1,splitter1);
        string str = match1[1];
        while(regex_search(str,match1,spaceSplitter))
        {
            string tok = match1[0];
            cout << "token is " << tok << endl;


            //NFA* g = new NFA();

            string token = "";
            for(int i = 0; i < tok.length(); i++)
            {
                if(tok[i] != '\\')
                {
                    token.push_back(tok[i]);
                }
            }
            for(int i = 0; i < token.length(); i++)
            {   NFA g2 ,test,test2;
                string c = "";
                c.push_back(token[i]);
                g2 = test.create_NFA(c);
                result = test2.Union(result,g2);
            }

            //printNFA(g);
            expressions[token] = result;
            expNames.push_back(token);
           // setWeights(g);
            str = match1.suffix().str();
        }
        //return result;
    }
    else if(regex_match(line,splitter2))
    {
        cout << "matched [" << endl;
        regex_search(line,match1,splitter2);
        string str = match1[1];
        while(regex_search(str,match1,spaceSplitter))
        {
            string tok = match1[0];
            string token = "";
            for(int i = 0; i < tok.length(); i++)
            {
                if(tok[i] != '\\')
                {
                    token.push_back(tok[i]);
                }
            }
            for(int i = 0; i < token.length(); i++)
            {

                NFA g2 ,test,test2;
                string c = "";
                c.push_back(token[i]);
                g2 = test.create_NFA(c);
                result = test2.Union(result,g2);
                g2.print_NFA();
                cout << c << endl;

            }
          //  printNFA(g);
            expressions[token] = result;
            expNames.push_back(token);
            //       setWeights(g);
            str = match1.suffix().str();
        }
     //   return result;
    }
    else if (regex_match(line,splitter3))
    {
        regex_search(line,match1,splitter3);
        string acceptance = match1[1];
        string str2 = match1[3];
        vector<string> tokens = getTokens(str2);
        stack<string> postfix = getPostfix(tokens);
        cout << postfix.size()<<endl;
        NFA resultIn;
        cout << acceptance << " is drawing" << endl;
        stack<NFA> graphs;
        string token;
        cout << postfix.size() << endl;
        while(postfix.size()!=0)
        {
            token = postfix.top();
            cout << "token : " << token << endl;
            postfix.pop();
            bool point=false;
            if( isOperation(token) )
            {
                cout << "is operation" << endl;

                NFA G1,G2;
                NFA builder;
                switch(token[0])
                {
                    case '+':
                        G1 = graphs.top();
                        graphs.pop();
                        resultIn = G1.positiveClosure(acceptance);
                        //printNFA(g);
                        break;
                    case '*':
                        G1 = graphs.top();
                        graphs.pop();
                        resultIn = G1.kleene_closure(G1);
                        //printNFA(g);
                        break;
                    case '|':
                        G1 = graphs.top();
                        graphs.pop();
                        G2 = graphs.top();
                        graphs.pop();
                        //cout<<"entered or"<<endl;
                        resultIn = resultIn.Union(G1,G2);
                        //printNFA(g);
                        break;
                    case '#':
                        G1 = graphs.top();
                        graphs.pop();
                        G2 = graphs.top();
                        graphs.pop();
                        resultIn = resultIn.concatenate(G1,G2);
                        break;
                }
                graphs.push(resultIn);
            }
            else
            {
                regex split1("(\\S+)-(\\S+)");
                std::smatch match1;
                if(regex_match(token,split1))
                {
                    cout << "dash found" << endl;
                    regex_search(token,match1,split1);
                    string f = match1[1];
                    string sc = match1[2];
                    int first = f[0];
                    int second = sc[0];
                    NFA g2,g2build;
                    g2 = g2build.create_NFA("");
                    for(int i=first; i <= second; i++)
                    {
                        string s = "";
                        char c = i;
                        s.push_back(c);
                        NFA g3,test;
                        g3= g3.create_NFA(s);
                        g2 = g2.Union(g2,g3);
                    }
                    resultIn = g2;
                    graphs.push(g2);
                    //printNFA(g);
                }
                else
                {
                    unordered_map<string,NFA>::const_iterator found = definitions.find(token);
                    NFA G;

                    if(found != definitions.end())
                    {
                        resultIn = definitions[token];
                        point=true;
                    }
                    else if(token.length()==0)
                    {
                        resultIn = G.create_NFA("");

                        //printNFA(g);
                    }
                    else
                    {
                        G = G.create_NFA("");
                        cout << "token to be added now" << token << endl;
                        for(int i = 0; i < token.length(); i++)
                        {
                            string c = "";
                            if(token[i]  == '\\' && token.length() > 1){
                                continue;
                            }
                            c.push_back(token[i]);
                                                     //   cout<<"before error"<<endl;
                                                     //   cout<<c<<endl;

                            NFA G2;
                            G2.create_NFA(c);
                                                     //   cout<<"before error"<<endl;

                            resultIn = G.Union(G,G2);
                            cout<<"end error"<<endl;
                        }

                    }
                    graphs.push(resultIn);
                }

            }
        }
        cout << acceptance << " ended drawing" << endl;
        NFA g ;
        cout << "returned" << endl;
        // printNFA(g);
        if(!match1[2].compare("="))
        {
            definitions[acceptance] = resultIn;
            //      setWeights(g);
        }
        else
        {
            expressions[acceptance] = resultIn;
            expNames.push_back(acceptance);
        }
    }
}

void parser::addOperation(char c, stack<string>* in, stack<string>* post)
{
    string operation = "";
    string cha;
    if(in->size()==0)
    {
        operation.push_back(c);
        in->push(operation);
    }
    else
    {
        if(c=='*' || c == '+')
        {
            while(in->size()!=0)
            {
                cha= in->top();
                if(!cha.compare("*") || !cha.compare("+"))
                {
                    in->pop();
                    post->push(cha);
                }
                else
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
                if(in->size()==0)
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
            }
        }
        else if(c=='#')
        {
            while(in->size()!=0)
            {
                cha= in->top();
                if(!cha.compare("#") || !cha.compare("*") || !cha.compare("+"))
                {
                    in->pop();
                    post->push(cha);
                }
                else
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
                if(in->size()==0)
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
            }
        }
        else if(c=='|')
        {
            while(in->size()!=0)
            {
                cha= in->top();
                if(!cha.compare("|") || !cha.compare("#") || !cha.compare("*") || !cha.compare("+"))
                {
                    in->pop();
                    post->push(cha);
                }
                else
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
                if(in->size()==0)
                {
                    operation.push_back(c);
                    in->push(operation);
                    break;
                }
            }
        }
    }
}

bool parser::isOperation(char c)
{
    if(c == '+' ||c == '*' ||c == '|' ||c == '(' ||c == ')' || c == '#')
        return true;
    return false;
}

string parser::removeSpaces(string str)
{
    string temp = "";
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
        {
            temp += str[i];
        }
    }
    return temp;
}

vector<string> parser::getTokens(string str2)
{
    string str = "";
    for(int i=0; i<str2.length(); i++)
    {
        char c = str2[i];
        if(isOperation(c))
        {
            str.push_back(c);
            if(str2[i+1] != ' ')
            {
                str.push_back(' ');
            }
        }
        else if(isOperation(str2[i+1]) && str2[i] != '\\')
        {
            str.push_back(c);
            if(c != ' ')
            {
                str.push_back(' ');
            }
        }
        else
        {
            str.push_back(c);
        }
    }
    regex range ("(\\S\\s+-\\s+\\S)|(\\S\\s*-\\s+\\S)|(\\S\\s+-\\s*\\S)");
    string temp = "";
    temp += str;
    std::smatch match1;
    string prev = temp;
    while(regex_search(temp,match1,range))
    {
        string start = match1[0];
        string star = removeSpaces(start);
        str.replace(str.find(start),start.length(),star);
        temp = str;
        if (!temp.compare(prev))
            break;
    }
    vector<string> tokens;
    regex spaceSplitter("(\\S+)");
    while(regex_search(str,match1,spaceSplitter))
    {
        string token = match1[0];
        if(token.length() == 2 && token[0] == '\\' && token[1] == 'L')
        {
            //cout << "vvvvvvvvvvvvvvvvv" << endl;
            token = "";
        }
        /*if(token.length() == 2 && token[0] == '\\')
        {
            //cout << "vvvvvvvvvvvvvvvvv" << endl;
            token.push_back(token[1]);
        }*/
        //cout << "hhhhhhhhh : " << token.length() <<token << endl;
        tokens.push_back(token);
        str = match1.suffix().str();
    }
    return tokens;
}

stack<string> parser::getPostfix(vector<string> tokens)
{
    stack<string> post;
    stack<string> in;
    string cha,ch;
    for(int j=0; j < tokens.size(); j++)
    {
        string token = tokens[j];
        if(!isOperation(token))
        {
            if (j!= 0 && !isBinaryOp(tokens[j - 1]))
            {
                addOperation('#',&in,&post);
            }
            post.push(token);
        }
        else
        {
            if(!token.compare("("))
            {
                if (j != 0 && tokens[j - 1].compare("|"))
                    addOperation('#',&in,&post);
                in.push(token);
            }
            else if(!token.compare(")"))
            {
                while(true)
                {
                    cha= in.top();
                    in.pop();
                    if(!cha.compare("("))
                    {
                        if (j != tokens.size() - 1 && !isOperation(tokens[j + 1]))
                            addOperation('#',&in,&post);
                        break;
                    }
                    else
                    {
                        post.push(cha);
                    }
                }
            }
            else
            {
                addOperation(token[0], &in, &post);
            }
        }
    }
    while(in.size()!=0)
    {
        ch= in.top();
        in.pop();
        post.push(ch);
    }
    stack<string> postfix;
    while (!post.empty())
    {
        postfix.push(post.top());
        post.pop();
    }
    return postfix;
}

bool parser::isOperation(string c)
{
    if(!c.compare("+") ||!c.compare("*") ||!c.compare("|") ||!c.compare("(") ||!c.compare(")") ||!c.compare("#"))
        return true;
    return false;
}

bool parser::isBinaryOp(string c)
{
    if (!c.compare("|") ||!c.compare(")") ||!c.compare("("))
        return true;
    return false;
}

NFA* parser::clone(NFA* g)
{
    unordered_map<int,state> clones;
    state oldst = g->get_start_state();
    int endnum = 0, oldnum = oldst.num;
    string endAcceptance = "";
    NFA* newg = new NFA();
    vector<pair<string,pair<int,int>>> edges;
    queue<state> current;
    if(oldst.visited)
    {
        return newg;
    }
    oldst.visited = true;
    current.push(oldst);
    while(!current.empty())
    {
        state tmp = current.front();
        cout << "state removed : " <<tmp.num << endl;
        state newst ;
        newst.is_accepted = false;
        newst.accepted_token = "";
        newst.visited = false;
        newst.num = statenum++;
        clones[tmp.num] = newst;
        if(tmp.num == oldnum)
        {
            newg->set_start_state(newst);
        }
        cout << " state : " << clones[tmp.num].num;
        cout << "   with : " << tmp.num << endl;
        current.pop();
        vector<edge>* children = &tmp.children;
        if(children->empty())
        {
            endnum = tmp.num;
            endAcceptance = tmp.accepted_token;
        }
        for(int i=0; i < children->size(); i++)
        {
            edge e = (*children)[i];
            state to = e.to;
            if(!to.visited)
            {
                current.push(to);
                to.visited = true;
                cout << "child added : " << to.num << endl;
            }
            pair<int,int> x;
            x.first = tmp.num;
            x.second = to.num;
            pair<string,pair<int,int>> p;
            p.first = e.weight;
            p.second = x;
            edges.push_back(p);
            cout << p.second.first << ":" << p.second.second << "->>" << p.first << endl;
        }
    }
    for(int i=0; i < edges.size(); i++)
    {
        int from = edges[i].second.first;
        int to = edges[i].second.second;
        string weight = edges[i].first;
        edge e1;
        e1.from = clones[from];
        e1.to = clones[to];
        e1.weight = weight;
        clones[from].children.push_back(e1);
        cout << clones[from].num << ":" << clones[to].num << "->>" << weight << endl;
    }
    cout << endl;
    clones[endnum].is_accepted = true;
    clones[endnum].accepted_token = endAcceptance;
    newg->set_end_state(clones[endnum]);
    g->clear_visited();
    return newg;
}



vector<string> parser::get_weights()
{
    return weights;
}


vector<string> parser::get_expressions()
{
    return expNames;
}

parser::~parser()
{
    //dtor
}
