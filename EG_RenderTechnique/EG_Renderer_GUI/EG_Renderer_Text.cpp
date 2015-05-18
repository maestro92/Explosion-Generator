
#include "EG_Renderer_Text.h"


EG_Renderer_Text::EG_Renderer_Text()
{
}

EG_Renderer_Text::~EG_Renderer_Text()
{

}


void EG_Renderer_Text::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("/EG_GUIShaders/EG_TextShader.vs", "/EG_GUIShaders/EG_TextShader.fs");
    m_textureUnitUniformLocation = GetUniformLocation( m_shaders[RENDER_PASS1], "gTexture");

    GLuint nice = GetUniformLocation( m_shaders[RENDER_PASS1], "apple");
    initMemberVariables();
}


void EG_Renderer_Text::setTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_textureUnitUniformLocation, TextureUnit);
}
