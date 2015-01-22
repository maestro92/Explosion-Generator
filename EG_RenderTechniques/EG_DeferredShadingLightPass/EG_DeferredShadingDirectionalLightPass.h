#ifndef EG_DEFERRED_SHADING_DIRECTIONAL_LIGHT_PASS_H_
#define EG_DEFERRED_SHADING_DIRECTIONAL_LIGHT_PASS_H_

#include "EG_DirectionalLight.h"
#include "EG_DeferredShadingLightPass.h"


class EG_DeferredShadingDirectionalLightPass : public EG_DeferredShadingLightPass
{
    public:

        GLuint l_modelViewProjectionMatrix_UniLoc_;
        GLuint lightPosition_ModelViewMatrix_UniLoc_;
        GLuint positionMap_UniLoc_;
        GLuint screenSize_UniLoc_;
        GLuint shadowMap_UniLoc_;
        GLuint colorMap_UniLoc_;
        GLuint normalMap_UniLoc_;
        GLuint m_colorTextureUnitLocation_;
     //

        EG_DeferredShadingDirectionalLightPass();
        ~EG_DeferredShadingDirectionalLightPass();

        void init(int shaderCount);
        void init(int shaderCount, char* vsSource, char* fsSource);
        void setDirectionalLight(EG_DirectionalLight& Light);
        void loadUniformLocations(pipeline& p, int RenderPassID);

        GLuint lightPosition_ObjectSpace_UniLoc_;

    private:

        struct {
            GLuint color;
            GLuint ambientIntensity;
            GLuint diffuseIntensity;
            GLuint direction;
        } m_dirLightLocation;
};
#endif // EG_DEFERRED_SHADING_LIGHT_PASS_H_



