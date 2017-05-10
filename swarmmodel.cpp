#include "swarmmodel.h"

SwarmModel::SwarmModel():continuos(false),lastId(0)
{
}

void SwarmModel::iterate()
{
    calcVel();
    move();
}

uint SwarmModel::swarmCount()
{
    return swarms.size();
}

Swarm* SwarmModel::getSwarm(uint key)
{
    return &swarms.at(key);
}
