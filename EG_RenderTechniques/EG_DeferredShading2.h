#ifndef EG_DEFERRED_SHADING_2_H_
#define EG_DEFERRED_SHADING_2_H_

#include "EG_RenderTechnique.h"
#include "EG_DirectionalLight.h"
#include "EG_PointLight.h"

enum DeferredShadingPass
{
    GEOMETRY_PASS1,
    STENCIL_PASS2,
    LIGHT_PASS3,
    POINTLIGHT_PASS4,
    DIRECTIONAL_PASS5,
    FINAL_PASS6
};




struct LightPassUniformLocation
{
    /// WorldViewProjectionMatrix
    GLuint m_WVPLocation;
    GLuint m_posTextureUnitLocation;
    GLuint m_normalTextureUnitLocation;
    GLuint m_colorTextureUnitLocation;
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_screenSizeLocation;
};

struct PointLightUniformLocation{
    LightPassUniformLocation lightPass;
    GLuint color;
    GLuint ambientIntensity;
    GLuint diffuseIntensity;
    GLuint position;
    struct {
        GLuint constant;
        GLuint linear;
        GLuint exp;
    } Atten;
};

struct DirectionalLightUniformLocation{
    LightPassUniformLocation lightPass;
    GLuint color;
    GLuint ambientIntensity;
    GLuint diffuseIntensity;
    GLuint direction;
};




class EG_DeferredShading2 : public EG_RenderTechnique
{
    public:

        GLuint m_colorTextureUnitLocation_;

        GLuint l_modelViewProjectionMatrix_UniLoc_;
        GLuint lightPosition_ModelViewMatrix_UniLoc_;
        GLuint positionMap_UniLoc_;
        GLuint screenSize_UniLoc_;

        GLuint shadowMap_UniLoc_;
        GLuint colorMap_UniLoc_;
        GLuint normalMap_UniLoc_;

/// stack size linker option
/// Project->Build Options->Linker Settings->Other Linker Options
/// -Wl,--stack,52800000


        GLuint lightPosition_ObjectSpace_UniLoc_;

        PointLightUniformLocation pointLightUniformLocation;
        DirectionalLightUniformLocation directionalLightPassUniformLocation;


    public:
        EG_DeferredShading2();
        ~EG_DeferredShading2();
        void init(int shaderCount);
        void SetColorTextureUnit(unsigned int TextureUnit);
        void loadUniformLocations(pipeline& p, int RenderPassID);

        void setDirectionalLight(EG_DirectionalLight& Light);
        void setPointLight(EG_PointLight& Light);

        void setPositionTextureUnit(GLuint uniformLocation, unsigned int TextureUnit);
        void setColorTextureUnit(GLuint uniformLocation, unsigned int TextureUnit);
        void setNormalTextureUnit(GLuint uniformLocation, unsigned int TextureUnit);
        void setScreenSize(GLuint uniformLocation, unsigned int Width, unsigned int Height);




/*
    //    GLuint cameraPosition_WorldSpace_UniLoc;
        glm::mat4 beautiful2;
        glm::mat4 beautiful3;
        glm::mat4 beautiful4;
        glm::mat4 beautiful5;
        glm::mat4 beautiful6;
        glm::mat4 beautiful17;

        glm::mat4 beautiful21;
        glm::mat4 beautiful31;
        glm::mat4 beautiful41;
        glm::mat4 beautiful51;
        glm::mat4 beautiful112;
        glm::mat4 beautiful1132;

        glm::mat4 beautiful22222;
        glm::mat4 beautiful333333;
        glm::mat4 beautiful44444;
        glm::mat4 beautiful555555;
*/
/*
        GLuint LightMVPmatrix_UniLoc;
        GLuint shadowMap_UniLoc;
        GLuint LightPosition_MVmatrix_UniLoc;
        GLuint LightPosition_UniLoc;
        GLuint CameraPosition_UniLoc;
*/

    private:

    protected:
};
#endif // EG_TECHNIQUE_DEFERRED_SHADING_H_
