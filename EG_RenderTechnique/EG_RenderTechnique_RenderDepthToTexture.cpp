
#include "EG_RenderTechnique_RenderDepthToTexture.h"



EG_RenderTechnique_RenderDepthToTexture::EG_RenderTechnique_RenderDepthToTexture()
{

}



EG_RenderTechnique_RenderDepthToTexture::~EG_RenderTechnique_RenderDepthToTexture()
{



}


void EG_RenderTechnique_RenderDepthToTexture::init(int Shader_Num)
{
    allocateMemberVariables(Shader_Num);
    m_shaders[RENDER_PASS1] = new Shader("EG_RenderDepthToTexture.vs", "EG_RenderDepthToTexture.fs");
    initMemberVariables();
}

