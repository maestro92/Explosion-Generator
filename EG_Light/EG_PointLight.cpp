#include "EG_PointLight.h"

EG_PointLight::EG_PointLight()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    Attenuation.constant = 0.0f;
    Attenuation.linear = 0.0f;
    Attenuation.exp = 0.0f;
}
