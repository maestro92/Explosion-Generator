
#include "EG_DeferredShadingStencilPass.h"

EG_DeferredShadingStencilPass::EG_DeferredShadingStencilPass()
{

}

EG_DeferredShadingStencilPass::~EG_DeferredShadingStencilPass()
{

}

void EG_DeferredShadingStencilPass::init(int shaderCount)
{
    allocateMemberVariables(shaderCount);
    m_shaders[RENDER_PASS1] = new Shader("/EG_DeferredShadingShaders/EG_DeferredShadingStencilPass.vs",
                                           "/EG_DeferredShadingShaders/EG_DeferredShadingStencilPass.fs");
    initMemberVariables();
}
