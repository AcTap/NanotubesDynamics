#ifndef SWARMMODEL_H
#define SWARMMODEL_H
#include <vector>
#include <QTime>
#include <QColor>
#include "particle.h"
#include "swarm.h"
#include "swarmrelation.h"

using std::vector;
class SwarmModel
{
public:
    SwarmModel();
    virtual vector<float> getModel(uint key)=0;
    void iterate();
    uint swarmCount();
    Swarm* getSwarm(uint key);
    virtual void addSwarm(int amount,QColor color, float size,float dR, float c1, float c2, float c3, float c4, float c5, float velMax, float velNorm, float c6,float c7,float visc,float angle)=0;
    virtual void setSwarmAmount(uint key,int amount)=0;
    void hardReset(){
        lastId=0;
        Swarm::lastId=0;
        swarms.clear();
        relation.reset();
    }
    virtual void setSize(int,int)=0;
    virtual void getSize(int&,int&)=0;
    virtual void setRadius(float &)=0;
    bool continuos;
    SwarmRelation relation;
protected:
    vector<Swarm> swarms;    
    virtual void move()=0;
    virtual void calcVel()=0;
    int lastId;
//    QTime lastTime;
};

#endif // SWARMMODEL_H
