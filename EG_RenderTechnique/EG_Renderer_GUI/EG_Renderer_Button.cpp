
#include "EG_Renderer_Button.h"


EG_Renderer_Button::EG_Renderer_Button()
{
}

EG_Renderer_Button::~EG_Renderer_Button()
{

}


void EG_Renderer_Button::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("/EG_GUIShaders/EG_Renderer_Button.vs", "/EG_GUIShaders/EG_Renderer_Button.fs");

    m_shaders[RENDER_PASS2] = new Shader("/EG_GUIShaders/EG_Renderer_ButtonWithDynamicColor.vs", "/EG_GUIShaders/EG_Renderer_ButtonWithDynamicColor.fs");
    m_colorUniformLocation = GetUniformLocation( m_shaders[RENDER_PASS2], "gColor");

    m_shaders[RENDER_PASS3] = new Shader("/EG_GUIShaders/EG_Renderer_ButtonWithTexture.vs", "/EG_GUIShaders/EG_Renderer_ButtonWithTexture.fs");
    m_textureUnitUniformLocation = GetUniformLocation( m_shaders[RENDER_PASS3], "gTexture");

    initMemberVariables();
}


void EG_Renderer_Button::setColor(glm::vec3 color)
{
    setUniformLocation(m_colorUniformLocation, color);
}

void EG_Renderer_Button::setTextureUnit(unsigned int textureUnit)
{
    glUniform1i(m_textureUnitUniformLocation, textureUnit);
}

