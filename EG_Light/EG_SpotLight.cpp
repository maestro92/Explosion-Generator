#include "EG_SpotLight.h"


EG_SpotLight::EG_SpotLight()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    direction = glm::vec3(0.0f, 0.0f, 0.0f);
    cutoff = 0.0f;
    Attenuation.constant = 0.0f;
    Attenuation.linear = 0.0f;
    Attenuation.exp = 0.0f;

}
