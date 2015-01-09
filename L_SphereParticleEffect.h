#ifndef _L_SPHERE_PARTICLE_EFFECT_H_
#define _L_SPHERE_PARTICLE_EFFECT_H_

#include "pipeline.h"
#include "EG_RenderTechnique.h"
#include "EG_Technique_Shadow_Render.h"
#include "EG_Technique_TwoPass_RayCasting.h"
#include "sceneLoader.h"
#include "L_ParticleEffect_Interface.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "CollisionDetection_HGrid.h"
//#include "pipeline.h"
#include "Random.h"
using namespace std;
class L_SphereParticleEffect : public L_ParticleEffect_Interface
{
    public:

    float MaxRadius;
    float MinRadius;
    float testRadius;


    bool Ball2Ball_CollisionMode;
    bool first;

    h_ParticleBuffer    e_ParticleBuffer;

    CollisionDetection_HGrid    myHgrid;

    public:
        L_SphereParticleEffect();

        void InitParticle(bool reset = false);
        void InitParticlePos(int i, int k, int j, int Index);
        void InitParticleVel(int i, int k, int j, int Index);
        void InitParticleAttribute(int i, int k, int j, int Index);


        void UpdateParticleEffect(float dt);
        void DrawParticleCube(pipeline &m_pipeline, unsigned int shaderID, meshLoader* mymesh);
        void DrawParticleCube(pipeline &m_pipeline, meshLoader* mymesh);

        void update(bool toggle = false);
        void show(bool toggle = false);
     //   void show(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderTypeID, int RenderPassID, meshLoader* mymesh);
        void show(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderTypeID, meshLoader* mymesh);
        void show(pipeline &m_pipeline,  unsigned int shaderID , meshLoader* mymesh);

        void Reset();

        void ExamineParticleAttribute();


    float start_x, start_y, start_z;
    float pos_x, pos_y, pos_z;
    float vel_x, vel_y, vel_z;
    float Radius;
    GLUquadricObj* quad;
};


#endif
