
#include "EG_Technique_Reflection.h"


Technique_Reflection::Technique_Reflection()
{
}



Technique_Reflection::~Technique_Reflection()
{
}



void Technique_Reflection::init(int Shader_Num)
{
    allocate_memberVariables(Shader_Num);
    ProgShaders[RENDER_PASS1] = new shader("Reflection.vs", "Reflection.frag");

    CameraPosition_UniformLocation = GetUniformLocation(ProgShaders[RENDER_PASS1], "cameraPosition");
    CubeMap_UniformLocation = GetUniformLocation(ProgShaders[RENDER_PASS1], "cubeMap");

    init_memberVariables();
}
