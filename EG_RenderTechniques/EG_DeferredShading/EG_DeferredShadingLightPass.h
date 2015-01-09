#ifndef EG_DEFERRED_SHADING_LIGHT_PASS_H_
#define EG_DEFERRED_SHADING_LIGHT_PASS_H_

#include "EG_RenderTechnique.h"

class EG_DeferredShadingLightPass : public EG_RenderTechnique
{

    public:
        EG_DeferredShadingLightPass();
        ~EG_DeferredShadingLightPass();
        void init(int shaderCount);
        void initLightPass(int shaderId);
        void SetColorTextureUnit(unsigned int TextureUnit);
        void SetPositionTextureUnit(unsigned int TextureUnit);
        void SetNormalTextureUnit(unsigned int TextureUnit);
        void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
        void SetMatSpecularIntensity(float Intensity);
        void SetMatSpecularPower(float Power);
        void SetScreenSize(unsigned int Width, unsigned int Height);

//        void loadUniformLocations(pipeline& p, int RenderPassID);

    private:

        GLuint m_posTextureUnitLocation;
        GLuint m_normalTextureUnitLocation;
        GLuint m_colorTextureUnitLocation;
        GLuint m_eyeWorldPosLocation;
        GLuint m_matSpecularIntensityLocation;
        GLuint m_matSpecularPowerLocation;
        GLuint m_screenSizeLocation;
    protected:
};
#endif // EG_TECHNIQUE_DEFERRED_SHADING_H_
