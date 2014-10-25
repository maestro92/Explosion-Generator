#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_
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

class ParticleEmitter
{
    public:
  //      virtual ~ParticleEmitter() {}
        ParticleBuffer    e_ParticleBuffer;
        virtual void EmitParticles( Particle& particle) = 0;
        //void EmitParticles( Particle& particle);





};


#endif
