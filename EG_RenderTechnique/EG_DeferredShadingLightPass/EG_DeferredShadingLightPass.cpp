
#include "EG_DeferredShadingLightPass.h"

EG_DeferredShadingLightPass::EG_DeferredShadingLightPass()
{
}

EG_DeferredShadingLightPass::~EG_DeferredShadingLightPass()
{

}
/*
void EG_DeferredShadingLightPass::init(int shaderCount)
{
}
*/

#if 0
bool EG_DeferredShadingLightPass::init(int shaderCount)
{
    /*
	m_posTextureUnitLocation = glGetUniformLocation( shaderID, "gPositionMap");
	m_colorTextureUnitLocation = glGetUniformLocation( shaderID, "gColorMap");
	m_normalTextureUnitLocation = glGetUniformLocation( shaderID, "gNormalMap");
    m_eyeWorldPosLocation = glGetUniformLocation( shaderID, "gEyeWorldPos");
    m_matSpecularIntensityLocation = glGetUniformLocation( shaderID, "gMatSpecularIntensity");
    m_matSpecularPowerLocation = glGetUniformLocation( shaderID, "gSpecularPower");
    m_screenSizeLocation = glGetUniformLocation( shaderID, "gScreenSize");
*/
    /*
    m_WVPLocation = GetUniformLocation("gWVP");
	m_posTextureUnitLocation = GetUniformLocation("gPositionMap");
	m_colorTextureUnitLocation = GetUniformLocation("gColorMap");
	m_normalTextureUnitLocation = GetUniformLocation("gNormalMap");
    m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
    m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
    m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
    m_screenSizeLocation = GetUniformLocation("gScreenSize");
*/

    m_WVPLocation                   = GetUniformLocation(progShaders[RENDER_PASS1], "gWVP");
	m_posTextureUnitLocation        = GetUniformLocation(progShaders[RENDER_PASS1], "gPositionMap");
	m_colorTextureUnitLocation      = GetUniformLocation(progShaders[RENDER_PASS1], "gColorMap");
	m_normalTextureUnitLocation     = GetUniformLocation(progShaders[RENDER_PASS1], "gNormalMap");
    m_eyeWorldPosLocation           = GetUniformLocation(progShaders[RENDER_PASS1], "gEyeWorldPos");
    m_matSpecularIntensityLocation  = GetUniformLocation(progShaders[RENDER_PASS1], "gMatSpecularIntensity");
    m_matSpecularPowerLocation      = GetUniformLocation(progShaders[RENDER_PASS1], "gSpecularPower");
    m_screenSizeLocation            = GetUniformLocation(progShaders[RENDER_PASS1], "gScreenSize");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
        m_posTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_colorTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
		m_normalTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
        m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
        m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
        m_screenSizeLocation == INVALID_UNIFORM_LOCATION) {
        exit(1);
    }

	return true;
}
#endif


void EG_DeferredShadingLightPass::init(int nShaders)
{

}




void EG_DeferredShadingLightPass::initLightPass(int shaderId)
{
	m_posTextureUnitLocation        = GetUniformLocation(m_shaders[shaderId], "gPositionMap");
	m_colorTextureUnitLocation      = GetUniformLocation(m_shaders[shaderId], "gColorMap");
	m_normalTextureUnitLocation     = GetUniformLocation(m_shaders[shaderId], "gNormalMap");
	m_stencilTextureUnitLocation    = GetUniformLocation(m_shaders[shaderId], "gStencilMap");
    m_eyeWorldPosLocation           = GetUniformLocation(m_shaders[shaderId], "gEyeWorldPos");
    m_matSpecularIntensityLocation  = GetUniformLocation(m_shaders[shaderId], "gMatSpecularIntensity");
    m_matSpecularPowerLocation      = GetUniformLocation(m_shaders[shaderId], "gSpecularPower");
    m_screenSizeLocation            = GetUniformLocation(m_shaders[shaderId], "gScreenSize");
/*
	if (m_posTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_colorTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
		m_normalTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
        m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
        m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
        m_screenSizeLocation == INVALID_UNIFORM_LOCATION) {
        exit(1);
    }
*/
}



void EG_DeferredShadingLightPass::setPositionTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_posTextureUnitLocation, TextureUnit);
}

void EG_DeferredShadingLightPass::setColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorTextureUnitLocation, TextureUnit);
}

void EG_DeferredShadingLightPass::setNormalTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_normalTextureUnitLocation, TextureUnit);
}

void EG_DeferredShadingLightPass::setStencilTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_stencilTextureUnitLocation, TextureUnit);
}

void EG_DeferredShadingLightPass::setEyeWorldPos(const glm::vec3& EyePos)
{
//	glUniform3f(m_eyeWorldPosLocation, EyePos.x, EyePos.y, EyePos.z);
	setUniformLocation(m_eyeWorldPosLocation, EyePos);
}


void EG_DeferredShadingLightPass::setScreenSize(unsigned int Width, unsigned int Height)
{
//    glUniform2f(m_screenSizeLocation, (float)Width, (float)Height);
    setUniformLocation(m_screenSizeLocation, (float)Width, (float)Height);
}

void EG_DeferredShadingLightPass::setMatSpecularIntensity(float Intensity)
{
    // glUniform1f(m_matSpecularIntensityLocation, Intensity);
    setUniformLocation(m_screenSizeLocation, Intensity);
}

void EG_DeferredShadingLightPass::setMatSpecularPower(float Power)
{
//    glUniform1f(m_matSpecularPowerLocation, Power);
    setUniformLocation(m_screenSizeLocation, Power);
}


