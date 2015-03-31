#ifndef WORLD_SPHERE
#define WORLD_SPHERE

#include "EG_WorldObject.h"

class WorldSphere : public WorldObject
{
    public:
        // constructor
        WorldSphere();


        // Destructor
		virtual ~WorldSphere();

        virtual void init();

   //     virtual void setPosition(glm::vec3 pos);
        virtual void setAngularVelocity(glm::vec3 ang_vel);
        virtual void setSize(float x, float y, float z);

        virtual bool CheckObjGroundCollision(float dt);

        virtual float getBoundingVolumeSize();
    /*
        virtual void render(pipeline &m_pipeline,
                            EG_DeferredShading* RenderTechnique,
                            int RenderPassID,
                            unsigned int textureId,
                            meshLoader* model);
*/
        float w_radius;

};


#endif
