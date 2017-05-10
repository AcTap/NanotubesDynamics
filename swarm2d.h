#ifndef SWARM2D_H
#define SWARM2D_H
#include "swarmmodel.h"
#include <random>

class Swarm2d: public SwarmModel
{
public:
    Swarm2d();
    vector<float> getModel(uint key);
    void addSwarm(int amount, QColor color, float size, float dR, float c1, float c2, float c3, float c4, float c5, float velMax, float velNorm, float c6, float c7, float visc,float angle);
    void setSize(int height,int width);
    void getSize(int& height,int& width);
    void setRadius(float &rad);
    void setSwarmAmount(uint key,int amount);
protected:
    std::random_device rd;
    std::uniform_real_distribution<float> *distr;
    void move();
    void calcVel();
    int height;
    int width;
    float Radius;
};

#endif // SWARM2D_H
