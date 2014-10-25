#ifndef _L_CUBE_SPHERE_PARTICLE_EFFECT_H_
#define _L_CUBE_SPHERE_PARTICLE_EFFECT_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>


#include "WorldBox.h"
#include "WorldSphere.h"

#include "pipeline.h"
#include "sceneLoader.h"
#include "L_ParticleEffect_Interface.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "CollisionDetection_HGrid_new.h"
//#include "pipeline.h"
#include "Random.h"
using namespace std;

typedef std::vector<WorldObject*> w_ParticleBuffer;


class L_Cube_Sphere_ParticleEffect : public L_ParticleEffect_Interface
{
    public:


    float MaxX, MinX;
    float MaxY, MinY;
    float MaxZ, MinZ;

    float testRadius;


    bool CollisionMode;
    bool first;


    w_ParticleBuffer    e_ParticleBuffer;
    CollisionDetection_HGrid_NEW    myHgrid;

    public:
        L_Cube_Sphere_ParticleEffect();

        void InitParticle(bool reset = false);
        void InitParticle(int flag, float angle, bool reset = false);
        void InitParticlePos(int i, int k, int j, int Index);
        void InitParticleVel(int i, int k, int j, int Index);
        void InitParticleAttribute(int i, int k, int j, int Index);
        void InitParticleAttribute(int i, int k, int j, int Index, int flag, float angle);

        void UpdateParticleEffect(float dt);
        void DrawParticles();
        void DrawParticles(pipeline &m_pipeline, unsigned int shaderID, meshLoader* mymesh);
        void DrawMyHgridFrames();

        void update(bool toggle = false);
        void show(bool toggle = false);
        void show(pipeline &m_pipeline,  unsigned int shaderID , meshLoader* mymesh);
        void Reset();
        void Reset(int flag);
        void Reset(int flag, float angle);
        void ExamineParticleAttribute();

};


#endif
