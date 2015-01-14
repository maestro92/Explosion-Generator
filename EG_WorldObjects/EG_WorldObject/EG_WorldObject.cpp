
#include "EG_WorldObject.h"


WorldObject::WorldObject()
{

}


WorldObject::~WorldObject()
{

}


void WorldObject::setPosition(glm::vec3 pos)
{
    w_Position = pos;
}

void WorldObject::setVelocity(glm::vec3 vel)
{
    w_Velocity = vel;
}

void WorldObject::setRotation(glm::vec3 rot)
{
    w_Rotation = rot;
}

void WorldObject::setAngularVelocity(glm::vec3 ang_vel)
{
    w_AngularVelocity = ang_vel;
}


void WorldObject::setSize(float x, float y, float z)
{

}

void WorldObject::ReCalculateBoundingVolume()
{

}


bool WorldObject::CheckObjGroundCollision(float dt)
{

}


float WorldObject::getBoundingVolumeSize()
{

}
