
#include "EG_DepthRenderer.h"



EG_DepthRenderer::EG_DepthRenderer()
{

}



EG_DepthRenderer::~EG_DepthRenderer()
{



}


void EG_DepthRenderer::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("EG_RenderDepthToTexture.vs", "EG_RenderDepthToTexture.fs");
    initMemberVariables();
}

