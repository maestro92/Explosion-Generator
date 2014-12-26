
#include "EG_Technique_DepthTexture_Render.h"



Technique_DepthTexture_Render::Technique_DepthTexture_Render()
{

}



Technique_DepthTexture_Render::~Technique_DepthTexture_Render()
{



}


void Technique_DepthTexture_Render::init(int Shader_Num)
{
    allocate_memberVariables(Shader_Num);
    ProgShaders[RENDER_PASS1] = new Shader("Depth_CameraRender.vs", "Depth_CameraRender.fs");
    init_memberVariables();
}

