#include "EG_PointLight.h"

EG_PointLight::EG_PointLight()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    attenuation.constant = 0.0f;
    attenuation.linear = 0.0f;
    attenuation.exp = 0.0f;
}

EG_PointLight::EG_PointLight(   glm::vec3 Color, float AmbientIntensity, float DiffuseIntensity,
                                glm::vec3 Position, float Constant, float Linear, float Exp)
{
    color = Color;
    ambientIntensity = AmbientIntensity;
    diffuseIntensity = DiffuseIntensity;
    position = Position;
    attenuation.constant = Constant;
    attenuation.linear = Linear;
    attenuation.exp = Exp;
}
