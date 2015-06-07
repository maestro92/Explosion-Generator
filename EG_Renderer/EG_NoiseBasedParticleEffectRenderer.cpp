
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
    m_shaders[RENDER_PASS1] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_BlitShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_BlitShader.gs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_BlitShader.fs");

    m_depthPair;
    m_scrollOffsetTimePair;
    initDataPairUniLoc(&m_depthPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "gDepth");
    initDataPairUniLoc(&m_scrollOffsetTimePair, m_shaders[RENDER_PASS1], RENDER_PASS1, "gScrollOffsetTime");



    m_shaders[RENDER_PASS2] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.gs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.fs");

    initDataPairUniLoc(&m_colorPair,            m_shaders[RENDER_PASS2], RENDER_PASS2, "gColor");
    initDataPairUniLoc(&m_fadeRatePair,         m_shaders[RENDER_PASS2], RENDER_PASS2, "gFadeRate");
    initDataPairUniLoc(&m_depthTexturePair,     m_shaders[RENDER_PASS2], RENDER_PASS2, "gDepthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,    m_shaders[RENDER_PASS2], RENDER_PASS2, "gSpriteTexture");
    initDataPairUniLoc(&m_timePair,             m_shaders[RENDER_PASS2], RENDER_PASS2, "gTime");
    initDataPairUniLoc(&m_pointSizePair,        m_shaders[RENDER_PASS2], RENDER_PASS2, "gPointSize");
    initDataPairUniLoc(&m_inverseSizePair,      m_shaders[RENDER_PASS2], RENDER_PASS2, "gInverseSize");

    m_shaders[RENDER_PASS3] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.fs");
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

void EG_NoiseBasedParticleEffectRenderer::loadUniformLocations(pipeline& p, int pass)
{
    setAllDataPairUniLocs(pass);
    EG_Renderer::loadUniformLocations(p, pass);
}
