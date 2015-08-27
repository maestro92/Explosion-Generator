#include "EG_GeneralRenderer.h"


EG_GeneralRenderer::EG_GeneralRenderer()
{}

EG_GeneralRenderer::~EG_GeneralRenderer()
{}

void EG_GeneralRenderer::init(int nShaders)
{}

void EG_GeneralRenderer::loadUniformLocations(pipeline& p, int pass)
{
    for ( auto local_it = tables[pass].begin(); local_it!= tables[pass].end(); ++local_it )
        (local_it->second)->setUniLoc();

    EG_Renderer::loadUniformLocations(p, pass);
}
