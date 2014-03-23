#ifndef _CUBE_EMITTER_H_
#define _CUBE_EMITTER_H_


#include "ParticleEmitter.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "Random.h"
using namespace std;
class CubeEmitter : ParticleEmitter
{
    glm::vec3 ExternalForce;
    float MaxRadius;
    float MinRadius;



    public:
        CubeEmitter();
        void EmitParticles( Particle& particle);

        void InitParticleCube();
        void InitParticlePos(int i, int k, int k, int Index);
        void InitParticleVel(int i, int k, int k, int Index);
        void InitParticleAttribute(int i, int k, int k, int Index);

        void Reset();
        void UpdateParticleCube(float dt);
        void DrawParticleCube();


};


#endif
