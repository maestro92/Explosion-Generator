#ifndef EG_DEFERRED_SHADING_STENCIL_PASS_H_
#define EG_DEFERRED_SHADING_STENCIL_PASS_H_

#include "EG_Renderer.h"

class EG_DeferredShadingStencilPass : public EG_Renderer
{
    public:
        EG_DeferredShadingStencilPass();
        ~EG_DeferredShadingStencilPass();

        void init(int shaderCount);
};


#endif // EG_DEFERRED_SHADING_STENCIL_PASS_H_
