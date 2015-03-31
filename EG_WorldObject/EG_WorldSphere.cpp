#include "EG_WorldSphere.h"


WorldSphere::WorldSphere()
{
    obj_type = SPHERE;
}

WorldSphere::~WorldSphere()
{

}


void WorldSphere::init()
{

}
/*
void WorldSphere::setPosition(glm::vec3 pos)
{
    w_Position = pos;
    m_position = pos;
}
*/
void WorldSphere::setSize(float x, float y, float z)
{

}

void WorldSphere::setAngularVelocity(glm::vec3 ang_vel)
{
    w_AngularVelocity = ang_vel;
}

bool WorldSphere::CheckObjGroundCollision(float dt)
{

    return true;
}


float WorldSphere::getBoundingVolumeSize()
{

}

/*
void WorldSphere::render(pipeline &m_pipeline,
                          EG_DeferredShading* RenderTechnique,
                          int RenderPassID,
                          unsigned int textureId,
                          meshLoader* model)
{
    WorldObject::render(m_pipeline, RenderTechnique, RenderPassID, textureId, model);
}
*/
