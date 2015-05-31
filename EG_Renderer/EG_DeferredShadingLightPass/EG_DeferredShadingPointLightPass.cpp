#include "EG_DeferredShadingPointLightPass.h"


EG_DeferredShadingPointLightPass::EG_DeferredShadingPointLightPass()
{
}

EG_DeferredShadingPointLightPass::~EG_DeferredShadingPointLightPass()
{
}

void EG_DeferredShadingPointLightPass::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);

    m_shaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingPointLightPass.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingPointLightPass.fs");

    m_pointLightUniformLocation.color                = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.base.color");
    m_pointLightUniformLocation.ambientIntensity     = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.base.ambientIntensity");
    m_pointLightUniformLocation.diffuseIntensity     = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.base.diffuseIntensity");
    m_pointLightUniformLocation.position             = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.position");

    m_pointLightUniformLocation.attenuation.constant = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.atten.constant");
    m_pointLightUniformLocation.attenuation.linear   = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.atten.linear");
    m_pointLightUniformLocation.attenuation.exp      = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.atten.exp");




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




void EG_DeferredShadingPointLightPass::init(int shaderCount, char* vsSource, char* fsSource)
{
    allocateMemberVariables(shaderCount);

    m_shaders[RENDER_PASS1] = new Shader(vsSource, fsSource);


    m_pointLightUniformLocation.color                = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.base.color");
    m_pointLightUniformLocation.ambientIntensity     = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.base.ambientIntensity");
    m_pointLightUniformLocation.diffuseIntensity     = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.base.diffuseIntensity");
    m_pointLightUniformLocation.position             = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.position");

    m_pointLightUniformLocation.attenuation.constant = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.atten.constant");
    m_pointLightUniformLocation.attenuation.linear   = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.atten.linear");
    m_pointLightUniformLocation.attenuation.exp      = GetUniformLocation( m_shaders[RENDER_PASS1], "gPointLight.atten.exp");

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






void EG_DeferredShadingPointLightPass::loadUniformLocations(pipeline& p, int RenderPassID)
{
    p.updateShadowMatrix();
//    p.shadowMatrix = LIGHT_BIAS_MATRIX * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
 //   glUniformMatrix4fv(l_modelViewProjectionMatrix_UniLoc_,1,GL_FALSE,&p.m_shadowMatrix[0][0]);
    EG_Renderer::loadUniformLocations(p, RenderPassID);
}


void EG_DeferredShadingPointLightPass::setPointLight(EG_PointLight& Light)
{
    glUniform3f(m_pointLightUniformLocation.color, Light.color.x, Light.color.y, Light.color.z);
    glUniform1f(m_pointLightUniformLocation.ambientIntensity, Light.ambientIntensity);
    glUniform1f(m_pointLightUniformLocation.diffuseIntensity, Light.diffuseIntensity);
    glUniform3f(m_pointLightUniformLocation.position, Light.position.x, Light.position.y, Light.position.z);
    glUniform1f(m_pointLightUniformLocation.attenuation.constant, Light.attenuation.constant);
    glUniform1f(m_pointLightUniformLocation.attenuation.linear, Light.attenuation.linear);
    glUniform1f(m_pointLightUniformLocation.attenuation.exp, Light.attenuation.exp);
}



float EG_DeferredShadingPointLightPass::computePointLightBoundingSphere(EG_PointLight& Light)
{
    float MaxChannel = fmax(fmax(Light.color.x, Light.color.y), Light.color.z);

    float ret = (-Light.attenuation.linear + sqrtf(Light.attenuation.linear * Light.attenuation.linear - 4 * Light.attenuation.exp * (Light.attenuation.exp - 256 * MaxChannel * Light.diffuseIntensity)))
                /
                2 * Light.attenuation.exp;
    return ret;
//    return 50;

//    return 0;
}


