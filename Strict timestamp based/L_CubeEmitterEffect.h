#ifndef _L_CUBE_EMITTER_H_
#define _L_CUBE_EMITTER_H_


#include "ParticleEffect_Interface.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "CollisionDetection_HGrid.h"

#include "Random.h"
using namespace std;
class L_CubeEmitterEffect : public ParticleEffect_Interface
{
    public:

    glm::vec3 ExternalForce_neg;
    glm::vec3 ExternalForce_half_neg;
    glm::vec3 ExternalForce_pos;
    glm::vec3 ExternalForce_half_pos;
    float MaxRadius;
    float MinRadius;

    bool Ball2Ball_CollisionMode;
    bool first;

    h_ParticleBuffer    e_ParticleBuffer;

    CollisionDetection_HGrid    myHgrid;

    public:
        L_CubeEmitterEffect();
        void EmitParticles( Particle& particle);

        void InitParticleCube(bool reset = false);
        void InitParticlePos(int i, int k, int j, int Index);
        void InitParticleVel(int i, int k, int j, int Index);
        void InitParticleAttribute(int i, int k, int j, int Index);
        void InitCollisionDetection_HGrid();


        void UpdateParticleCube(float dt);
        void DrawParticleCube();

        void update(bool toggle = false);
        void show(bool toggle = false);
        void Reset();

        void ExamineParticleAttribute();
        void UpdateNextTimeStamp(float dt);
};


#endif
