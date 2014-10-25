#ifndef _CUBE_EMITTER_H_
#define _CUBE_EMITTER_H_


#include "ParticleEffect_Interface.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "Random.h"


#include "gl/gl.h"
#include "gl/glu.h"


using namespace std;
class CubeEmitterEffect : public ParticleEffect_Interface
{
    glm::vec3 ExternalForce;
    float MaxRadius;
    float MinRadius;

    GLUquadricObj* quad;
    float Radius;
    float start_x, start_y, start_z;
    float pos_x, pos_y, pos_z;
    float vel_x, vel_y, vel_z;



    ParticleBuffer    e_ParticleBuffer;

    public:
        CubeEmitterEffect();
        void EmitParticles( Particle& particle);

        void InitParticleCube();
        void InitParticlePos(int i, int k, int j, int Index);
        void InitParticleVel(int i, int k, int j, int Index);
        void InitParticleAttribute(int i, int k, int j, int Index);

        void UpdateParticleCube(float dt);
        void DrawParticleCube();

        void update(bool toggle = false);
        void show(bool toggle = false);
        void Reset();


};


#endif
