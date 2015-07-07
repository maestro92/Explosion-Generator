
#include "EG_NoiseBasedParticleEffectRenderer.h"


EG_NoiseBasedParticleEffectRenderer::EG_NoiseBasedParticleEffectRenderer()
{

}

EG_NoiseBasedParticleEffectRenderer::~EG_NoiseBasedParticleEffectRenderer()
{

}

void EG_NoiseBasedParticleEffectRenderer::init(int nShaders)
{
    Shader* s = new Shader( "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.vs",
                            "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.gs",
                            "EG_NoiseBasedParticleEffectShaders/EG_ParticleShader.fs");
    addShader(s);
    initDataPairUniLoc(&m_colorPair,                    RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,                 RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,             RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,            RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,                     RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,                RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,              RENDER_PASS1, "u_inverseSize");

    initDataPairUniLoc(&m_MVMatrixDataPair,             RENDER_PASS1, "u_modelViewMat2");
    initDataPairUniLoc(&m_MVPMatrixDataPair,            RENDER_PASS1, "u_modelViewProjMat2");
    initDataPairUniLoc(&m_cameraPositionDataPair,       RENDER_PASS1, "u_cameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        RENDER_PASS1, "u_cameraViewDir");





    s = new Shader( "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.vs",
                    "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.fs");
    addShader(s);
    initDataPairUniLoc(&m_depthPair2,                   RENDER_PASS2, "u_depth");
  //  initDataPairUniLoc(&m_inverseSizePair2,             RENDER_PASS2, "u_inverseSize");
    initDataPairUniLoc(&m_backgroundTexturePair,        RENDER_PASS2, "u_backgroundTexture");
    initDataPairUniLoc(&m_particlesTexturePair,         RENDER_PASS2, "u_particlesTexture");




    s = new Shader( "EG_NoiseBasedParticleEffectShaders/EG_AdvectShader.vs", true);
    addShader(s);
    const GLchar* Varyings[3];
    Varyings[0] = "vPosition";
    Varyings[1] = "vBirthTime";
    Varyings[2] = "vVelocity";
    glTransformFeedbackVaryings(m_shaders[RENDER_PASS3]->getProgramId(), 3, Varyings, GL_INTERLEAVED_ATTRIBS);
    m_shaders[RENDER_PASS3]->linkShader();
    m_shaders[RENDER_PASS3]->useShader();

    initDataPairUniLoc(&m_timePair3,            RENDER_PASS3, "Time");
    initDataPairUniLoc(&m_sizeDataPair,         RENDER_PASS3, "Size");
    initDataPairUniLoc(&m_extentDataPair,       RENDER_PASS3, "Extent");


/*
    s = new Shader( "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.vs",
                    "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.gs",
                    "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.fs");
    addShader(s);
    initDataPairUniLoc(&m_depthPair2,                   RENDER_PASS4, "u_depth");
    initDataPairUniLoc(&m_inverseSizePair2,             RENDER_PASS4, "u_inverseSize");
    initDataPairUniLoc(&m_backgroundTexturePair,        RENDER_PASS4, "u_backgroundTexture");
    initDataPairUniLoc(&m_particlesTexturePair,         RENDER_PASS4, "u_particlesTexture");
*/

/*
    m_shaders[RENDER_PASS4] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.gs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.fs");
    initDataPairUniLoc(&m_colorPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,         m_shaders[RENDER_PASS1], RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,     m_shaders[RENDER_PASS1], RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,    m_shaders[RENDER_PASS1], RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,      m_shaders[RENDER_PASS1], RENDER_PASS1, "u_inverseSize");

    initDataPairUniLoc(&m_MVMatrixDataPair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_modelViewMat2");
    initDataPairUniLoc(&m_MVPMatrixDataPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_modelViewProjMat2");
    initDataPairUniLoc(&m_cameraPositionDataPair,       m_shaders[RENDER_PASS1], RENDER_PASS1, "u_cameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_cameraViewDir");
*/

#if 0
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
/*
    initDataPairUniLoc(&m_colorPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,         m_shaders[RENDER_PASS1], RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,     m_shaders[RENDER_PASS1], RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,    m_shaders[RENDER_PASS1], RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,      m_shaders[RENDER_PASS1], RENDER_PASS1, "u_inverseSize");
*/

    initDataPairUniLoc(&m_colorPair,                    RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,                 RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,             RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,            RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,                     RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,                RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,              RENDER_PASS1, "u_inverseSize");


    initDataPairUniLoc(&m_MVMatrixDataPair,             RENDER_PASS1, "u_modelViewMat2");
    initDataPairUniLoc(&m_MVPMatrixDataPair,            RENDER_PASS1, "u_modelViewProjMat2");
    initDataPairUniLoc(&m_cameraPositionDataPair,       RENDER_PASS1, "u_cameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        RENDER_PASS1, "u_cameraViewDir");



    m_shaders[RENDER_PASS2] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.fs");
    initDataPairUniLoc(&m_depthPair2,                   RENDER_PASS2, "u_depth");
    initDataPairUniLoc(&m_inverseSizePair2,             RENDER_PASS2, "u_inverseSize");
    initDataPairUniLoc(&m_backgroundTexturePair,        RENDER_PASS2, "u_backgroundTexture");
    initDataPairUniLoc(&m_particlesTexturePair,         RENDER_PASS2, "u_particlesTexture");



    m_shaders[RENDER_PASS3] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_AdvectShader.vs", true);
    const GLchar* Varyings[3];
    Varyings[0] = "vPosition";
    Varyings[1] = "vBirthTime";
    Varyings[2] = "vVelocity";
    glTransformFeedbackVaryings(m_shaders[RENDER_PASS3]->getProgramId(), 3, Varyings, GL_INTERLEAVED_ATTRIBS);
    m_shaders[RENDER_PASS3]->linkShader();
    m_shaders[RENDER_PASS3]->useShader();

    initDataPairUniLoc(&m_timePair3,            RENDER_PASS3, "Time");
    initDataPairUniLoc(&m_sizeDataPair,         RENDER_PASS3, "Size");
    initDataPairUniLoc(&m_extentDataPair,       RENDER_PASS3, "Extent");

    getAllMatrixUniLocs();


/*
    m_shaders[RENDER_PASS4] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.gs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.fs");
    initDataPairUniLoc(&m_colorPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,         m_shaders[RENDER_PASS1], RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,     m_shaders[RENDER_PASS1], RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,    m_shaders[RENDER_PASS1], RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,      m_shaders[RENDER_PASS1], RENDER_PASS1, "u_inverseSize");

    initDataPairUniLoc(&m_MVMatrixDataPair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_modelViewMat2");
    initDataPairUniLoc(&m_MVPMatrixDataPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_modelViewProjMat2");
    initDataPairUniLoc(&m_cameraPositionDataPair,       m_shaders[RENDER_PASS1], RENDER_PASS1, "u_cameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_cameraViewDir");
*/
#endif
}




#if 0
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
/*
    initDataPairUniLoc(&m_colorPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,         m_shaders[RENDER_PASS1], RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,     m_shaders[RENDER_PASS1], RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,    m_shaders[RENDER_PASS1], RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,      m_shaders[RENDER_PASS1], RENDER_PASS1, "u_inverseSize");
*/

    initDataPairUniLoc(&m_colorPair,                    RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,                 RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,             RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,            RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,                     RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,                RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,              RENDER_PASS1, "u_inverseSize");


    initDataPairUniLoc(&m_MVMatrixDataPair,             RENDER_PASS1, "u_modelViewMat2");
    initDataPairUniLoc(&m_MVPMatrixDataPair,            RENDER_PASS1, "u_modelViewProjMat2");
    initDataPairUniLoc(&m_cameraPositionDataPair,       RENDER_PASS1, "u_cameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        RENDER_PASS1, "u_cameraViewDir");



    m_shaders[RENDER_PASS2] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.fs");
    initDataPairUniLoc(&m_depthPair2,                   RENDER_PASS2, "u_depth");
    initDataPairUniLoc(&m_inverseSizePair2,             RENDER_PASS2, "u_inverseSize");
    initDataPairUniLoc(&m_backgroundTexturePair,        RENDER_PASS2, "u_backgroundTexture");
    initDataPairUniLoc(&m_particlesTexturePair,         RENDER_PASS2, "u_particlesTexture");



    m_shaders[RENDER_PASS3] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_AdvectShader.vs", true);
    const GLchar* Varyings[3];
    Varyings[0] = "vPosition";
    Varyings[1] = "vBirthTime";
    Varyings[2] = "vVelocity";
    glTransformFeedbackVaryings(m_shaders[RENDER_PASS3]->getProgramId(), 3, Varyings, GL_INTERLEAVED_ATTRIBS);
    m_shaders[RENDER_PASS3]->linkShader();
    m_shaders[RENDER_PASS3]->useShader();

    initDataPairUniLoc(&m_timePair3,            RENDER_PASS3, "Time");
    initDataPairUniLoc(&m_sizeDataPair,         RENDER_PASS3, "Size");
    initDataPairUniLoc(&m_extentDataPair,       RENDER_PASS3, "Extent");

    getAllMatrixUniLocs();


/*
    m_shaders[RENDER_PASS4] = new Shader("EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.vs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.gs",
                                         "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.fs");
    initDataPairUniLoc(&m_colorPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_color");
    initDataPairUniLoc(&m_fadeRatePair,         m_shaders[RENDER_PASS1], RENDER_PASS1, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,     m_shaders[RENDER_PASS1], RENDER_PASS1, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,    m_shaders[RENDER_PASS1], RENDER_PASS1, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_pointSizePair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_pointSize");
    initDataPairUniLoc(&m_inverseSizePair,      m_shaders[RENDER_PASS1], RENDER_PASS1, "u_inverseSize");

    initDataPairUniLoc(&m_MVMatrixDataPair,             m_shaders[RENDER_PASS1], RENDER_PASS1, "u_modelViewMat2");
    initDataPairUniLoc(&m_MVPMatrixDataPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_modelViewProjMat2");
    initDataPairUniLoc(&m_cameraPositionDataPair,       m_shaders[RENDER_PASS1], RENDER_PASS1, "u_cameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        m_shaders[RENDER_PASS1], RENDER_PASS1, "u_cameraViewDir");
*/
}
#endif


void EG_NoiseBasedParticleEffectRenderer::setModelViewMatrix(glm::mat4 mvMat)
{
    m_MVMatrixDataPair.value = mvMat;
}

void EG_NoiseBasedParticleEffectRenderer::setModelViewProjectionMatrix(glm::mat4 mvpMat)
{
    m_MVPMatrixDataPair.value = mvpMat;
}

void EG_NoiseBasedParticleEffectRenderer::setCameraPosition(glm::vec3 camPos)
{
    m_cameraPositionDataPair.value = camPos;
}

void EG_NoiseBasedParticleEffectRenderer::setCameraViewDir(glm::vec3 dir)
{
    m_cameraViewDirDataPair.value = dir;
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





void EG_NoiseBasedParticleEffectRenderer::setDepth2(float depth)
{
    m_depthPair2.value = depth;
}

void EG_NoiseBasedParticleEffectRenderer::setInverseSize2(glm::vec2 size)
{
    m_inverseSizePair2.value = size;
}

void EG_NoiseBasedParticleEffectRenderer::setBackgroundTextureUnit(int unit)
{
    m_backgroundTexturePair.value = unit;
}

void EG_NoiseBasedParticleEffectRenderer::setParticlesTextureUnit(int unit)
{
    m_particlesTexturePair.value = unit;
}






void EG_NoiseBasedParticleEffectRenderer::setTime3(float time)
{
    m_timePair3.value = time;
}

void EG_NoiseBasedParticleEffectRenderer::setSize(glm::vec3 size)
{
    m_sizeDataPair.value = size;
}

void EG_NoiseBasedParticleEffectRenderer::setExtent(glm::vec3 extent)
{
    m_extentDataPair.value = extent;
}


void EG_NoiseBasedParticleEffectRenderer::loadUniformLocations(pipeline& p, int pass)
{
    setAllDataPairUniLocs(pass);
    EG_Renderer::loadUniformLocations(p, pass);
}
