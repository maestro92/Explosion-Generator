#ifndef EG_DEFERRED_SHADING_LIGHT_PASS_H_
#define EG_DEFERRED_SHADING_LIGHT_PASS_H_

#include "EG_Renderer.h"

class EG_DeferredShadingLightPass : public EG_Renderer
{

    public:
        EG_DeferredShadingLightPass();
        ~EG_DeferredShadingLightPass();
 //       void init(int shaderCount);
        void init(int nShaders);

        void initLightPass(int shaderId);
        void setColorTextureUnit(unsigned int TextureUnit);
        void setPositionTextureUnit(unsigned int TextureUnit);
        void setNormalTextureUnit(unsigned int TextureUnit);
        void setStencilTextureUnit(unsigned int TextureUnit);

        void setEyeWorldPos(const glm::vec3& EyeWorldPos);
        void setMatSpecularIntensity(float Intensity);
        void setMatSpecularPower(float Power);
        void setScreenSize(unsigned int Width, unsigned int Height);

//        void loadUniformLocations(pipeline& p, int RenderPassID);

    private:

        GLuint m_posTextureUnitLocation;
        GLuint m_normalTextureUnitLocation;
        GLuint m_colorTextureUnitLocation;
        GLuint m_stencilTextureUnitLocation;
        GLuint m_eyeWorldPosLocation;
        GLuint m_matSpecularIntensityLocation;
        GLuint m_matSpecularPowerLocation;
        GLuint m_screenSizeLocation;
    protected:
};
#endif // EG_TECHNIQUE_DEFERRED_SHADING_H_
