#include "EG_DeferredShading2.h"

EG_DeferredShading2::EG_DeferredShading2()
{

}



EG_DeferredShading2::~EG_DeferredShading2()
{

}

void EG_DeferredShading2::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorTextureUnitLocation_, TextureUnit);
}



void EG_DeferredShading2::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);
//    string path = "/EG_DeferredShadingShaders/";
    progShaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingGeometryPass.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingGeometryPass.fs");
    m_colorTextureUnitLocation_ = glGetUniformLocation( progShaders[RENDER_PASS1]->getProgramId(), "gColorMap");

    progShaders[RENDER_PASS2] = new Shader("EG_Shader_DeferredShadingLightingPass.vs",
                                           "EG_Shader_DeferredShadingLightingPass.fs");



    l_modelViewProjectionMatrix_UniLoc_ =       GetUniformLocation(progShaders[RENDER_PASS2], "l_modelViewProjectionMatrix");
    lightPosition_ModelViewMatrix_UniLoc_ =     GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ModelViewMatrix");
    lightPosition_ObjectSpace_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ObjectSpace");
    screenSize_UniLoc_                =         GetUniformLocation(progShaders[RENDER_PASS2], "gScreenSize");


    positionMap_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS2], "positionMap");
    colorMap_UniLoc_    =         GetUniformLocation(progShaders[RENDER_PASS2], "colorMap");
    normalMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS2], "normalMap");
    shadowMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS2], "shadowMap");


    progShaders[RENDER_PASS3] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingStencilPass.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingStencilPass.fs");


    progShaders[RENDER_PASS4] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingPointLightPass.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingPointLightPass.fs");
//    pointLightUniformLocation.lightPass =


    progShaders[RENDER_PASS5] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass2.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass2.fs");
//    directionalLightPassUniformLocation.lightPass =

    l_modelViewProjectionMatrix_UniLoc_ =       GetUniformLocation(progShaders[RENDER_PASS5], "l_modelViewProjectionMatrix");
    lightPosition_ModelViewMatrix_UniLoc_ =     GetUniformLocation(progShaders[RENDER_PASS5], "lightPosition_ModelViewMatrix");
    lightPosition_ObjectSpace_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS5], "lightPosition_ObjectSpace");
    screenSize_UniLoc_                =         GetUniformLocation(progShaders[RENDER_PASS5], "gScreenSize");


    positionMap_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS5], "positionMap");
    colorMap_UniLoc_    =         GetUniformLocation(progShaders[RENDER_PASS5], "colorMap");
    normalMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS5], "normalMap");
    shadowMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS5], "shadowMap");



    directionalLightPassUniformLocation.lightPass.m_posTextureUnitLocation      = GetUniformLocation(progShaders[RENDER_PASS5], "positionMap");
    directionalLightPassUniformLocation.lightPass.m_colorTextureUnitLocation    = GetUniformLocation(progShaders[RENDER_PASS5], "colorMap");
    directionalLightPassUniformLocation.lightPass.m_normalTextureUnitLocation   = GetUniformLocation(progShaders[RENDER_PASS5], "normalMap");
    directionalLightPassUniformLocation.lightPass.m_screenSizeLocation          = GetUniformLocation(progShaders[RENDER_PASS5], "gScreenSize");

    directionalLightPassUniformLocation.color               = GetUniformLocation( progShaders[RENDER_PASS5],
                                                                                   "gDirectionalLight.base.color");
    directionalLightPassUniformLocation.ambientIntensity    = GetUniformLocation( progShaders[RENDER_PASS5],
                                                                                   "gDirectionalLight.base.ambientIntensity");
    directionalLightPassUniformLocation.diffuseIntensity    = GetUniformLocation( progShaders[RENDER_PASS5],
                                                                                   "gDirectionalLight.base.diffuseIntensity");
    directionalLightPassUniformLocation.direction           = GetUniformLocation( progShaders[RENDER_PASS5],
                                                                                   "gDirectionalLight.direction");


/*
    LightMVPmatrix_UniLoc_ =             GetUniformLocation(progShaders[RENDER_PASS2], "l_modelViewProjectionMatrix");
    shadowMap_UniLoc_ =                  GetUniformLocation(progShaders[RENDER_PASS2], "shadowMap");
    LightPosition_MVmatrix_UniLoc_ =     GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ModelViewMatrix");
    LightPosition_UniLoc_ =              GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ObjectSpace");
    CameraPosition_UniLoc_ =             GetUniformLocation(progShaders[RENDER_PASS2], "cameraPosition");
*/
    initMemberVariables();
}


void EG_DeferredShading2::loadUniformLocations(pipeline& p, int RenderPassID)
{
    p.shadowMatrix = p.Light_BiasMatrix * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
    glUniformMatrix4fv(l_modelViewProjectionMatrix_UniLoc_,1,GL_FALSE,&p.shadowMatrix[0][0]);
    EG_RenderTechnique::loadUniformLocations(p, RenderPassID);
}



void EG_DeferredShading2::setPositionTextureUnit(GLuint uniformLocation, unsigned int TextureUnit)
{
    glUniform1i(uniformLocation, TextureUnit);
}

void EG_DeferredShading2::setColorTextureUnit(GLuint uniformLocation, unsigned int TextureUnit)
{
    glUniform1i(uniformLocation, TextureUnit);
}

void EG_DeferredShading2::setNormalTextureUnit(GLuint uniformLocation, unsigned int TextureUnit)
{
    glUniform1i(uniformLocation, TextureUnit);
}

void EG_DeferredShading2::setScreenSize(GLuint uniformLocation, unsigned int Width, unsigned int Height)
{
    glUniform2f(uniformLocation, (float)Width, (float)Height);
}

/*
void EG_DeferredShading2::setDirectionalLightPass(EG_DirectionalLight& Light)
{
    setDirectionalLight(Light);
}
*/

void EG_DeferredShading2::setDirectionalLight(EG_DirectionalLight& Light)
{
    glUniform3f(directionalLightPassUniformLocation.color, Light.color.x, Light.color.y, Light.color.z);
    glUniform1f(directionalLightPassUniformLocation.ambientIntensity, Light.ambientIntensity);
    glm::vec3 direction = Light.direction;
    direction = glm::normalize(direction);
    glUniform3f(directionalLightPassUniformLocation.direction, direction.x, direction.y, direction.z);
    glUniform1f(directionalLightPassUniformLocation.diffuseIntensity, Light.diffuseIntensity);
}

void EG_DeferredShading2::setPointLight(EG_PointLight& Light)
{

}

