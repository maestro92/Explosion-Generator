#ifndef EG_VERTEX_DATA
#define EG_VERTEX_DATA

#include "define.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

struct EG_VertexData
{
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec3 m_color;
    glm::vec2 m_UV;
};

struct EG_TextureData
{
    GLuint m_id;
};


#endif // EG_VERTEX_DATA
