#include "EG_DeferredShading.h"

EG_DeferredShading::EG_DeferredShading()
{

}



EG_DeferredShading::~EG_DeferredShading()
{

}


void EG_DeferredShading::init(int nShaders)
{

}


void EG_DeferredShading::initStencilUniformLocation(int shaderId)
{
	m_stencilFlagUniformLocation = GetUniformLocation(m_shaders[shaderId], "stencilFlag");
}


void EG_DeferredShading::setStencilFlag(glm::vec3 stencilFlag)
{
    m_stencilFlag = stencilFlag;
}

void EG_DeferredShading::loadUniformLocations(pipeline& p, int RenderPassID)
{
    glUniform3f(m_stencilFlagUniformLocation, m_stencilFlag.x, m_stencilFlag.y, m_stencilFlag.z);
    EG_Renderer::loadUniformLocations(p, RenderPassID);
}
