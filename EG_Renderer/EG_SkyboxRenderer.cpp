
#include "EG_SkyboxRenderer.h"


EG_SkyboxRenderer::EG_SkyboxRenderer()
{
}

EG_SkyboxRenderer::~EG_SkyboxRenderer()
{

}






void EG_SkyboxRenderer::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("EG_SkyboxShader.vs", "EG_SkyboxShader.fs");

    initMemberVariables();
    m_cubemapTextureUniLoc = GetUniformLocation(m_shaders[RENDER_PASS1], "gCubemap");
}


void EG_SkyboxRenderer::setCubeMapTextureUnit(int unit)
{
    m_cubemapTextureUnit = unit;
}


void EG_SkyboxRenderer::loadUniformLocations(pipeline& p, int RenderPassID)
{
    setUniformLocation(m_cubemapTextureUniLoc, m_cubemapTextureUnit);
    EG_Renderer::loadUniformLocations(p, RenderPassID);
}
