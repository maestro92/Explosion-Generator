
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

//    progShaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_ModelMatrixBased.vs",
//                                           "/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_ModelMatrixBased.fs");

//    progShaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass.vs",
//                                           "/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass.fs");

    m_shaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_WithShadows.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_WithShadows.fs");


    m_dirLightLocation.color                = GetUniformLocation( m_shaders[RENDER_PASS1], "gDirectionalLight.base.color");
    m_dirLightLocation.ambientIntensity     = GetUniformLocation( m_shaders[RENDER_PASS1], "gDirectionalLight.base.ambientIntensity");
    m_dirLightLocation.diffuseIntensity     = GetUniformLocation( m_shaders[RENDER_PASS1], "gDirectionalLight.base.diffuseIntensity");
    m_dirLightLocation.direction            = GetUniformLocation( m_shaders[RENDER_PASS1], "gDirectionalLight.direction");


    l_modelViewProjectionMatrix_UniLoc_ =       GetUniformLocation(m_shaders[RENDER_PASS1], "l_modelViewProjectionMatrix");
    lightPosition_ModelViewMatrix_UniLoc_ =     GetUniformLocation(m_shaders[RENDER_PASS1], "lightPosition_ModelViewMatrix");
    lightPosition_ObjectSpace_UniLoc_ =         GetUniformLocation(m_shaders[RENDER_PASS1], "lightPosition_ObjectSpace");

    m_shadowMapTextureUnitLocation  =   GetUniformLocation(m_shaders[RENDER_PASS1], "gShadowMap");

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



void EG_DeferredShadingDirectionalLightPass::init(int shaderCount, char* vsSource, char* fsSource)
{
    allocateMemberVariables(shaderCount);

//    progShaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_ModelMatrixBased.vs",
//                                           "/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_ModelMatrixBased.fs");

    m_shaders[RENDER_PASS1] = new Shader(vsSource, fsSource);


    m_dirLightLocation.color                = GetUniformLocation( m_shaders[RENDER_PASS1], "gDirectionalLight.base.color");
    m_dirLightLocation.ambientIntensity     = GetUniformLocation( m_shaders[RENDER_PASS1], "gDirectionalLight.base.ambientIntensity");
    m_dirLightLocation.diffuseIntensity     = GetUniformLocation( m_shaders[RENDER_PASS1], "gDirectionalLight.base.diffuseIntensity");
    m_dirLightLocation.direction            = GetUniformLocation( m_shaders[RENDER_PASS1], "gDirectionalLight.direction");


    l_modelViewProjectionMatrix_UniLoc_ =       GetUniformLocation(m_shaders[RENDER_PASS1], "l_modelViewProjectionMatrix");
//    lightPosition_ModelViewMatrix_UniLoc_ =     GetUniformLocation(m_shaders[RENDER_PASS1], "lightPosition_ModelViewMatrix");
    lightPosition_ObjectSpace_UniLoc_ =         GetUniformLocation(m_shaders[RENDER_PASS1], "lightPosition");

    m_shadowMapTextureUnitLocation  =       GetUniformLocation(m_shaders[RENDER_PASS1], "gShadowMap");


//    shadowMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS1], "shadowMap");

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
 /*
    p.shadowMatrix = LIGHT_BIAS_MATRIX * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
    glUniformMatrix4fv(l_modelViewProjectionMatrix_UniLoc_,1,GL_FALSE,&p.shadowMatrix[0][0]);
    EG_Renderer::loadUniformLocations(p, RenderPassID);
*/
}


void EG_DeferredShadingDirectionalLightPass::setDirectionalLight(EG_DirectionalLight& Light)
{
    glUniform3f(m_dirLightLocation.color, Light.color.x, Light.color.y, Light.color.z);
    glUniform1f(m_dirLightLocation.ambientIntensity, Light.ambientIntensity);
    glm::vec3 direction = Light.direction;
    direction = glm::normalize(direction);
    glUniform3f(m_dirLightLocation.direction, direction.x, direction.y, direction.z);
    glUniform1f(m_dirLightLocation.diffuseIntensity, Light.diffuseIntensity);
}





void EG_DeferredShadingDirectionalLightPass::setShadowMapTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_shadowMapTextureUnitLocation, TextureUnit);
}
