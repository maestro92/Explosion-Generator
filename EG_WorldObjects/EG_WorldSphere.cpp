



#include "EG_WorldSphere.h"


WorldSphere::WorldSphere()
{
    obj_type = SPHERE;
}

WorldSphere::~WorldSphere()
{

}

void WorldSphere::setPosition(glm::vec3 pos)
{
    w_Position = pos;
}

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
