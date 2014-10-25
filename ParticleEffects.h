#ifndef PARTICLE_EFFECT_H
#define PARTICLE_EFFECT_H

/*
abstract class vs interfaces in java
http://www.javaworld.com/article/2077421/learn-java/abstract-classes-vs-interfaces.html



*/


using namespace std;

class ParticleEffect
{
public:
    virtual void update() = 0;
    virtual void show() = 0;

};


#endif // PARTICLE_EFFECT_H
