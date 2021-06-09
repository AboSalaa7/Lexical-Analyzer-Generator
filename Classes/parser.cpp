#include <iostream>
#include <regex>
#include <stack>
#include <fstream>
#include <sstream>
#include "parser.h"

parser::parser(string RegexFile, string opFile)
{
    this->RegexFile = RegexFile;
    this->opFile = opFile;
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
        cout << "expression number "<< i << endl;
        cout << expNames[i] << endl;

    }
    //setWeights(g2);
    //g2.print_NFA();
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
    result = result.create_NFA("");
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
                g2.print_NFA();
                result = result.concatenate(result,g2);
                result.print_NFA();
            }

            expressions[token] = result;
            expNames.push_back(token);
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
                g2.print_NFA();
                result.Union(result,g2);
                result.print_NFA();
                cout << c << endl;

            }
            expressions[token] = result;
            expNames.push_back(token);
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
                        resultIn = G1.positiveClosure(G1);

                        break;
                    case '*':
                        G1 = graphs.top();
                        graphs.pop();
                        resultIn = G1.kleene_closure(G1);

                        break;
                    case '|':
                        G1 = graphs.top();
                        graphs.pop();
                        G2 = graphs.top();
                        graphs.pop();
                        resultIn = G1.Union(G1,G2);

                        break;
                    case '#':
                        G1 = graphs.top();
                        graphs.pop();
                        G2 = graphs.top();
                        graphs.pop();
                        resultIn = G1.concatenate(G1,G2);

                        break;
                }
                resultIn.print_NFA();
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
                    cout<<token<<endl;
                    NFA g2,g2build, g3;
                    g2 = g2build.create_NFA("");
                    g3= g3.create_NFA(token);
                    g2 = g2.Union(g2,g3);
                   /* string f = match1[1];
                    string sc = match1[2];
                    int first = f[0];
                    int second = sc[0];
                    NFA g2,g2build;
                    g2 = g2build.create_NFA("");
                    cout<<"g2 "<<endl;
                    g2.print_NFA();
                    for(int i=first; i <= second; i++)
                    {
                        string s = "";
                        char c = i;
                        s.push_back(c);
                        NFA g3,test;
                        g3= g3.create_NFA(s);
                        cout<<"g3 "<<endl;
                        g3.print_NFA();
                        g2 = g2.Union(g2,g3);
                        cout<<"g2 "<<endl;
                        g2.print_NFA();
                    }*/
                    resultIn = g2;
                    resultIn.print_NFA();
                    graphs.push(g2);
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
                            NFA G2;
                            G2.create_NFA(c);
                            resultIn = G.Union(G,G2);
                        }

                    }
                    resultIn.print_NFA();
                    graphs.push(resultIn);
                }
            }
        }
        cout << acceptance << " ended drawing" << endl;
        NFA g ;
        cout << "returned" << endl;
        if(!match1[2].compare("="))
        {
            definitions[acceptance] = resultIn;
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
            token = "";
        }
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




vector<string> parser::get_expressions()
{
    return expNames;
}

unordered_map<string,NFA> parser::get_allnfa()
{
    return expressions;
}




void parser::readOpFile(){
    ifstream inFile;
    inFile.open (this->opFile);
    string str;
    int i=0;
    while(!inFile.eof())
    {
        while (getline(inFile, str))
        {
            istringstream ss(str);
            string token;
            //Separate string based on commas and white spaces
            bool check= false;
            while (getline(ss,token,' ')) {
                check = false;
                if ((token.find(";") != std::string::npos) && (token.size() != 1)){
                    token.erase(remove(token.begin(), token.end(), ';'), token.end());
                    opFileRegex.push_back(token);
                    opFileRegex.push_back(";");
                    check = true;
                }
                if ((token.find(",") != std::string::npos) && (token.size() != 1)){
                    token.erase(remove(token.begin(), token.end(), ','), token.end());
                    opFileRegex.push_back(",");
                    opFileRegex.push_back(token);
                    check = true;
                }
                if((token.find("(") != std::string::npos) && (token.size() != 1)) {
                    token.erase(remove(token.begin(), token.end(), '('), token.end());
                    opFileRegex.push_back("(");
                    opFileRegex.push_back(token);
                    check = true;
                }
                if ((token.find(")") != std::string::npos) && (token.size() != 1)){
                    token.erase(remove(token.begin(), token.end(), ')'), token.end());
                    opFileRegex.push_back(token);
                    opFileRegex.push_back(")");
                    check = true;
                }
                if(check == false) {
                    opFileRegex.push_back(token);
                }
            }
        }
    }
    inFile.close();
    /* for (int i=0;i<opFileRegex.size();i++) {
         cout<<opFileRegex[i]<<endl;
     }*/
}


vector<string> parser::getOpFile() {

    ofstream myfile;
    myfile.open ("output.txt");

    for(int i = 0; i < opFileRegex.size(); i++)
    {
        string token = opFileRegex[i];
        string op = parseToken(token);
        myfile << op;

    }
    myfile.close();
}


string parser::parseToken(string token) {
    vector<D_State> transitions;
    D_State next;
    D_State current = this->dfa.get_start();
    transitions.push_back(current);
    bool found=false;
    for (int i=0;i<token.size();i++){
        found=false;
        string symbol(1,token[i]);
        for (int k=0;k<current.children.size();k++){
            if (current.children[k].weight == symbol){
                found=true;
                next = current.children[k].to;
                for (int q=0;q<this->dfa.states.size();q++){
                    if (this->dfa.states[q].num == next.num)
                        next=this->dfa.states[q];
                }
            }
        }

        if (found){
            transitions.push_back(next);
            current=next;
        }
        else{
            for (int j=transitions.size()-1;j>=0;j--){
                if (transitions[j].is_accepted){
                    // cout<< "accept state num: "<<transitions[j].num<<endl;
                    // break;
                    return transitions[j].accepted_token;
                }
            }
        }
    }
    return transitions[transitions.size()-1].accepted_token;
    //  cout<<"accepttttt stateee nummm ::: "<<transitions.[transitions.size()-1].num<<endl;
}

parser::~parser()
{
    //dtor
}
