#include "table.h"


vector<string> table::find_productions(string start)
{
    vector<string> res;
    for (int i=0; i<productions.size(); i++)
    {
        for (int j=0; j<productions[i].size(); j++)
        {
            if (productions[i][j][0]==start )
            {
                res.push_back(nons[i]);
                break;
            }

        }
    }
    return res;
}


int table::find_ix(string prod)
{
    for (int i=0; i<nons.size(); i++)
    {
        if (nons[i] == prod)
            return i;
    }
    return -1;
}


bool table::check_eps(string prod)
{
    int ix= find_ix(prod);
    for (int i=0; i<productions[ix].size(); i++)
    {
        if (productions[ix][i][0]=="''")
            return true;
    }
    return false;
}


bool table::check_all_eps(vector<string> prod)
{
    int reach=0;
    for (int i=0; i<prod.size(); i++)
    {
        if (prod[i][0]!='\''&&check_eps(prod[i]))
            reach++;
    }
    if (reach==prod.size()) return true;
    return false;
}

void table::update_productions(string x)
{
    for (int i=0; i<productions.size(); i++)
    {
        for (int j=0; j<productions[i].size(); j++)
        {
            for (int k=0; k<productions[i][j].size(); k++)
            {
                if (productions[i][j][k]==x)
                {
                    productions[i][j].erase(productions[i][j].begin()+k);
                }
            }
        }
    }
}
void table::get_first()
{
    for (int i=0; i<nons.size(); i++)
    {
        vector<string> x;
        firsts.insert({nons[i],x});
    }

    for (int i=0; i<productions.size(); i++)
    {
        for (int j=0; j<productions[i].size(); j++)
        {
            for (int k=0; k<productions[i][j].size(); k++)
            {
                cout<<productions[i][j][k];
            }
        }
        cout<<endl;
    }

    for (int i=productions.size()-1; i>=0; i--)
    {
        vector<vector<string>> non_terminal = productions[i];
        for (int j=0; j<non_terminal.size(); j++)
        {

            if (non_terminal[j][0][0]== '\'' )    // if terminal
            {
                string terminal = non_terminal[j][0].substr(1,non_terminal[j][0].size()-2);
                auto it = firsts.find(nons[i]); //
                if (find(it->second.begin(), it->second.end(),terminal) == it->second.end())
                {
                    it->second.push_back(terminal);  //
                    cout<<"from terminal :"<<i<<j<<terminal<<endl;

                }
            }

            else
            {
                string take = non_terminal[j][0];
                auto it = firsts.find(take);
                int size= it->second.size();
                for (int k =0; k < size; k++)
                {
                    auto itt =firsts.find(nons[i]);
                    if (it->second[k]!=""&&find(itt->second.begin(), itt->second.end(),it->second[k]) == itt->second.end())
                    {
                        itt->second.push_back(it->second[k]);
                        cout<<"from non terminalqqqqqqqqqq"<<i<<j<<it->second[k]<<endl;
                    }
                }
            }
        }
    }

    for (int i=productions.size()-1; i>=0; i--)
    {
        vector<vector<string>> non_terminal = productions[i];
        for (int n=0; n<non_terminal.size(); n++)
        {

            if (non_terminal[n][0]== "''")       //if is epsilon
            {
                vector <string> takes = find_productions(nons[i]);
                for (int j=0; j<takes.size(); j++)
                {
                    int ix= find_ix(takes[j]);
                    vector<vector<string>> prod = productions[ix];
                    for (int k=0; k<prod.size(); k++)
                    {
                        bool reach=false;
                        if(prod[k][0]==nons[i])
                        {
                            int l=1;
                            while(l<prod[k].size()&&check_eps(prod[k][l-1]))
                            {
                                string take = prod[k][l++];
                                auto it = firsts.find(take); //
                                for (int k =0; k < it->second.size(); k++)
                                {
                                    auto itt =firsts.find(nons[ix]);
                                    if (it->second[k]!=""&&find(itt->second.begin(), itt->second.end(),it->second[k]) == itt->second.end())
                                    {
                                        itt->second.push_back(it->second[k]);
                                        cout<<"from non terminalllll"<<ix<<it->second[k]<<endl;
                                    }

                                }
                            }
                            if (l=prod[k].size() && check_all_eps(prod[k]))
                            {
                                auto it = firsts.find(nons[ix]); //
                                it->second.push_back("");  //
                                cout<<"from non terminalllll "<<ix<<""<<endl;
                                cout<<nons[ix]<<endl;

                            }
                        }
                    }

                }
            }

        }

    }

}





void table::call(int q)
{

    for (int i=0; i<productions.size(); i++)
    {
        for (int j=0; j<productions[i].size(); j++)
        {
            for (int k=0; k<productions[i][j].size(); k++)
            {
                if (productions[i][j][k]==nons[q])
                {
                    // if (productions[i][j][k][0]!='\''){    //non terminal found
                    if (k!=productions[i][j].size()-1)     //sth after it
                    {
                        if(productions[i][j][k+1][0]=='\'')
                        {
                            auto it = follows.find(productions[i][j][k]); //
                            string add=productions[i][j][k+1].substr(1,productions[i][j][k+1].size()-2);
                            if (add!=""&&find(it->second.begin(), it->second.end(),add) == it->second.end())
                            {
                                it->second.push_back(productions[i][j][k+1].substr(1,productions[i][j][k+1].size()-2));  //
                                cout<<"from terminal : "<<productions[i][j][k]<<">>"<<add<<endl;
                            }
                        }

                        else
                        {
                            auto it = firsts.find(productions[i][j][k+1]);
                            auto itt=follows.find(productions[i][j][k]);

                            for (int m =0; m < it->second.size(); m++)
                            {
                                if (it->second[m]!="" &&find(itt->second.begin(), itt->second.end(),it->second[m]) == itt->second.end())
                                {
                                    itt->second.push_back( it->second[m]);
                                    cout<<"from non terminal : "<<productions[i][j][k]<<">>"<<it->second[m]<<endl;
                                }
                            }
                        }
                    }
                    else if (productions[i][j][k]!=nons[i])       //nothing after it
                    {
                        auto it= follows.find(nons[i]);
                        cout<<"rule 3 :"<<nons[i]<<":"<<it->second.size()<<endl;
                        auto itt=follows.find(productions[i][j][k]);

                        for (int m =0; m < it->second.size(); m++)
                        {
                            if (it->second[m]!="" &&find(itt->second.begin(), itt->second.end(),it->second[m]) == itt->second.end())
                            {
                                itt->second.push_back( it->second[m]);  //
                                cout<<"from else terminal : "<<productions[i][j][k]<<">>"<<it->second[m]<<endl;
                            }
                        }
                    }
                    //}
                }
            }
        }

    }
}

void table::get_follow()
{
    for (int i=0; i<nons.size(); i++)
    {
        vector<string> x;
        follows.insert({nons[i],x});
    }
    cout<<"*\n*\n*\n"<<endl;
    auto it = follows.find(nons[0]);
    it->second.push_back("$");



    for (int q=0; q<nons.size(); q++)
    {
        call(q);


//if go to eps update productions
        if (check_eps(nons[q-1]))
        {
            update_productions(nons[q-1]);
            call(q);


        }   //q --> q-1
    }

}

