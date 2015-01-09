#ifndef EG_DIRECTIONAL_LIGHT_H_
#define EG_DIRECTIONAL_LIGHT_H_

#include "define.h"
#include "EG_BaseLight.h"

struct EG_DirectionalLight : public EG_BaseLight
{
    glm::vec3 direction;
    EG_DirectionalLight();
};

#endif // EG_LIGHT_H_
