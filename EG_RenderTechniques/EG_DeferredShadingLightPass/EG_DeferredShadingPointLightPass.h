#ifndef EG_DEFERRED_SHADING_POINT_LIGHT_PASS_H_
#define EG_DEFERRED_SHADING_POINT_LIGHT_PASS_H_

#include "EG_PointLight.h"
#include "EG_DeferredShadingLightPass.h"

class EG_DeferredShadingPointLightPass : public EG_DeferredShadingLightPass
{
    public:

        EG_DeferredShadingPointLightPass();
        ~EG_DeferredShadingPointLightPass();

        void init(int shaderCount);
        void init(int shaderCount, char* vsSource, char* fsSource);

        void setPointLight(EG_PointLight& Light);
        float computePointLightBoundingSphere(EG_PointLight& Light);
        void loadUniformLocations(pipeline& p, int RenderPassID);

/*
        GLuint l_modelViewProjectionMatrix_UniLoc_;
        GLuint lightPosition_ModelViewMatrix_UniLoc_;
        GLuint lightPosition_ObjectSpace_UniLoc_;
*/

    private:


        struct Attenuation
        {
            GLuint constant;
            GLuint linear;
            GLuint exp;
        } ;

        struct
        {
            GLuint color;
            GLuint ambientIntensity;
            GLuint diffuseIntensity;
            GLuint position;
            Attenuation attenuation;
        } m_pointLightUniformLocation;
};
#endif // EG_DEFERRED_SHADING_LIGHT_PASS_H_



