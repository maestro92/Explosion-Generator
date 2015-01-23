#ifndef EG_POINT_LIGHT_H_
#define EG_POINT_LIGHT_H_

#include "define.h"

#include "EG_BaseLight.h"


struct Attenuation
{
    float constant;
    float linear;
    float exp;
};


struct EG_PointLight : public EG_BaseLight
{
    glm::vec3 position;
    Attenuation attenuation;
    EG_PointLight();

    EG_PointLight(  glm::vec3 Color,
                    float AmbientIntensity,
                    float DiffuseIntensity,
                    glm::vec3 Position,
                    float Constant, float Linear, float Exp);
};

#endif // EG_LIGHT_H_
