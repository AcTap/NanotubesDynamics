#ifndef PARTICLE_H
#define PARTICLE_H
#include <QVector3D>

class Particle
{
public:
    Particle(int id, int swarm, QVector3D coord, QVector3D velocity, QVector3D direction):coord(coord),velocity(velocity),direction(direction),id(id),swarm(swarm){}
    Particle(){}
    QVector3D coord;
    QVector3D velocity;
    QVector3D direction;
    int id;
    int swarm;
    void move(float dt){ coord+=velocity*dt;}
};

#endif // PARTICLE_H
