
#include "EG_DeferredShadingLightPos.h"

EG_DeferredShadingLightPos::EG_DeferredShadingLightPos()
{

}

EG_DeferredShadingLightPos::~EG_DeferredShadingLightPos()
{

}

void EG_DeferredShadingLightPos::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);
    m_shaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingLightPos.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingLightPos.fs");
    m_lightColor = GetUniformLocation(m_shaders[RENDER_PASS1], "gLightColor");
    EG_DeferredShading::initStencilUniformLocation(RENDER_PASS1);
    initMemberVariables();
}


void EG_DeferredShadingLightPos::setLightColor(glm::vec3 color)
{
	glUniform3f(m_lightColor, color.x, color.y, color.z);
}
