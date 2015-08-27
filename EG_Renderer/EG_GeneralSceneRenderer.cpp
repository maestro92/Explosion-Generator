
#include "EG_GeneralSceneRenderer.h"


EG_GeneralSceneRenderer::EG_GeneralSceneRenderer()
{


}



EG_GeneralSceneRenderer::~EG_GeneralSceneRenderer()
{



}





void EG_GeneralSceneRenderer::init(int w, int h, int Shader_Num)
{
    allocateMemberVariables(Shader_Num);

    ShadowMapWidth = w*2;
    ShadowMapHeight = h*2;

    /// FirstPass_LightPOV
    /// SecondPass_CameraPOV
    m_shaders[RENDER_PASS1] = new Shader("EG_SceneRendererShadowMap.vs", "EG_SceneRendererShadowMap.fs");
    m_shaders[RENDER_PASS2] = new Shader("EG_SceneRenderer.vs", "EG_SceneRenderer.fs");

    initDirLightUniformLocations(RENDER_PASS2);
    initPointLightUniformLocations(RENDER_PASS2);
    initSpotLightUniformLocations(RENDER_PASS2);

    l_modelViewProjectionMatrix_UniLoc_     = GetUniformLocation( m_shaders[RENDER_PASS2], "l_modelViewProjectionMatrix");
    shadowMap_UniLoc                        = GetUniformLocation( m_shaders[RENDER_PASS2], "gShadowMap");
    initMemberVariables();
}













void EG_GeneralSceneRenderer::initDirLightUniLocs(int pass)
{
   // addDataPair(pass, gDirectionalLight.base.color, )
}

void EG_GeneralSceneRenderer::initPointLightUniLoc(int pass)
{


}

void EG_GeneralSceneRenderer::initSpotLightUniLocs(int pass)
{

}

void EG_GeneralSceneRenderer::setDirLightUniLocs(int pass)
{

}

void EG_GeneralSceneRenderer::setPointLightUniLoc(int pass)
{

}
void EG_GeneralSceneRenderer::setSpotLightUniLocs(int pass)
{

}




void EG_GeneralSceneRenderer::initDirLightUniformLocations(int pass)
{

    m_dirLightLocation.color                = GetUniformLocation( m_shaders[pass], "gDirectionalLight.base.color");
    m_dirLightLocation.ambientIntensity     = GetUniformLocation( m_shaders[pass], "gDirectionalLight.base.ambientIntensity");
    m_dirLightLocation.diffuseIntensity     = GetUniformLocation( m_shaders[pass], "gDirectionalLight.base.diffuseIntensity");
    m_dirLightLocation.direction            = GetUniformLocation( m_shaders[pass], "gDirectionalLight.direction");

    m_eyeWorldPosLocation                   = GetUniformLocation( m_shaders[pass], "gEyeWorldPos");
    m_matSpecularIntensityLocation          = GetUniformLocation( m_shaders[pass], "gMatSpecularIntensity");
    m_matSpecularPowerLocation              = GetUniformLocation( m_shaders[pass], "gSpecularPower");

}

void EG_GeneralSceneRenderer::initPointLightUniformLocations(int pass)
{
    gNumPointLights = GetUniformLocation( m_shaders[pass], "gNumPointLights");

    for (unsigned int i = 0 ; i < 103 ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gPointLights[%d].base.color", i);
        m_pointLightsLocation[i].color = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].base.ambientIntensity", i);
        m_pointLightsLocation[i].ambientIntensity = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].position", i);
        m_pointLightsLocation[i].position = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].base.diffuseIntensity", i);
        m_pointLightsLocation[i].diffuseIntensity = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].atten.constant", i);
        m_pointLightsLocation[i].attenuation.constant = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].atten.linear", i);
        m_pointLightsLocation[i].attenuation.linear = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].atten.exp", i);
        m_pointLightsLocation[i].attenuation.exp = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].scale", i);
        m_pointLightsLocation[i].scale = GetUniformLocation( m_shaders[pass], Name);
    }

}



void EG_GeneralSceneRenderer::initSpotLightUniformLocations(int pass)
{

}



void EG_GeneralSceneRenderer::loadUniformLocations(pipeline& p, int RenderPassID)
{
    p.updateShadowMatrix();
//    p.shadowMatrix = LIGHT_BIAS_MATRIX * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
    glUniformMatrix4fv(l_modelViewProjectionMatrix_UniLoc_,1,GL_FALSE,&p.m_shadowMatrix[0][0]);
    EG_Renderer::loadUniformLocations(p, RenderPassID);
}


void EG_GeneralSceneRenderer::setDirectionalLight(EG_DirectionalLight& Light)
{
    setUniformLocation(m_dirLightLocation.color, Light.color);
    setUniformLocation(m_dirLightLocation.ambientIntensity, Light.ambientIntensity);
    glm::vec3 direction = Light.direction;
    direction = glm::normalize(direction);
    setUniformLocation(m_dirLightLocation.direction, direction);
    setUniformLocation(m_dirLightLocation.diffuseIntensity, Light.diffuseIntensity);
}




void EG_GeneralSceneRenderer::setPointLight(int index, EG_PointLight& Light)
{
    setUniformLocation(m_pointLightsLocation[index].color, Light.color);
    setUniformLocation(m_pointLightsLocation[index].ambientIntensity, Light.ambientIntensity);
    setUniformLocation(m_pointLightsLocation[index].diffuseIntensity, Light.diffuseIntensity);
    setUniformLocation(m_pointLightsLocation[index].position, Light.position);
    setUniformLocation(m_pointLightsLocation[index].attenuation.constant, Light.attenuation.constant);
    setUniformLocation(m_pointLightsLocation[index].attenuation.linear, Light.attenuation.linear);
    setUniformLocation(m_pointLightsLocation[index].attenuation.exp, Light.attenuation.exp);
    float scalef = computePointLightBoundingSphere(Light);
    setUniformLocation(m_pointLightsLocation[index].scale, scalef);
}



float EG_GeneralSceneRenderer::computePointLightBoundingSphere(EG_PointLight& Light)
{
    float MaxChannel = fmax(fmax(Light.color.x, Light.color.y), Light.color.z);

    float ret = (-Light.attenuation.linear + sqrtf(Light.attenuation.linear * Light.attenuation.linear - 4 * Light.attenuation.exp * (Light.attenuation.exp - 256 * MaxChannel * Light.diffuseIntensity)))
                /
                2 * Light.attenuation.exp;
    return ret;
}


void EG_GeneralSceneRenderer::setPointLightCount(int count)
{
	setUniformLocation(gNumPointLights, count);
}




void EG_GeneralSceneRenderer::setEyeWorldPos(const glm::vec3& EyePos)
{
	setUniformLocation(m_eyeWorldPosLocation, EyePos);
}



void EG_GeneralSceneRenderer::setMatSpecularIntensity(float Intensity)
{
    setUniformLocation(m_matSpecularIntensityLocation, Intensity);
}

void EG_GeneralSceneRenderer::setMatSpecularPower(float Power)
{
    setUniformLocation(m_matSpecularPowerLocation, Power);

}



