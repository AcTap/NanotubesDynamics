#include "particle.h"

Particle::Particle()
{

}

Particle::Particle(int id, int swarm, QVector3D coord, QVector3D velocity, QVector3D direction):coord(coord),velocity(velocity),direction(direction),id(id),swarm(swarm)
{

}


