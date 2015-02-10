#ifndef EG_TECHNIQUE_SHADOW_RENDER_H
#define EG_TECHNIQUE_SHADOW_RENDER_H

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "EG_RenderTechnique.h"
#include "EG_DirectionalLight.h"
#include "EG_PointLight.h"

/// LightPOV    render1
/// CameraPOV   render2
class Technique_Shadow_Render : public EG_RenderTechnique
{

        /// first pass for depth in Light's point of View
        /// Second pass in Camera's point of View


    public:

        GLuint ShadowMatrix_UniLoc[2];

        Technique_Shadow_Render();
        ~Technique_Shadow_Render();

        GLuint l_modelViewProjectionMatrix_UniLoc_;

        GLuint LightMVPmatrix_UniLoc;
        GLuint shadowMap_UniLoc;
        GLuint LightPosition_MVmatrix_UniLoc;
        GLuint LightPosition_UniLoc;
        GLuint CameraPosition_UniLoc;

        GLuint gNumPointLights;
        GLuint m_eyeWorldPosLocation;
        GLuint m_matSpecularIntensityLocation;
        GLuint m_matSpecularPowerLocation;

      //  void init(int w, int h);
        void init(int w, int h, int Shader_Num);
        void loadUniformLocations(pipeline& p, int RenderPassID);

        void setDirectionalLight(EG_DirectionalLight& Light);
        void setPointLight(int index, EG_PointLight& Light);
        void setPointLightCount(int count);
        float computePointLightBoundingSphere(EG_PointLight& Light);


        void setEyeWorldPos(const glm::vec3& EyePos);
        void setScreenSize(unsigned int Width, unsigned int Height);
        void setMatSpecularIntensity(float Intensity);
        void setMatSpecularPower(float Power);





        GLuint ShadowMap;
        GLuint DepthTexture_LightPOV;

        int ShadowMapWidth;
        int ShadowMapHeight;



        // Shaders
        Shader* FirstPass_LightPOV;
        Shader* SecondPass_CameraPOV;


        struct {
            GLuint color;
            GLuint ambientIntensity;
            GLuint diffuseIntensity;
            GLuint direction;
        } m_dirLightLocation;


        struct
        {
            GLuint color;
            GLuint ambientIntensity;
            GLuint diffuseIntensity;
            GLuint position;
            Attenuation attenuation;
            GLuint scale;
        } m_pointLightsLocation[103];

    //    m_pointLightUniformLocation m_pointLightUniformLocations[103];
};

#endif
