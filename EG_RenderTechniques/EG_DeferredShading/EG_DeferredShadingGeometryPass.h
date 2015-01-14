#ifndef EG_DEFERRED_SHADING_GEOMETRY_PASS_H_
#define EG_DEFERRED_SHADING_GEOMETRY_PASS_H_

#include "EG_RenderTechnique.h"

class EG_DeferredShadingGeometryPass : public EG_RenderTechnique
{
    public:
        EG_DeferredShadingGeometryPass();
        ~EG_DeferredShadingGeometryPass();

        void init(int shaderCount);
};
#endif
