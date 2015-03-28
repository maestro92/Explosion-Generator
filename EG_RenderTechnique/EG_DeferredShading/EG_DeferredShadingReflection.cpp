
#include "EG_DeferredShadingReflection.h"



EG_DeferredShadingReflection::EG_DeferredShadingReflection()
{

}

EG_DeferredShadingReflection::~EG_DeferredShadingReflection()
{

}

void EG_DeferredShadingReflection::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);
    m_shaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingReflection.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingReflection.fs");


    m_cubeMapTextureUnitLocation = GetUniformLocation(m_shaders[RENDER_PASS1], "gCubeMap");
    m_eyeWorldPosUniformLocation = GetUniformLocation(m_shaders[RENDER_PASS1], "gEyeWorldPos");
    EG_DeferredShading::initStencilUniformLocation(RENDER_PASS1);
    initMemberVariables();
}


void EG_DeferredShadingReflection::setCubeMapTextureUnit(unsigned int TextureUnit)
{
    glUniform1f(m_cubeMapTextureUnitLocation, TextureUnit);
}

void EG_DeferredShadingReflection::setEyeWorldPos(const glm::vec3& EyePos)
{
	glUniform3f(m_eyeWorldPosUniformLocation, EyePos.x, EyePos.y, EyePos.z);
}
