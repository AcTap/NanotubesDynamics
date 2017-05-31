#ifndef SWARMRELATION_H
#define SWARMRELATION_H
#include <map>
#include <string>
using std::map;
using std::pair;
using std::string;
typedef pair<int,int> bikey;
typedef map<string,float> Relation;

class SwarmRelation
{
public:
    SwarmRelation();
    void add(int swarmA,int swarmB, Relation relation){
        table[bikey(swarmA,swarmB)]=relation;
    }

    Relation get(int swarmA,int swarmB)
    {
        if(table.find(bikey(swarmA,swarmB))!=table.end()){
            return table.at(bikey(swarmA,swarmB));
        }
        else{
            return Relation();
        }
    }

    void set(int swarmA,int swarmB, Relation relation){
        table[bikey(swarmA,swarmB)]=relation;
    }

    void reset(){
        table.clear();
    }

private:
    map<bikey,Relation> table;
};

#endif // SWARMRELATION_H
