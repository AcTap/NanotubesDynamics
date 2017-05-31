#ifndef SWARM_H
#define SWARM_H
#include <vector>
#include <QColor>
#include "particle.h"
using std::vector;

class Swarm
{
public:
    Swarm();
    Swarm(int id, QColor color, float size, float dR, float c1, float c2, float c3, float c4, float c5, float velMax, float velNorm, float c6, float c7, float visc, float angle);
    int id;
    QColor color;
    vector<Particle> swarm;
    float size;
    float detectR;
    float c1;
    float c2;
    float c3;
    float c4;
    float c5;
    float velMax;
    float velNorm;
    float c6;
    float c7;
    float viscos;
    float angle;
    static int lastId;
    void setAmount(int);
};

#endif // SWARM_H
