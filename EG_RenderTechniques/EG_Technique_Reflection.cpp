
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

    m_cameraPositionUniLoc      = GetUniformLocation(progShaders[RENDER_PASS1], "cameraPosition");
    m_reflectionTextureIdUniLoc = GetUniformLocation(progShaders[RENDER_PASS1], "cubeMap");

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


