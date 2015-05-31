
#include "EG_FullColorRenderer.h"

EG_FullColorRenderer::EG_FullColorRenderer()
{
}

EG_FullColorRenderer::~EG_FullColorRenderer()
{

}


void EG_FullColorRenderer::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("EG_FullVertexColorShader.vs", "EG_FullVertexColorShader.fs");
 //   m_colorUniformLocation = GetUniformLocation( m_shaders[RENDER_PASS1], "gColor");
    initMemberVariables();
}

void EG_FullColorRenderer::setColor(glm::vec3 color)
{
    setUniformLocation(m_colorUniformLocation, color);
}
