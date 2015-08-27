
#include "EG_WorldObject.h"


WorldObject::WorldObject()
{
    m_position = glm::vec3(0.0, 0.0, 0.0);
    m_velocity = glm::vec3(0.0, 0.0, 0.0);
    m_scale = glm::vec3(1.0, 1.0, 1.0);

    m_orientation = glm::toQuat(glm::mat4(1.0));

    defaultModel = NULL;
}


WorldObject::~WorldObject()
{

}


void WorldObject::setModel(EG_ModelABS* model)
{
    defaultModel = model;
}




glm::vec3 WorldObject::getPosition()
{
    return m_position;
}

/*
glm::vec3 WorldObject::getRotation()
{
    return rotation;
}
*/
glm::quat WorldObject::getRotation()
{
    return m_rotation;
}

glm::vec3 WorldObject::getVelocity()
{
    return m_velocity;
}





void WorldObject::setPosition(glm::vec3 pos)
{
    w_Position = pos;
    m_position = pos;
}

void WorldObject::setPosition(float x, float y, float z)
{
    w_Position = glm::vec3(x, y, z);
    m_position = glm::vec3(x, y, z);
}


void WorldObject::updatePosition(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
    m_position += xAxis * m_velocity.x;
    m_position += yAxis * m_velocity.y;
    m_position += zAxis * m_velocity.z;
}



void WorldObject::updatePosition()
{
    m_position += m_velocity;
}

void WorldObject::setVelocity(glm::vec3 vel)
{
    w_Velocity = vel;
    m_velocity = vel;
}

void WorldObject::setVelocity(float x, float y, float z)
{
    w_Velocity = glm::vec3(x, y, z);
    m_velocity = glm::vec3(x, y, z);
}

void WorldObject::setAngularVelocityX(float x)
{
    m_angularVelocity.x = x;
}

void WorldObject::setAngularVelocityY(float y)
{
    m_angularVelocity.y = y;
}

void WorldObject::setAngularVelocityZ(float z)
{
    m_angularVelocity.z = z;
}



void WorldObject::setRotation(glm::vec3 rot)
{
    m_rotation = glm::quat(glm::mat4(1.0f));
    setRotation(rot, glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f));
}

void WorldObject::setAngularVelocity(glm::vec3 ang_vel)
{
    m_angularVelocity = ang_vel;
}

void WorldObject::setAngularVelocity(float x, float y, float z)
{
    m_angularVelocity = glm::vec3(x, y, z);
}

void WorldObject::updateRotation()
{
    updateRotation(glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f));
}

void WorldObject::updateRotation(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
    setRotation(m_angularVelocity, xAxis, yAxis, zAxis);
}

void WorldObject::setRotation(glm::vec3 rot, glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
    /// yaw
    glm::quat yawChange = glm::angleAxis(rot.y, yAxis);
    m_rotation = yawChange * m_rotation;

    /// pitch
    glm::quat pitchChange = glm::angleAxis(rot.x, xAxis);
    m_rotation = pitchChange * m_rotation;

    /// roll
    glm::quat rollChange = glm::angleAxis(rot.z, zAxis);
    m_rotation = rollChange * m_rotation;

 //   m_rotation = glm::normalize(m_rotation);

}

/*
void WorldObject::updateRotation(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
    setRotation(angularVelocity, glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);

    /// yaw
    glm::quat yawChange = glm::angleAxis(angularVelocity.y, yAxis);
    rotation = yawChange * rotation;

    /// pitch
    glm::quat pitchChange = glm::angleAxis(angularVelocity.x, xAxis);
    rotation = pitchChange * rotation;

    /// roll
    glm::quat rollChange = glm::angleAxis(angularVelocity.z, zAxis);
    rotation = rollChange * rotation;
}
*/
void WorldObject::setOrientation(float angle, glm::vec3 axis)
{
    m_orientation = glm::angleAxis(angle, axis.x, axis.y, axis.z);
}


void WorldObject::setSize(float x, float y, float z)
{

}


void WorldObject::setScale(glm::vec3 scale)
{
    m_scale = scale;
}

void WorldObject::setScale(float x, float y, float z)
{
    m_scale = glm::vec3(x,y,z);
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
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                meshLoader* model)
{
    Renderer->enableShader(RenderPassID);
    render(m_pipeline, Renderer, RenderPassID, model);
    Renderer->disableShader(RenderPassID);
}



void WorldObject::render(   pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID,
                            meshLoader* model)
{
    Renderer->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
        m_pipeline.scale(m_scale);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        model->draw();
    m_pipeline.popMatrix();
    Renderer->disableShader(RenderPassID);
}


void WorldObject::renderGroup(  pipeline& m_pipeline,
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                meshLoader* model)
{
    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
        m_pipeline.scale(m_scale);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        model->draw();
    m_pipeline.popMatrix();
}



void WorldObject::renderGroup( pipeline& m_pipeline,
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                EG_Model* model)
{
    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
        m_pipeline.loadMatrix(glm::toMat4(m_orientation));
        m_pipeline.scale(m_scale);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        model->render();
    m_pipeline.popMatrix();
}



void WorldObject::renderSingle( pipeline& m_pipeline,
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                EG_Model* model)
{
    Renderer->enableShader(RenderPassID);
    renderGroup(m_pipeline, Renderer, RenderPassID, model);
    Renderer->disableShader(RenderPassID);
}





void WorldObject::renderSingle( pipeline& m_pipeline,
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                EG_ModelABS* model)
{
    Renderer->enableShader(RenderPassID);
    renderGroup(m_pipeline, Renderer, RenderPassID, model);
    Renderer->disableShader(RenderPassID);
}

void WorldObject::renderGroup( pipeline& m_pipeline,
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                EG_ModelABS* model)
{
    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
        m_pipeline.loadMatrix(glm::toMat4(m_orientation));
        m_pipeline.scale(m_scale);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        model->render();
    m_pipeline.popMatrix();
}

void WorldObject::render(EG_Renderer* Renderer, int pass)
{
    if(defaultModel != NULL)
    {

    }
}


/*
void WorldObject::renderSingle( pipeline& m_pipeline,
                                EG_Renderer* Renderer,
                                int RenderPassID,
                                EG_Model* model)
{
    Renderer->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
  //      m_pipeline.rotateX(270);
        m_pipeline.loadMatrix(glm::toMat4(m_orientation));
        m_pipeline.scale(m_scale);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        model->render();
    m_pipeline.popMatrix();
    Renderer->disableShader(RenderPassID);
}
*/

/*
void WorldObject::render(pipeline &m_pipeline, EG_DeferredShading* Renderer,
                        int RenderPassID, unsigned int textureId, meshLoader* model)
{
    Renderer->enableShader(RENDER_PASS1);
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
    Renderer->disableShader(RENDER_PASS1);
}
*/

