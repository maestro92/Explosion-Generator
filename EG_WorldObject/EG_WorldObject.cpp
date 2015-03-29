
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

/*
glm::vec3 WorldObject::getRotation()
{
    return rotation;
}
*/
glm::quat WorldObject::getRotation()
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

void WorldObject::updatePosition(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
    position += xAxis * velocity.x;
    position += yAxis * velocity.y;
    position += zAxis * velocity.z;
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

void WorldObject::setAngularVelocityX(float x)
{
    angularVelocity.x = x;
}

void WorldObject::setAngularVelocityY(float y)
{
    angularVelocity.y = y;
}

void WorldObject::setAngularVelocityZ(float z)
{
    angularVelocity.z = z;
}



void WorldObject::setRotation(glm::vec3 rot)
{
    rotation = glm::quat(glm::mat4(1.0f));
    setRotation(rot, glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f));
}

void WorldObject::setAngularVelocity(glm::vec3 ang_vel)
{
    angularVelocity = ang_vel;
}

void WorldObject::setAngularVelocity(float x, float y, float z)
{
    angularVelocity = glm::vec3(x, y, z);
}

void WorldObject::updateRotation()
{
    updateRotation(glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f));
}

void WorldObject::updateRotation(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
    setRotation(angularVelocity, xAxis, yAxis, zAxis);
}

void WorldObject::setRotation(glm::vec3 rot, glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
    /// yaw
    glm::quat yawChange = glm::angleAxis(rot.y, yAxis);
    rotation = yawChange * rotation;

    /// pitch
    glm::quat pitchChange = glm::angleAxis(rot.x, xAxis);
    rotation = pitchChange * rotation;

    /// roll
    glm::quat rollChange = glm::angleAxis(rot.z, zAxis);
    rotation = rollChange * rotation;

 //   rotation = glm::normalize(rotation);

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



void WorldObject::renderSingle( pipeline& m_pipeline,
                                EG_RenderTechnique* RenderTechnique,
                                int RenderPassID,
                                EG_Model* model)
{
    RenderTechnique->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        m_pipeline.translate(position);
        RenderTechnique->loadUniformLocations(m_pipeline, RenderPassID);
        model->render();
    m_pipeline.popMatrix();
    RenderTechnique->disableShader(RenderPassID);
}



/*
/// changes the way the model is facing
void EG_ThirdPersonPovCamera::updateCharacterOrientation(float pitchChange, float yawChange, float rollChange)
{
    c_eulerOrient.x += pitchChange;
    c_eulerOrient.y += yawChange;
    c_eulerOrient.z += rollChange;

    if (c_eulerOrient.x > 360.0f)
        c_eulerOrient.x -= 360.0f;

    if (c_eulerOrient.x < -360.0f)
        c_eulerOrient.x += 360.0f;

    if (c_eulerOrient.y > 360.0f)
        c_eulerOrient.y -= 360.0f;

    if (c_eulerOrient.y < -360.0f)
        c_eulerOrient.y += 360.0f;

    if (c_eulerOrient.z > 360.0f)
        c_eulerOrient.z -= 360.0f;

    if (c_eulerOrient.z < -360.0f)
        c_eulerOrient.z += 360.0f;
}


/// changes the way the model is facing
void EG_ThirdPersonPovCamera::updateCharacterRotation(float pitchChange, float yawChange, float rollChange)
{
    c_eulerRotate.x += pitchChange;
    c_eulerRotate.y += yawChange;
    c_eulerRotate.z += rollChange;

    if (c_eulerRotate.x > 360.0f)
        c_eulerRotate.x -= 360.0f;

    if (c_eulerRotate.x < -360.0f)
        c_eulerRotate.x += 360.0f;

    if (c_eulerRotate.y > 360.0f)
        c_eulerRotate.y -= 360.0f;

    if (c_eulerRotate.y < -360.0f)
        c_eulerRotate.y += 360.0f;

    if (c_eulerRotate.z > 360.0f)
        c_eulerRotate.z -= 360.0f;

    if (c_eulerRotate.z < -360.0f)
        c_eulerRotate.z += 360.0f;
}
*/


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

