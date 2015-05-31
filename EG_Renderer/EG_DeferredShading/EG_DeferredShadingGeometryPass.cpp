#include "EG_DeferredShadingGeometryPass.h"

EG_DeferredShadingGeometryPass::EG_DeferredShadingGeometryPass()
{

}

EG_DeferredShadingGeometryPass::~EG_DeferredShadingGeometryPass()
{

}

void EG_DeferredShadingGeometryPass::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);

    m_shaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingGeometryPass.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingGeometryPass.fs");

    m_stencilFlagUniformLocation = GetUniformLocation( m_shaders[RENDER_PASS1], "stencilFlag");
    initMemberVariables();
}

void EG_DeferredShadingGeometryPass::setStencilFlag(glm::vec3 stencilFlag)
{
    m_stencilFlag = stencilFlag;
}

void EG_DeferredShadingGeometryPass::loadUniformLocations(pipeline& p, int RenderPassID)
{
    glUniform3f(m_stencilFlagUniformLocation, m_stencilFlag.x, m_stencilFlag.y, m_stencilFlag.z);
    EG_Renderer::loadUniformLocations(p, RenderPassID);
}
