#include "EG_DeferredShadingGeometryPass.h"

EG_DeferredShadingGeometryPass::EG_DeferredShadingGeometryPass()
{

}

EG_DeferredShadingGeometryPass::~EG_DeferredShadingGeometryPass()
{

}

void EG_DeferredShadingGeometryPass::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);
    progShaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingGeometryPass.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingGeometryPass.fs");
    initMemberVariables();
}
