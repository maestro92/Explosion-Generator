#ifndef EG_DEFERRED_SHADING_SKYBOX_H_
#define EG_DEFERRED_SHADING_SKYBOX_H_

// #include "EG_DeferredShadingLightPass.h"

#include "EG_DeferredShading.h"

// class EG_DeferredShadingSkybox : public EG_DeferredShadingLightPass
class EG_DeferredShadingSkybox : public EG_DeferredShading

{
    public:
        EG_DeferredShadingSkybox();
        ~EG_DeferredShadingSkybox();

        void init(int shaderCount);
        GLuint m_cubeMapTextureUnitLocation;
        void setCubeMapTextureUnit(unsigned int TextureUnit);
//        void setStencilFlag(glm::vec3 stencilFlag);
    //    void loadUniformLocations(pipeline& p, int RenderPassID);

//        glm::vec3 m_stencilFlag;

//        GLuint m_stencilFlagUniformLocation;
};


#endif // EG_DEFERRED_SHADING_STENCIL_PASS_H_
