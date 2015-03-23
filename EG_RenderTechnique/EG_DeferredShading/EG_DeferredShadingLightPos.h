#ifndef EG_DEFERRED_SHADING_LIGHT_POS_H_
#define EG_DEFERRED_SHADING_LIGHT_POS_H_

#include "EG_DeferredShading.h"

class EG_DeferredShadingLightPos : public EG_DeferredShading
{
    public:
        EG_DeferredShadingLightPos();
        ~EG_DeferredShadingLightPos();

        GLuint m_lightColor;

        void init(int shaderCount);
        void setLightColor(glm::vec3 color);
};


#endif // EG_DEFERRED_SHADING_STENCIL_PASS_H_
