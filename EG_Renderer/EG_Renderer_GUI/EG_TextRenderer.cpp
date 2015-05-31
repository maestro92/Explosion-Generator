#include "EG_TextRenderer.h"

EG_TextRenderer::EG_TextRenderer()
{
}

EG_TextRenderer::~EG_TextRenderer()
{

}


void EG_TextRenderer::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("/EG_GUIShaders/EG_Renderer_Text.vs", "/EG_GUIShaders/EG_Renderer_Text.fs");
    m_textureUnitUniformLocation = GetUniformLocation( m_shaders[RENDER_PASS1], "gTexture");

    GLuint nice = GetUniformLocation( m_shaders[RENDER_PASS1], "apple");
    initMemberVariables();
}


void EG_TextRenderer::setTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_textureUnitUniformLocation, TextureUnit);
}
