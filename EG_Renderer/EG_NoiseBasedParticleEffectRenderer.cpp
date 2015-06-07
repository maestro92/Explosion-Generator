
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
    m_shaders[RENDER_PASS1] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.gs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.fs");
/*
    m_colorPair.uniLoc  = getUniLoc(m_shaders[RENDER_PASS1], "gColor");
    m_fadeRatePair      = getUniLoc(m_shaders[RENDER_PASS1], "gFadeRate");
    m_timePair          = getUniLoc(m_shaders[RENDER_PASS1], "gTime");
    m_inverseSizePair   = getUniLoc(m_shaders[RENDER_PASS1], "gInverseSize");
*/
    initDataPairUniLoc(&m_colorPair,            m_shaders[RENDER_PASS1], "gColor");
    initDataPairUniLoc(&m_fadeRatePair,         m_shaders[RENDER_PASS1], "gFadeRate");
    initDataPairUniLoc(&m_depthTexturePair,     m_shaders[RENDER_PASS1], "gDepthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,    m_shaders[RENDER_PASS1], "gSpriteTexture");
    initDataPairUniLoc(&m_timePair,             m_shaders[RENDER_PASS1], "gTime");
    initDataPairUniLoc(&m_pointSizePair,        m_shaders[RENDER_PASS1], "gPointSize");
    initDataPairUniLoc(&m_inverseSizePair,      m_shaders[RENDER_PASS1], "gInverseSize");

    m_shaders[RENDER_PASS2] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.fs");

    getAllMatrixUniLocs();
}


void EG_NoiseBasedParticleEffectRenderer::loadUniformLocations(pipeline& p, int pass)
{

}
