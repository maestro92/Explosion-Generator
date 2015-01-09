
#include "EG_Technique_DepthTexture_Render.h"



Technique_DepthTexture_Render::Technique_DepthTexture_Render()
{

}



Technique_DepthTexture_Render::~Technique_DepthTexture_Render()
{



}


void Technique_DepthTexture_Render::init(int Shader_Num)
{
    allocateMemberVariables(Shader_Num);
    progShaders[RENDER_PASS1] = new Shader("Depth_CameraRender.vs", "Depth_CameraRender.fs");
    initMemberVariables();
}

