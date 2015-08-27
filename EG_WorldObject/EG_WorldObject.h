#ifndef WORLD_OBJECT
#define WORLD_OBJECT

#include "define.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <cstdlib>
#include <stdint.h>
#include <vector>
#include "Particle.h"

#include "EG_Model.h"
#include "EG_ModelABS.h"
#include "EG_DeferredShading.h"
#include "sceneLoader.h"
#include "pipeline.h"
#include "EG_Renderer.h"

#define SPHERE  0
#define BOX     1

/*
const glm::vec3 WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const glm::vec3 WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_ZAXIS(0.0f, 0.0f, 1.0f);
*/
static glm::vec3 ExternalForce_neg1 = vec3(0,-9.8,0);
static glm::vec3 ExternalForce_half_neg1 = vec3(0,9.8,0);
static glm::vec3 ExternalForce_pos1 = vec3(0,-4.9,0);
static glm::vec3 ExternalForce_half_pos1 = vec3(0,4.9,0);



using namespace std;
class WorldObject
{
    public:

        WorldObject* pPrevObj;
        WorldObject* pNextObj;      // embedded link to next hgrid particle

       // static meshLoader* model;

        /// Translational
        float   w_mass;



        glm::vec3    w_Position;
        glm::vec3    w_Velocity;

        /// Rotational
        glm::mat3 w_InertiaTensor;
        glm::mat3 w_InverseInertiaTensor;

        glm::vec3    w_Rotation;
        glm::vec3    w_AngularVelocity;
        glm::vec3    w_AngularAcceleration;

        vec4    w_Color;
        int     obj_type;
        int     w_id;
        hash_t  w_hash;

        float   w_Age;
        float   w_LifeTime;
        int     state;

        // default constructor
        WorldObject();
        virtual ~WorldObject();

        EG_ModelABS* defaultModel;



        vec3    m_position;
        vec3    m_velocity;
        glm::quat m_orientation;
        glm::quat m_rotation;
        vec3    m_angularVelocity;
        vec3    m_scale;

        glm::vec3 getPosition();
        glm::quat getRotation();
        glm::vec3 getScale();
        glm::vec3 getVelocity();


        void setScale(glm::vec3 scale);
        void setScale(float x, float y, float z);

        void setPosition(glm::vec3 pos);
        void setPosition(float x, float y, float z);

        void setVelocity(glm::vec3 vel);
        void setVelocity(float x, float y, float z);

        void updatePosition();
        void updatePosition(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);

        void setRotation(glm::vec3 rot);
        void setRotation(glm::vec3 rot, glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);
    //    void setRotation(float x, float y, float z);
        void setAngularVelocityX(float x);
        void setAngularVelocityY(float y);
        void setAngularVelocityZ(float z);
        void setAngularVelocity(glm::vec3 ang_vel);
        void setAngularVelocity(float x, float y, float z);
        void updateRotation();
        void updateRotation(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);

        void setOrientation(float angle, glm::vec3 axis);

        virtual void ReCalculateBoundingVolume();
        virtual bool CheckObjGroundCollision(float dt);
        virtual float getBoundingVolumeSize();
        // for sphere
        virtual void setSize(float x, float y, float z);
        void setModel(EG_ModelABS* model);


        virtual void render(EG_Renderer* Renderer, int pass);

        virtual void renderSingle(pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID,
                            meshLoader* model);

        virtual void render(pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID,
                            meshLoader* model);

        virtual void renderGroup(   pipeline& m_pipeline,
                                    EG_Renderer* Renderer,
                                    int RenderPassID,
                                    meshLoader* model);


        void renderGroup(   pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID,
                            EG_Model* model);

        void renderSingle(  pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID,
                            EG_Model* model);



        void renderSingle( pipeline& m_pipeline,
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                EG_ModelABS* model);


        void renderGroup( pipeline& m_pipeline,
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                EG_ModelABS* model);

        // destructor
        /*
        virtual void render(pipeline& m_pipeline,
                            EG_DeferredShading* Renderer,
                            int RenderPassID,
                            unsigned int textureId,
                            meshLoader* model);
*/
};




#endif // WORLD_OBJECTS
