
#include "EG_ReflectionRenderer.h"


EG_ReflectionRenderer::EG_ReflectionRenderer()
{
}



EG_ReflectionRenderer::~EG_ReflectionRenderer()
{
}




void EG_ReflectionRenderer::init(int Shader_Num)
{
    allocateMemberVariables(Shader_Num);
    m_shaders[RENDER_PASS1] = new Shader("Reflection.vs", "Reflection.fs");

    m_cameraPositionUniLoc      = GetUniformLocation(m_shaders[RENDER_PASS1], "m_cameraPosition");
    m_reflectionTextureIdUniLoc = GetUniformLocation(m_shaders[RENDER_PASS1], "m_reflectionTexture");

    initMemberVariables();
}

void EG_ReflectionRenderer::setCameraPosition(glm::vec3 camPos)
{
    m_cameraPosition = camPos;
}


void EG_ReflectionRenderer::setReflectionTextureId(GLuint id)
{
    m_reflectionTextureId = id;
}

/*
void EG_ReflectionRenderer::setReflectionTextureUnit(GLuint unit)
{
    m_reflectionTextureUnit = unit;
}
*/

void EG_ReflectionRenderer::loadUniformLocations(pipeline& p, int RenderPassID)
{
    setUniformLocation(m_cameraPositionUniLoc, m_cameraPosition);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_reflectionTextureId);
    setUniformLocation(m_reflectionTextureIdUniLoc, 0);

    EG_Renderer::loadUniformLocations(p, RenderPassID);
}


