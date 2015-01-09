#include "EG_DeferredShading.h"

EG_DeferredShading::EG_DeferredShading()
{

}



EG_DeferredShading::~EG_DeferredShading()
{

}

void EG_DeferredShading::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorTextureUnitLocation_, TextureUnit);
}



void EG_DeferredShading::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);

    progShaders[RENDER_PASS1] = new Shader("EG_Shader_DeferredShadingGeometryPass.vs", "EG_Shader_DeferredShadingGeometryPass.fs");
    m_colorTextureUnitLocation_ = glGetUniformLocation( progShaders[RENDER_PASS1]->getProgramId(), "gColorMap");

    progShaders[RENDER_PASS2] = new Shader("EG_Shader_DeferredShadingLightingPass.vs", "EG_Shader_DeferredShadingLightingPass.fs");



    l_modelViewProjectionMatrix_UniLoc_ =       GetUniformLocation(progShaders[RENDER_PASS2], "l_modelViewProjectionMatrix");
    lightPosition_ModelViewMatrix_UniLoc_ =     GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ModelViewMatrix");
    lightPosition_ObjectSpace_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ObjectSpace");
    screenSize_UniLoc_                =         GetUniformLocation(progShaders[RENDER_PASS2], "gScreenSize");


    positionMap_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS2], "positionMap");
    colorMap_UniLoc_    =         GetUniformLocation(progShaders[RENDER_PASS2], "colorMap");
    normalMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS2], "normalMap");
    shadowMap_UniLoc_   =         GetUniformLocation(progShaders[RENDER_PASS2], "shadowMap");


/*
    LightMVPmatrix_UniLoc_ =             GetUniformLocation(progShaders[RENDER_PASS2], "l_modelViewProjectionMatrix");
    shadowMap_UniLoc_ =                  GetUniformLocation(progShaders[RENDER_PASS2], "shadowMap");
    LightPosition_MVmatrix_UniLoc_ =     GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ModelViewMatrix");
    LightPosition_UniLoc_ =              GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ObjectSpace");
    CameraPosition_UniLoc_ =             GetUniformLocation(progShaders[RENDER_PASS2], "cameraPosition");
*/
    initMemberVariables();
}


void EG_DeferredShading::loadUniformLocations(pipeline& p, int RenderPassID)
{
    p.shadowMatrix = p.Light_BiasMatrix * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
    glUniformMatrix4fv(l_modelViewProjectionMatrix_UniLoc_,1,GL_FALSE,&p.shadowMatrix[0][0]);
    EG_RenderTechnique::loadUniformLocations(p, RenderPassID);
}
