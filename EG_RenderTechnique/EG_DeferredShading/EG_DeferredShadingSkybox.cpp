
#include "EG_DeferredShadingSkybox.h"

EG_DeferredShadingSkybox::EG_DeferredShadingSkybox()
{

}

EG_DeferredShadingSkybox::~EG_DeferredShadingSkybox()
{

}

void EG_DeferredShadingSkybox::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);
    m_shaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingSkybox.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingSkybox.fs");

	m_cubeMapTextureUnitLocation = GetUniformLocation(m_shaders[RENDER_PASS1], "gCubeMap");
//    m_stencilFlagUniformLocation = GetUniformLocation(progShaders[RENDER_PASS1], "stencilFlag");
    // EG_DeferredShadingLightPass::initLightPass(RENDER_PASS1);
    EG_DeferredShading::initStencilUniformLocation(RENDER_PASS1);
    initMemberVariables();
}

void EG_DeferredShadingSkybox::setCubeMapTextureUnit(unsigned int TextureUnit)
{
    glUniform1f(m_cubeMapTextureUnitLocation, TextureUnit);
}

/*
void EG_DeferredShadingSkybox::setStencilFlag(glm::vec3 stencilFlag)
{
    m_stencilFlag = stencilFlag;
}
*/

/*
void EG_DeferredShadingSkybox::loadUniformLocations(pipeline& p, int RenderPassID)
{
    glUniform3f(m_stencilFlagUniformLocation, m_stencilFlag.x, m_stencilFlag.y, m_stencilFlag.z);
    EG_RenderTechnique::loadUniformLocations(p, RenderPassID);
}
*/
