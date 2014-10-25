#ifndef LIGHTING_H
#define LIGHTING_H
#include "pipeline.h"
#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include "define.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>


using namespace std;

struct BaseLight
{
    glm::vec3 Color;
    glm::mat4 MV_Matrix;
    float AmbientIntensity;
    float DiffuseIntensity;

    // constructor
    BaseLight()
    {
        Color = glm::vec3(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};


struct DirectionalLight : public BaseLight
{
    glm::vec3 Direction;

    DirectionalLight()
    {
        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
        Color = glm::vec3(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};



struct PointLight : public BaseLight
{
    glm::vec3 Position;

    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

    PointLight()
    {
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }
};

struct SpotLight : public BaseLight
{
    glm::vec3 Direction;
    glm::vec3 Position;
    float Cutoff;
    glm::mat3 Normal_Matrix;
    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

    SpotLight()
    {
        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
        Cutoff = 0.0f;
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }
};


class Lighting_Technique
{

    public:
        static const unsigned int MAX_POINT_LIGHTS = 2;
        static const unsigned int MAX_SPOT_LIGHTS = 2;

    Lighting_Technique();

    void Init(unsigned int shaderID);

    void LoadingModeViewMatrix_ForPointLights(pipeline& m_pipeline, PointLight* pLights, glm::mat4 View_Matrix, unsigned int index);
    void LoadingModeViewMatrix_ForSpotLights(pipeline& m_pipeline, SpotLight* pLights, glm::mat4 View_Matrix, unsigned int index);

    void SetDirectionalLight(const DirectionalLight& Light);
    void SetPointLights(unsigned int NumLights, const PointLight* pLights);
    void SetSpotLights(unsigned int NumLights, const SpotLight* pLights);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);


    private:




        GLuint m_matSpecularIntensityLocation;
        GLuint m_matSpecularPowerLocation;

        GLuint m_numPointLightsLocation;
        GLuint m_numSpotLightsLocation;

        GLuint nice;

        struct
        {
            GLuint Color;
            GLuint AmbientIntensity;
            GLuint DiffuseIntensity;
            GLuint Direction;
        } m_dirLightLocation;



        struct
        {
            GLuint Color;
            GLuint AmbientIntensity;
            GLuint DiffuseIntensity;
            GLuint Position;
            GLuint MV_MatrixLocation;
            struct
            {
                GLuint Constant;
                GLuint Linear;
                GLuint Exp;
            } Atten;
        } m_pointLightsLocation[MAX_POINT_LIGHTS];



        struct
        {
            GLuint Color;
            GLuint AmbientIntensity;
            GLuint DiffuseIntensity;
            GLuint Position;
            GLuint Direction;
            GLuint Cutoff;
            GLuint MV_MatrixLocation;
            GLuint Normal_MatrixLocation;
            struct
            {
                GLuint Constant;
                GLuint Linear;
                GLuint Exp;
            } Atten;
        } m_spotLightsLocation[MAX_SPOT_LIGHTS];




};

#endif // LIGHTING_H
