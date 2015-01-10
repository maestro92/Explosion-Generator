
#include "EG_DeferredShadingDirectionalLightPass.h"


EG_DeferredShadingDirectionalLightPass::EG_DeferredShadingDirectionalLightPass()
{
}

EG_DeferredShadingDirectionalLightPass::~EG_DeferredShadingDirectionalLightPass()
{
}

void EG_DeferredShadingDirectionalLightPass::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);

    progShaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_ModelMatrixBased.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_ModelMatrixBased.fs");

    m_dirLightLocation.color                = GetUniformLocation( progShaders[RENDER_PASS1], "gDirectionalLight.base.color");
    m_dirLightLocation.ambientIntensity     = GetUniformLocation( progShaders[RENDER_PASS1], "gDirectionalLight.base.ambientIntensity");
    m_dirLightLocation.diffuseIntensity     = GetUniformLocation( progShaders[RENDER_PASS1], "gDirectionalLight.base.diffuseIntensity");
    m_dirLightLocation.direction            = GetUniformLocation( progShaders[RENDER_PASS1], "gDirectionalLight.direction");


    l_modelViewProjectionMatrix_UniLoc_ =       GetUniformLocation(progShaders[RENDER_PASS1], "l_modelViewProjectionMatrix");
    lightPosition_ModelViewMatrix_UniLoc_ =     GetUniformLocation(progShaders[RENDER_PASS1], "lightPosition_ModelViewMatrix");
    lightPosition_ObjectSpace_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS1], "lightPosition_ObjectSpace");
    screenSize_UniLoc_                =         GetUniformLocation(progShaders[RENDER_PASS1], "gScreenSize");


    positionMap_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS1], "positionMap");
    colorMap_UniLoc_    =         GetUniformLocation(progShaders[RENDER_PASS1], "colorMap");
    normalMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS1], "normalMap");
    shadowMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS1], "shadowMap");

/*
	if (m_dirLightLocation.ambientIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.color == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.diffuseIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.direction == INVALID_UNIFORM_LOCATION) {
        exit(1);
    }
*/
    EG_DeferredShadingLightPass::initLightPass(RENDER_PASS1);

    initMemberVariables();
}

void EG_DeferredShadingDirectionalLightPass::loadUniformLocations(pipeline& p, int RenderPassID)
{
    p.shadowMatrix = p.Light_BiasMatrix * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
    glUniformMatrix4fv(l_modelViewProjectionMatrix_UniLoc_,1,GL_FALSE,&p.shadowMatrix[0][0]);
    EG_RenderTechnique::loadUniformLocations(p, RenderPassID);
}


void EG_DeferredShadingDirectionalLightPass::SetDirectionalLight(EG_DirectionalLight& Light)
{
    glUniform3f(m_dirLightLocation.color, Light.color.x, Light.color.y, Light.color.z);
    glUniform1f(m_dirLightLocation.ambientIntensity, Light.ambientIntensity);
    glm::vec3 direction = Light.direction;
    direction = glm::normalize(direction);
    glUniform3f(m_dirLightLocation.direction, direction.x, direction.y, direction.z);
    glUniform1f(m_dirLightLocation.diffuseIntensity, Light.diffuseIntensity);
}


