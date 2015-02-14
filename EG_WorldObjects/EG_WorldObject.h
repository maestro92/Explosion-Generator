#ifndef WORLD_OBJECT
#define WORLD_OBJECT


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <cstdlib>
#include <stdint.h>
#include <vector>
#include "Particle.h"

#include "EG_DeferredShading.h"
#include "sceneLoader.h"

#define SPHERE  0
#define BOX     1


static glm::vec3 ExternalForce_neg1 = vec3(0,-9.8,0);
static glm::vec3 ExternalForce_half_neg1 = vec3(0,9.8,0);
static glm::vec3 ExternalForce_pos1 = vec3(0,-4.9,0);
static glm::vec3 ExternalForce_half_pos1 = vec3(0,4.9,0);


using namespace glm;
class WorldObject
{
    public:

        WorldObject* pPrevObj;
        WorldObject* pNextObj;      // embedded link to next hgrid particle

       // static meshLoader* model;

        /// Translational
        float   w_mass;



        vec3    w_Position;
        vec3    w_Velocity;

        /// Rotational
        glm::mat3 w_InertiaTensor;
        glm::mat3 w_InverseInertiaTensor;

        vec3    w_Rotation;
        vec3    w_AngularVelocity;
        vec3    w_AngularAcceleration;

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


        vec3    position;
        vec3    velocity;
        vec3    rotation;
        vec3    angularVelocity;

        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getVelocity();


        void setPosition(glm::vec3 pos);
        void setPosition(float x, float y, float z);
        void setVelocity(glm::vec3 vel);
        void setVelocity(float x, float y, float z);
        void updatePosition();

        void setRotation(glm::vec3 rot);
        void setRotation(float x, float y, float z);
        void setAngularVelocity(glm::vec3 ang_vel);
        void setAngularVelocity(float x, float y, float z);
        void updateRotation();




        virtual void ReCalculateBoundingVolume();
        virtual bool CheckObjGroundCollision(float dt);
        virtual float getBoundingVolumeSize();
        // for sphere
        virtual void setSize(float x, float y, float z);


        virtual void renderSingle(pipeline& m_pipeline,
                            EG_RenderTechnique* RenderTechnique,
                            int RenderPassID,
                            meshLoader* model);

        virtual void render(pipeline& m_pipeline,
                            EG_RenderTechnique* RenderTechnique,
                            int RenderPassID,
                            meshLoader* model);

        virtual void renderGroup(  pipeline& m_pipeline,
                                    EG_RenderTechnique* RenderTechnique,
                                    int RenderPassID,
                                    meshLoader* model);

        // destructor
        /*
        virtual void render(pipeline& m_pipeline,
                            EG_DeferredShading* RenderTechnique,
                            int RenderPassID,
                            unsigned int textureId,
                            meshLoader* model);
*/
};




#endif // WORLD_OBJECTS
