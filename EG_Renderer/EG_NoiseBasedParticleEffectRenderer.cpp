
#include "EG_NoiseBasedParticleEffectRenderer.h"


EG_NoiseBasedParticleEffectRenderer::EG_NoiseBasedParticleEffectRenderer()
{

}

EG_NoiseBasedParticleEffectRenderer::~EG_NoiseBasedParticleEffectRenderer()
{

}

void EG_NoiseBasedParticleEffectRenderer::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    /*
    m_shaders[RENDER_PASS1] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_BlitShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_BlitShader.gs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_BlitShader.fs");

    m_depthPair;
    m_scrollOffsetTimePair;
    initDataPairUniLoc(&m_depthPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "gDepth");
    initDataPairUniLoc(&m_scrollOffsetTimePair, m_shaders[RENDER_PASS1], RENDER_PASS1, "gScrollOffsetTime");
*/


    m_shaders[RENDER_PASS1] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.gs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.fs");
    initDataPairUniLoc(&m_colorPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,         m_shaders[RENDER_PASS1], RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,     m_shaders[RENDER_PASS1], RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,    m_shaders[RENDER_PASS1], RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,      m_shaders[RENDER_PASS1], RENDER_PASS1, "u_inverseSize");


    m_shaders[RENDER_PASS2] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.fs");
    initDataPairUniLoc(&m_depthPair3,           m_shaders[RENDER_PASS2], RENDER_PASS2, "u_depth");
    initDataPairUniLoc(&m_inverseSizePair3,     m_shaders[RENDER_PASS2], RENDER_PASS2, "u_inverseSize");
    initDataPairUniLoc(&m_backgroundTexturePair,m_shaders[RENDER_PASS2], RENDER_PASS2, "u_backgroundTexture");
    initDataPairUniLoc(&m_particlesTexturePair, m_shaders[RENDER_PASS2], RENDER_PASS2, "u_particlesTexture");

    getAllMatrixUniLocs();
}


void EG_NoiseBasedParticleEffectRenderer::setDepth(float color)
{

}


void EG_NoiseBasedParticleEffectRenderer::setScrollOffsetTime(float value)
{

}




void EG_NoiseBasedParticleEffectRenderer::setColor(glm::vec4 color)
{
    m_colorPair.value = color;
}

void EG_NoiseBasedParticleEffectRenderer::setFadeRate(float value)
{
    m_fadeRatePair.value = value;
}

void EG_NoiseBasedParticleEffectRenderer::setDepthTextureUnit(int unit)
{
    m_depthTexturePair.value = unit;
}

void EG_NoiseBasedParticleEffectRenderer::setSpriteTextureUnit(int unit)
{
    m_spriteTexturePair.value = unit;
}

void EG_NoiseBasedParticleEffectRenderer::setTime(float time)
{
    m_timePair.value = time;
}

void EG_NoiseBasedParticleEffectRenderer::setPointSize(float size)
{
    m_pointSizePair.value = size;
}

void EG_NoiseBasedParticleEffectRenderer::setInverseSize(glm::vec2 size)
{
    m_inverseSizePair.value = size;
}





void EG_NoiseBasedParticleEffectRenderer::setDepth3(float depth)
{
    m_depthPair3.value = depth;
}

void EG_NoiseBasedParticleEffectRenderer::setInverseSize3(glm::vec2 size)
{
    m_inverseSizePair3.value = size;
}

void EG_NoiseBasedParticleEffectRenderer::setBackgroundTextureUnit(int unit)
{
    m_backgroundTexturePair.value = unit;
}

void EG_NoiseBasedParticleEffectRenderer::setParticlesTextureUnit(int unit)
{
    m_particlesTexturePair.value = unit;
}


void EG_NoiseBasedParticleEffectRenderer::loadUniformLocations(pipeline& p, int pass)
{
    setAllDataPairUniLocs(pass);
    EG_Renderer::loadUniformLocations(p, pass);
}
