#ifndef _L_SPHERE_PARTICLE_EFFECT_H_
#define _L_SPHERE_PARTICLE_EFFECT_H_

#include "EG_Model.h"
#include "EG_Model/EG_ModelABS.h"
#include "pipeline.h"
#include "EG_RenderTechnique.h"
#include "EG_Technique_Shadow_Render.h"
#include "EG_Technique_TwoPass_RayCasting.h"
#include "sceneLoader.h"
#include "EG_ParticleEffect.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "CollisionDetection_HGrid.h"
//#include "pipeline.h"
#include "Random.h"
using namespace std;
class EG_SphereParticleEffect : public EG_ParticleEffect
{
    public:

    float m_maxRadius;
    float m_minRadius;
    float testRadius;

    int m_particlesCount;

    bool Ball2Ball_CollisionMode;
    bool first;

    h_ParticleBuffer    e_ParticleBuffer;

    CollisionDetection_HGrid    myHgrid;

    public:
        EG_SphereParticleEffect();

   //    void InitParticles(bool reset = false);
        void InitParticles(int count);
        void InitParticlePos(int i, int k, int j, int Index);
        void InitParticleVel(int i, int k, int j, int Index);
        void InitParticleAttribute(int i, int k, int j, int Index);


        void UpdateParticleEffect(float dt);
        void DrawParticleCube(pipeline &m_pipeline, unsigned int shaderID, meshLoader* mymesh);
        void DrawParticleCube(pipeline &m_pipeline, meshLoader* mymesh);

        void update(bool toggle = false);

   //     void show(pipeline &m_pipeline,  unsigned int shaderID , meshLoader* mymesh);
        void show(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderTypeID, meshLoader* mymesh);
        void render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderTypeID, meshLoader* mymesh);
        void render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID, EG_Model& model);

        void Reset();

        void ExamineParticleAttribute();


    float start_x, start_y, start_z;
    float pos_x, pos_y, pos_z;
    float vel_x, vel_y, vel_z;
    float Radius;
    GLUquadricObj* quad;
};


#endif
