#ifndef EG_POINT_LIGHT_H_
#define EG_POINT_LIGHT_H_

#include "define.h"

#include "EG_BaseLight.h"


struct Attenuation_t
{
    float constant;
    float linear;
    float exp;
};


struct EG_PointLight : public EG_BaseLight
{
    glm::vec3 position;
    Attenuation_t Attenuation;
    EG_PointLight();
};

#endif // EG_LIGHT_H_
