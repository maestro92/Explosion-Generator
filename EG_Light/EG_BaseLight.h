#ifndef EG_BASE_LIGHT_H_
#define EG_BASE_LIGHT_H_

#include "define.h"

struct EG_BaseLight
{
    glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;

    EG_BaseLight();
};

#define COLOR_BLUE      glm::vec3(0.0f, 0.0f, 1.0f)
#define COLOR_GREEN     glm::vec3(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN      glm::vec3(0.0f, 1.0f, 1.0f)
#define COLOR_RED       glm::vec3(1.0f, 0.0f, 0.0f)
#define COLOR_PINK      glm::vec3(1.0f, 0.0f, 1.0f)
#define COLOR_YELLOW    glm::vec3(1.0f, 1.0f, 0.0f)
#define COLOR_WHITE     glm::vec3(1.0f, 1.0f, 1.0f)


const static glm::vec3 ColorList[] = {COLOR_BLUE, COLOR_GREEN, COLOR_CYAN, COLOR_RED, COLOR_PINK, COLOR_YELLOW, COLOR_WHITE };

#endif // EG_BASE_LIGHT_H_
