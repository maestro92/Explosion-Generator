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
    m_shaders[RENDER_PASS1] = new Shader("EG_InstancedRenderingShader.vs", "EG_InstancedRenderingShader.fs");

    initDirLightUniformLocations(RENDER_PASS1);

    initMemberVariables();
}

