
#include "EG_Technique_DepthTexture_Render.h"



EG_RenderTechnique_RenderDepthToTexture::EG_RenderTechnique_RenderDepthToTexture()
{

}



EG_RenderTechnique_RenderDepthToTexture::~EG_RenderTechnique_RenderDepthToTexture()
{



}


void EG_RenderTechnique_RenderDepthToTexture::init(int Shader_Num)
{
    allocateMemberVariables(Shader_Num);
    progShaders[RENDER_PASS1] = new Shader("EG_RenderDepthToTexture.vs", "EG_RenderDepthToTexture.fs");
    initMemberVariables();
}

