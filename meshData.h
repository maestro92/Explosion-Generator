#ifndef MESHDATA_H_
#define MESHDATA_H_

#include "define.h"

struct vertexData
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 color;
    float U,V;  // we want the UV coordinates
};


struct textureData
{
    unsigned int id;
    unsigned int type;
};
#endif

