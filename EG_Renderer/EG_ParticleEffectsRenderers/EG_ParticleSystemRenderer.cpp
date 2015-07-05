
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

/*
    initDataPairUniLoc(&m_deltaTimeDataPair,                m_shaders[RENDER_PASS1], RENDER_PASS1, "u_deltaTimeMillis");
    initDataPairUniLoc(&m_timeDataPair,                     m_shaders[RENDER_PASS1], RENDER_PASS1, "u_time");
    initDataPairUniLoc(&m_randomTexturePair,                m_shaders[RENDER_PASS1], RENDER_PASS1, "u_randomTexture");
    initDataPairUniLoc(&m_launchLifeTimeDataPair,           m_shaders[RENDER_PASS1], RENDER_PASS1, "u_launcherLifeTime");
    initDataPairUniLoc(&m_shellLifeTimeDataPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "u_shellLifeTime");
    initDataPairUniLoc(&m_secondaryShellLifeTimeDataPair,   m_shaders[RENDER_PASS1], RENDER_PASS1, "u_secondaryShellLifeTime");
*/
    initDataPairUniLoc(&m_deltaTimeMillisDataPair,          m_shaders[RENDER_PASS1], RENDER_PASS1, "gDeltaTimeMillis");
    initDataPairUniLoc(&m_randomTextureDataPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "gRandomTexture");
    initDataPairUniLoc(&m_timeDataPair,                     m_shaders[RENDER_PASS1], RENDER_PASS1, "gTime");
    initDataPairUniLoc(&m_launchLifeTimeDataPair,           m_shaders[RENDER_PASS1], RENDER_PASS1, "gLauncherLifetime");
    initDataPairUniLoc(&m_shellLifeTimeDataPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "gShellLifetime");
    initDataPairUniLoc(&m_secondaryShellLifeTimeDataPair,   m_shaders[RENDER_PASS1], RENDER_PASS1, "gSecondaryShellLifetime");

/*
    m_launcherLifetimeLocation = glGetUniformLocation( ParticleSystemShader->getProgramId(), "gLauncherLifetime");
    m_shellLifetimeLocation = glGetUniformLocation( ParticleSystemShader->getProgramId(), "gShellLifetime");
    m_secondaryShellLifetimeLocation = glGetUniformLocation( ParticleSystemShader->getProgramId(), "gSecondaryShellLifetime");
  */


    m_randomTextureDataPair.value = 3;
    m_launchLifeTimeDataPair.value = 100.0f;
    m_shellLifeTimeDataPair.value = 10000.0f;
    m_secondaryShellLifeTimeDataPair.value = 2500.0f;


/*
    m_shaders[RENDER_PASS2] = new Shader("EG_ParticleSystemShaders/EG_ParticleSystemRender.vs",
                                         "EG_ParticleSystemShaders/EG_ParticleSystemRender.gs",
                                         "EG_ParticleSystemShaders/EG_ParticleSystemRender.fs");

    initDataPairUniLoc(&m_deltaTimeDataPair,                m_shaders[RENDER_PASS1], RENDER_PASS1, "gDeltaTimeMillis");
    initDataPairUniLoc(&m_timeDataPair,                     m_shaders[RENDER_PASS1], RENDER_PASS1, "gTime");
    initDataPairUniLoc(&m_randomTexturePair,                m_shaders[RENDER_PASS1], RENDER_PASS1, "gRandomTexture");
    initDataPairUniLoc(&m_launchLifeTimeDataPair,           m_shaders[RENDER_PASS1], RENDER_PASS1, "gLauncherLifetime");
    initDataPairUniLoc(&m_shellLifeTimeDataPair,            m_shaders[RENDER_PASS1], RENDER_PASS1, "gShellLifetime");
    initDataPairUniLoc(&m_secondaryShellLifeTimeDataPair,   m_shaders[RENDER_PASS1], RENDER_PASS1, "gSecondaryShellLifetime");

*/


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
    /*
    int size = m_allDataPairs[pass].size();
    EG_Utility::debug("size is", size);
    for(int i=0; i<size; i++)
    {
        // EG_Utility::debug("value is", m_allDataPairs[pass][i]->value);
        m_allDataPairs[pass][i]->printValue();
    }
*/

    setAllDataPairUniLocs(pass);
    EG_Renderer::loadUniformLocations(p, pass);
}
