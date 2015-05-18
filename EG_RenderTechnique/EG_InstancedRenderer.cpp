#include "EG_InstancedRenderer.h"



EG_InstancedRenderer::EG_InstancedRenderer()
{
}

EG_InstancedRenderer::~EG_InstancedRenderer()
{

}


void EG_InstancedRenderer::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("EG_InstancedRenderingShadowMap.vs", "EG_InstancedRenderingShadowMap.fs");
    m_shaders[RENDER_PASS2] = new Shader("EG_InstancedRendering.vs", "EG_InstancedRendering.fs");

    initDirLightUniformLocations(RENDER_PASS2);

    initMemberVariables();
}

