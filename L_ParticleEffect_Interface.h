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


#include "glm/glm.hpp"



static glm::vec3 ExternalForce_neg = vec3(0,-9.8,0);
static glm::vec3 ExternalForce_half_neg = vec3(0,9.8,0);
static glm::vec3 ExternalForce_pos = vec3(0,-4.9,0);
static glm::vec3 ExternalForce_half_pos = vec3(0,4.9,0);

using namespace std;

class L_ParticleEffect_Interface
{
    public:

        /// the constructor of the derived class gets called after the constructor of the base class
   //     L_ParticleEffect_Interface();
   //     ~L_ParticleEffect_Interface();
  //      virtual ~ParticleEmitter() {}
        virtual void update(bool toggle = false) = 0;
        virtual void show(bool toggle = false) = 0;
 //       virtual void Reset() = 0;

//        virtual void InitParticle(bool reset = false) = 0;
//        virtual void InitParticle(int flag, bool reset = false) = 0;
        virtual void InitParticlePos(int i, int k, int j, int Index) = 0;
        virtual void InitParticleVel(int i, int k, int j, int Index) = 0;
        virtual void InitParticleAttribute(int i, int k, int j, int Index) = 0;

/*
    protected:
        glm::vec3 ExternalForce_neg;
        glm::vec3 ExternalForce_half_neg;
        glm::vec3 ExternalForce_pos;
        glm::vec3 ExternalForce_half_pos;
*/
};


#endif
