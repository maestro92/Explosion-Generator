
#include "EG_WorldObject.h"


WorldObject::WorldObject()
{

}


WorldObject::~WorldObject()
{

}




glm::vec3 WorldObject::getPosition()
{
    return position;
}

glm::vec3 WorldObject::getRotation()
{
    return rotation;
}

glm::vec3 WorldObject::getVelocity()
{
    return velocity;
}

void WorldObject::setPosition(glm::vec3 pos)
{
    w_Position = pos;
    position = pos;
}

void WorldObject::setPosition(float x, float y, float z)
{
    w_Position = glm::vec3(x, y, z);
    position = glm::vec3(x, y, z);
}

void WorldObject::updatePosition()
{
    position += velocity;
}

void WorldObject::setVelocity(glm::vec3 vel)
{
    w_Velocity = vel;
    velocity = vel;
}

void WorldObject::setVelocity(float x, float y, float z)
{
    w_Velocity = glm::vec3(x, y, z);
    velocity = glm::vec3(x, y, z);
}




void WorldObject::setRotation(glm::vec3 rot)
{
    w_Rotation = rot;
    rotation = rot;
}

void WorldObject::setRotation(float x, float y, float z)
{
    w_Rotation = glm::vec3(x, y, z);
    rotation = glm::vec3(x, y, z);
}

void WorldObject::setAngularVelocity(glm::vec3 ang_vel)
{
    w_AngularVelocity = ang_vel;
    angularVelocity = ang_vel;
}

void WorldObject::setAngularVelocity(float x, float y, float z)
{
    w_AngularVelocity = glm::vec3(x, y, z);
    angularVelocity = glm::vec3(x, y, z);
}

void WorldObject::updateRotation()
{
    w_Rotation += angularVelocity;
    rotation += angularVelocity;
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




void WorldObject::renderSingle( pipeline& m_pipeline,
                                EG_RenderTechnique* RenderTechnique,
                                int RenderPassID,
                                meshLoader* model)
{
    RenderTechnique->enableShader(RenderPassID);
    render(m_pipeline, RenderTechnique, RenderPassID, model);
    RenderTechnique->disableShader(RenderPassID);
}



void WorldObject::render(   pipeline& m_pipeline,
                            EG_RenderTechnique* RenderTechnique,
                            int RenderPassID,
                            meshLoader* model)
{
    RenderTechnique->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        m_pipeline.translate(position);
        RenderTechnique->loadUniformLocations(m_pipeline, RenderPassID);
        model->draw();
    m_pipeline.popMatrix();
    RenderTechnique->disableShader(RenderPassID);
}


void WorldObject::renderGroup(  pipeline& m_pipeline,
                                EG_RenderTechnique* RenderTechnique,
                                int RenderPassID,
                                meshLoader* model)
{
    m_pipeline.pushMatrix();
        m_pipeline.translate(position);
        RenderTechnique->loadUniformLocations(m_pipeline, RenderPassID);
        model->draw();
    m_pipeline.popMatrix();
}












/*
void WorldObject::render(pipeline &m_pipeline, EG_DeferredShading* RenderTechnique,
                        int RenderPassID, unsigned int textureId, meshLoader* model)
{
    RenderTechnique->enableShader(RENDER_PASS1);
    m_pipeline.pushMatrix();
        r_deferredShading->setStencilFlag(glm::vec3(1.0,0.0,0.0));

        if(isFirstPersonCamera)
            r_deferredShadingReflection.setEyeWorldPos(firstPersonPovCamera.getEyePoint());
        else
            r_deferredShadingReflection.setEyeWorldPos(thirdPersonPovCamera.m_eye);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
        r_deferredShading->setCubeMapTextureUnit(0);

        m_pipeline.translate(w_Position);

        r_deferredShading.loadUniformLocations(m_pipeline, RENDER_PASS1);
        model->draw();
    m_pipeline.popMatrix();
    RenderTechnique->disableShader(RENDER_PASS1);
}
*/

