#ifndef EG_DEFERRED_SHADING_REFLECTION_H_
#define EG_DEFERRED_SHADING_REFLECTION_H_

#include "EG_DeferredShading.h"

class EG_DeferredShadingReflection : public EG_DeferredShading
{
    public:
        EG_DeferredShadingReflection();
        ~EG_DeferredShadingReflection();

        void init(int shaderCount);
        GLuint m_cubeMapTextureUnitLocation;
        GLuint m_eyeWorldPosUniformLocation;
        void setCubeMapTextureUnit(unsigned int TextureUnit);
        void setEyeWorldPos(const glm::vec3& EyePos);
};


#endif // EG_DEFERRED_SHADING_REFLECTION_H_
