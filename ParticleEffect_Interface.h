#ifndef _PARTICLE_EFFECT_H_
#define _PARTICLE_EFFECT_H_
#include "Particle.h"
#include <cstdlib>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>





using namespace std;

class ParticleEffect_Interface
{
    public:
  //      virtual ~ParticleEmitter() {}


        virtual void update(bool toggle = false) = 0;
        virtual void show(bool toggle = false) = 0;
        virtual void Reset() = 0;
};


#endif
