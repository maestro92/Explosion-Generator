#include "EG_NoiseBasedParticleEffectRendererGPU.h"


EG_NoiseBasedParticleEffectRendererGPU::EG_NoiseBasedParticleEffectRendererGPU()
{

}

EG_NoiseBasedParticleEffectRendererGPU::~EG_NoiseBasedParticleEffectRendererGPU()
{

}

void EG_NoiseBasedParticleEffectRendererGPU::init(int nShaders)
{

}

void EG_NoiseBasedParticleEffectRendererGPU::init()
{
    Shader* s = new Shader( "EG_NoiseBasedParticleEffectShaders/EG_AdvectShader.vs", true);

    const GLchar* Varyings[3];
    Varyings[0] = "vPosition";
    Varyings[1] = "vBirthTime";
    Varyings[2] = "vVelocity";
    glTransformFeedbackVaryings(s->getProgramId(), 3, Varyings, GL_INTERLEAVED_ATTRIBS);
    s->linkShader();
    s->useShader();

    addShader(s);
    initDataPairUniLoc(&m_timePair,             RENDER_PASS1, "Time");
    initDataPairUniLoc(&m_sizeDataPair,         RENDER_PASS1, "Size");
    initDataPairUniLoc(&m_extentDataPair,       RENDER_PASS1, "Extent");


//     m_timePair

    s = new Shader( "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.vs",
                    "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.gs",
                    "EG_NoiseBasedParticleEffectShaders/EG_ParticleShaderGPU.fs");
    addShader(s);
  //  initDataPairUniLoc(&m_colorPair,                    RENDER_PASS2, "u_color");
  //  initDataPairUniLoc(&m_fadeRatePair,                 RENDER_PASS2, "u_fadeRate");
    initDataPairUniLoc(&m_depthTexturePair,             RENDER_PASS2, "u_depthTexture");
    initDataPairUniLoc(&m_spriteTexturePair,            RENDER_PASS2, "u_spriteTexture");
    initDataPairUniLoc(&m_timePair2,                    RENDER_PASS2, "u_time");
    initDataPairUniLoc(&m_pointSizePair,                RENDER_PASS2, "u_pointSize");
  //  initDataPairUniLoc(&m_inverseSizePair,              RENDER_PASS2, "u_inverseSize");

    initDataPairUniLoc(&m_MVMatrixDataPair,             RENDER_PASS2, "u_modelViewMat2");
    initDataPairUniLoc(&m_MVPMatrixDataPair,            RENDER_PASS2, "u_modelViewProjMat2");
    initDataPairUniLoc(&m_cameraPositionDataPair,       RENDER_PASS2, "u_cameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        RENDER_PASS2, "u_cameraViewDir");

    addDataPair(RENDER_PASS2, "u_color", DP_VEC4);
    addDataPair(RENDER_PASS2, "u_fadeRate", DP_FLOAT);
    addDataPair(RENDER_PASS2, "u_inverseSize", DP_VEC2);


    s = new Shader( "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.vs",
                    "EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.fs");
    addShader(s);
    initDataPairUniLoc(&m_inverseSizePair3,             RENDER_PASS3, "u_inverseSize");
    initDataPairUniLoc(&m_backgroundTexturePair,        RENDER_PASS3, "u_backgroundTexture");
    initDataPairUniLoc(&m_particlesTexturePair,         RENDER_PASS3, "u_particlesTexture");

 //   printTables();
}





void EG_NoiseBasedParticleEffectRendererGPU::setTime(float time)
{
    m_timePair.value = time;
}

void EG_NoiseBasedParticleEffectRendererGPU::setSize(glm::vec3 size)
{
    m_sizeDataPair.value = size;
}

void EG_NoiseBasedParticleEffectRendererGPU::setExtent(glm::vec3 extent)
{
    m_extentDataPair.value = extent;
}




void EG_NoiseBasedParticleEffectRendererGPU::setModelViewMatrix(glm::mat4 mvMat)
{
    m_MVMatrixDataPair.value = mvMat;
}

void EG_NoiseBasedParticleEffectRendererGPU::setModelViewProjectionMatrix(glm::mat4 mvpMat)
{
    m_MVPMatrixDataPair.value = mvpMat;
}

void EG_NoiseBasedParticleEffectRendererGPU::setCameraPosition(glm::vec3 camPos)
{
    m_cameraPositionDataPair.value = camPos;
}

void EG_NoiseBasedParticleEffectRendererGPU::setCameraViewDir(glm::vec3 dir)
{
    m_cameraViewDirDataPair.value = dir;
}






void EG_NoiseBasedParticleEffectRendererGPU::setColor(glm::vec4 color)
{
    m_colorPair.value = color;
}

void EG_NoiseBasedParticleEffectRendererGPU::setFadeRate(float value)
{
    m_fadeRatePair.value = value;
}

void EG_NoiseBasedParticleEffectRendererGPU::setDepthTextureUnit(int unit)
{
    m_depthTexturePair.value = unit;
}

void EG_NoiseBasedParticleEffectRendererGPU::setSpriteTextureUnit(int unit)
{
    m_spriteTexturePair.value = unit;
}

void EG_NoiseBasedParticleEffectRendererGPU::setTime2(float time)
{
    m_timePair2.value = time;
}

void EG_NoiseBasedParticleEffectRendererGPU::setPointSize(float size)
{
    m_pointSizePair.value = size;
}

void EG_NoiseBasedParticleEffectRendererGPU::setInverseSize(glm::vec2 size)
{
    m_inverseSizePair.value = size;
}






void EG_NoiseBasedParticleEffectRendererGPU::setInverseSize3(glm::vec2 size)
{
    m_inverseSizePair3.value = size;
}

void EG_NoiseBasedParticleEffectRendererGPU::setBackgroundTextureUnit(int unit)
{
    m_backgroundTexturePair.value = unit;
}

void EG_NoiseBasedParticleEffectRendererGPU::setParticlesTextureUnit(int unit)
{
    m_particlesTexturePair.value = unit;
}





void EG_NoiseBasedParticleEffectRendererGPU::loadUniformLocations(pipeline& p, int pass)
{
    setAllDataPairUniLocs(pass);



    for ( auto local_it = tables[pass].begin(); local_it!= tables[pass].end(); ++local_it )
        (local_it->second)->setUniLoc();
    //  std::cout << " " << local_it->first << ":" << local_it->second;
    //std::cout << std::endl;

  //  for(int i=0; i<size; i++)
  //      m_allDataPairs[pass][i]->setUniLoc();


    EG_Renderer::loadUniformLocations(p, pass);
}
