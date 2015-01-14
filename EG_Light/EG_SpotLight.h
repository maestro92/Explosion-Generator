#ifndef EG_SPOT_LIGHT_H_
#define EG_SPOT_LIGHT_H_

#include "define.h"
#include "EG_BaseLight.h"

#include "EG_PointLight.h"



struct EG_SpotLight : public EG_BaseLight
{
    glm::vec3 direction;
    float cutoff;
    glm::vec3 position;

    Attenuation attenuation;

    EG_SpotLight();
};


#endif // EG_LIGHT_H_
