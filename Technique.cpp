
#include "Technique.h"


Technique::Technique()
{

}

Technique::~Technique()
{

}


void Technique::init()
{

}



void Technique::EnableShader()
{
    Shader->useShader();
}


void Technique::DisableShader()
{
    Shader->delShader();
}
