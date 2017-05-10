#include "swarm.h"

Swarm::Swarm()
{

}

Swarm::Swarm(int id, QColor color, float size, float dR, float c1, float c2, float c3, float c4, float c5, float velMax, float velNorm, float c6, float c7, float visc, float angle):
    id(id),color(color),size(size),detectR(dR),c1(c1),c2(c2),c3(c3),c4(c4),c5(c5),velMax(velMax),velNorm(velNorm),c6(c6),c7(c7),viscos(visc),angle(angle)
{

}

int Swarm::lastId = 0;
