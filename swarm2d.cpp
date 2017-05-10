#include "swarm2d.h"
#include <omp.h>
Swarm2d::Swarm2d()
{
//    lastTime = QTime::currentTime();
    distr = new std::uniform_real_distribution<float>(-0.5,0.5);
}

vector<float> Swarm2d::getModel(uint key)
{
    vector<float> _out;
    if(key>swarms.size()-1) return _out;
    for(Particle &p:swarms[key].swarm){
        _out.push_back(p.coord.x()+swarms[key].size*p.direction.x()/2);
        _out.push_back(p.coord.y()+swarms[key].size*p.direction.y()/2);
//        _out.push_back(0);
        _out.push_back(p.coord.x()-swarms[key].size*p.direction.x()/2);
        _out.push_back(p.coord.y()-swarms[key].size*p.direction.y()/2);
//        _out.push_back(0);
    }
    return _out;
}

void Swarm2d::addSwarm(int amount, QColor color, float size, float dR, float c1, float c2, float c3, float c4, float c5, float velMax, float velNorm,float c6, float c7, float visc,float angle)
{
    swarms.emplace_back(Swarm(lastId++,color,size,dR,c1,c2,c3,c4,c5,velMax,velNorm,c6,c7,visc,angle));
    Swarm* s = &swarms.back();
    for (int i=0;i<amount;++i){
        auto coord  =QVector3D((*distr)(rd)*width,(*distr)(rd)*height,0);
        auto vel  =QVector3D((*distr)(rd)*10,(*distr)(rd)*10,0);
        auto dir = QVector3D((*distr)(rd),(*distr)(rd),0);
        dir.normalize();
        vel *= vel.length()>velMax ? velMax/vel.length():1;
        s->swarm.emplace_back(Particle(Swarm::lastId++,s->id,coord,vel,dir));
    }
    Relation rel;
    rel["c1"]=1;
    rel["c2"]=1;
    rel["c3"]=1;
    rel["c4"]=1;
    rel["c6"]=1;
    rel["c7"]=1;
    relation.add(s->id,s->id,rel);
    rel["c1"]=0.15f;
    rel["c2"]=0.15f;
    rel["c3"]=5.15f;
    rel["c4"]=0.15f;
    rel["c6"]=0.15f;
    rel["c7"]=0.15f;
    for(uint i =0; i< swarms.size()-1;++i)
    {
        relation.add(s->id,swarms[i].id,rel);
        relation.add(swarms[i].id,s->id,rel);
    }
}

void Swarm2d::setSize(int height, int width)
{
    this->height = height;
    this->width = width;
}

void Swarm2d::getSize(int &height, int &width)
{
    height = this->height;
    width = this->width;
}

void Swarm2d::setRadius(float &rad)
{
     Radius = rad;
}

void Swarm2d::setSwarmAmount(uint key, int amount)
{
    if (key>=swarms.size()) return;
    if(swarms[key].swarm.size()<(uint)amount){
        Swarm* s = &swarms[key];
        for (int i=s->swarm.size();i<amount;++i){
            auto coord  =QVector3D((*distr)(rd)*width,(*distr)(rd)*height,0);
            auto vel  =QVector3D((*distr)(rd)*10,(*distr)(rd)*10,0);
            auto dir = QVector3D((*distr)(rd),(*distr)(rd),0);
            dir.normalize();
            vel *= vel.length()>s->velMax ? s->velMax/vel.length():1;
            s->swarm.emplace_back(Particle(Swarm::lastId++,s->id,coord,vel,dir));
        }
    } else if(swarms[key].swarm.size()>(uint)amount && amount>0){
        Swarm* s = &swarms[key];
        for (int i=s->swarm.size();i>amount;--i){
            s->swarm.pop_back();
        }
    }
}

void Swarm2d::move()
{
    for(Swarm &s:swarms){
        #pragma omp parallel for
        for(unsigned i=0;i<s.swarm.size();i++){
            Particle &p = s.swarm[i];
            p.move(1);

            float x = p.coord.x();
            float y = p.coord.y();
            float h = (float) height;
            float w = (float) width;
            if(x > w/2)
                x -= w;
            else if (x<-w/2)
                x+=w;

            if(y>h/2)
                y-=h;
            else if (y<-h/2)
                y+=h;

            p.coord.setX(x);
            p.coord.setY(y);
        }
    }
}

void Swarm2d::calcVel()
{
    map<int,vector<QVector3D>> newVelocities;
    for(Swarm &s:swarms){
        newVelocities[s.id].resize(s.swarm.size());
        #pragma omp parallel for shared(newVelocities)
        for(unsigned i=0;i<s.swarm.size();i++){
            Particle &p = s.swarm[i];
            vector<Particle*> local;
            vector<Particle*> fakeLocal;
            for(Swarm &s2:swarms){
                for(Particle &p2:s2.swarm){
                    if(p.id==p2.id && p.swarm==p2.swarm) continue;
                    if (p.coord.distanceToPoint(p2.coord)<=s.detectR){
                        local.push_back(&p2);
                    }
                }
            }

            QVector3D a; //aceleration
            QVector3D dir = p.direction; // direction change
            if (local.size()==0){
                a.setX((*distr)(rd));
                a.setY((*distr)(rd));
            }else{
                QVector3D vel;
                QVector3D coord;
                for (Particle* pa:local){
                    Relation rel = relation.get(s.id,pa->swarm);
                    a+=(p.coord - pa->coord)/(p.coord.distanceToPoint(pa->coord)*p.coord.distanceToPoint(pa->coord))*rel["c3"];
                    dir+=pa->direction;
                    vel += (pa->velocity-p.velocity)*rel["c2"];
                    coord += (pa->coord-p.coord)*rel["c1"];
                }
                a*=s.c3;
                vel/=local.size();
                coord/=local.size();
                a+= s.c1*coord+s.c2*(vel);
            }
            dir.normalize();
            p.direction += s.c6*dir + s.c7*p.velocity;
            p.direction.normalize();
            if((*distr)(rd)+0.5<s.c4){
                a.setX(a.x()+(*distr)(rd));
                a.setY(a.y()+(*distr)(rd));
            }
            QVector3D newVel = (p.velocity+a)*(1-s.viscos);  //new velocity
            if (newVel.length()>s.velMax){
                newVel *= s.velMax/newVel.length();
            }
            if(newVel.length()>0)
                newVel*=1-s.c5*(1-s.velNorm/newVel.length());
            else
                newVel.setX(s.velNorm) ;
            newVelocities[s.id][i]=newVel;
        }
    }
    for(Swarm &s:swarms){
        for(uint i=0;i<s.swarm.size();++i){
            s.swarm[i].velocity = newVelocities[s.id][i];
        }
    }
}

