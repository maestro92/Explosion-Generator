#ifndef EG_DEFERRED_SHADING_GEOMETRY_PASS_H_
#define EG_DEFERRED_SHADING_GEOMETRY_PASS_H_

#include "EG_Renderer.h"

class EG_DeferredShadingGeometryPass : public EG_Renderer
{
    public:
        EG_DeferredShadingGeometryPass();
        ~EG_DeferredShadingGeometryPass();

        void init(int shaderCount);

        GLuint m_stencilFlagUniformLocation;
        glm::vec3 m_stencilFlag;
        void setStencilFlag(glm::vec3 stencilFlag);
        void loadUniformLocations(pipeline& p, int RenderPassID);
};
#endif
