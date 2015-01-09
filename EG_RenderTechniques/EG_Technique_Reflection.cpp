
#include "EG_Technique_Reflection.h"


Technique_Reflection::Technique_Reflection()
{
}



Technique_Reflection::~Technique_Reflection()
{
}



void Technique_Reflection::init(int Shader_Num)
{
    allocateMemberVariables(Shader_Num);
    progShaders[RENDER_PASS1] = new Shader("Reflection.vs", "Reflection.frag");

    CameraPosition_UniformLocation = GetUniformLocation(progShaders[RENDER_PASS1], "cameraPosition");
    CubeMap_UniformLocation = GetUniformLocation(progShaders[RENDER_PASS1], "cubeMap");

    initMemberVariables();
}
