
#include "EG_ParticleSystemRenderer.h"


EG_ParticleSystemRenderer::EG_ParticleSystemRenderer()
{

}

EG_ParticleSystemRenderer::~EG_ParticleSystemRenderer()
{

}

void EG_ParticleSystemRenderer::init(int nShaders)
{
    allocateMemberVariables(nShaders);


    const GLchar* Varyings[4];
    Varyings[0] = "Type1";
    Varyings[1] = "Position1";
    Varyings[2] = "Velocity1";
    Varyings[3] = "Age1";


    m_shaders[RENDER_PASS1] = new Shader("EG_ParticleSystemShaders/EG_ParticleSystemUpdate.vs",
                                         "EG_ParticleSystemShaders/EG_ParticleSystemUpdate.gs",
                                         "EG_ParticleSystemShaders/EG_ParticleSystemUpdate.fs", true);

    glTransformFeedbackVaryings(m_shaders[RENDER_PASS1]->getProgramId(), 4, Varyings, GL_INTERLEAVED_ATTRIBS);

    m_shaders[RENDER_PASS1]->linkShader();
    m_shaders[RENDER_PASS1]->useShader();


    initDataPairUniLoc(&m_deltaTimeMillisDataPair,          RENDER_PASS1, "gDeltaTimeMillis");
    initDataPairUniLoc(&m_randomTextureDataPair,            RENDER_PASS1, "gRandomTexture");
    initDataPairUniLoc(&m_timeDataPair,                     RENDER_PASS1, "gTime");
    initDataPairUniLoc(&m_launchLifeTimeDataPair,           RENDER_PASS1, "gLauncherLifetime");
    initDataPairUniLoc(&m_shellLifeTimeDataPair,            RENDER_PASS1, "gShellLifetime");
    initDataPairUniLoc(&m_secondaryShellLifeTimeDataPair,   RENDER_PASS1, "gSecondaryShellLifetime");


    m_randomTextureDataPair.value = 3;
    m_launchLifeTimeDataPair.value = 100.0f;
    m_shellLifeTimeDataPair.value = 10000.0f;
    m_secondaryShellLifeTimeDataPair.value = 2500.0f;


    getAllMatrixUniLocs();
}

void EG_ParticleSystemRenderer::setDeltaTime(float value)
{
    m_deltaTimeMillisDataPair.value = value;
}

void EG_ParticleSystemRenderer::setTime(float value)
{
    m_timeDataPair.value = value;
 //   EG_Utility::debug("time is", (float)value);
 //   glUniform1f(m_timeDataPair.uniLoc, (float)value);

}




void EG_ParticleSystemRenderer::loadUniformLocations(pipeline& p, int pass)
{
    setAllDataPairUniLocs(pass);
    EG_Renderer::loadUniformLocations(p, pass);
}
