
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
    m_shaders[RENDER_PASS1] = new Shader("Reflection.vs", "Reflection.fs");

    m_cameraPositionUniLoc      = GetUniformLocation(m_shaders[RENDER_PASS1], "m_cameraPosition");
    m_reflectionTextureIdUniLoc = GetUniformLocation(m_shaders[RENDER_PASS1], "m_reflectionTexture");

    initMemberVariables();
}

void Technique_Reflection::setCameraPosition(glm::vec3 camPos)
{
    m_cameraPosition = camPos;
}


void Technique_Reflection::setReflectionTextureId(GLuint id)
{
    m_reflectionTextureId = id;
}

/*
void Technique_Reflection::setReflectionTextureUnit(GLuint unit)
{
    m_reflectionTextureUnit = unit;
}
*/

void Technique_Reflection::loadUniformLocations(pipeline& p, int RenderPassID)
{
    setUniformLocation(m_cameraPositionUniLoc, m_cameraPosition);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_reflectionTextureId);
    setUniformLocation(m_reflectionTextureIdUniLoc, 0);

    EG_RenderTechnique::loadUniformLocations(p, RenderPassID);
}


