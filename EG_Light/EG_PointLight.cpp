#include "EG_PointLight.h"

EG_PointLight::EG_PointLight()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    attenuation.constant = 0.0f;
    attenuation.linear = 0.0f;
    attenuation.exp = 0.0f;
}
