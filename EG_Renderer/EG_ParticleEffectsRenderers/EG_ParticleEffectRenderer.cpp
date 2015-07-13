#include "EG_ParticleEffectRenderer.h"


EG_ParticleEffectRenderer::EG_ParticleEffectRenderer()
{}

EG_ParticleEffectRenderer::~EG_ParticleEffectRenderer()
{}

void EG_ParticleEffectRenderer::init(int nShaders)
{}

void EG_ParticleEffectRenderer::loadUniformLocations(pipeline& p, int pass)
{
    for ( auto local_it = tables[pass].begin(); local_it!= tables[pass].end(); ++local_it )
        (local_it->second)->setUniLoc();

    EG_Renderer::loadUniformLocations(p, pass);
}
