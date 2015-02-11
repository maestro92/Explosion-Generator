
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

