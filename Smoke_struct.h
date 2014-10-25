#ifndef _SMOKE_STRUCT_H_
#define _SMOKE_STRUCT_H_

#include "define.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>


struct SurfacePod {
    GLuint FboHandle;
    GLuint ColorTexture;
    GLsizei Width;
    GLsizei Height;
    GLsizei Depth;
};

struct Surfaces_t{
    SurfacePod Divergence;
    SurfacePod Obstacles;
    SurfacePod HiresObstacles;
};

struct SlabPod {
    SurfacePod Ping;
    SurfacePod Pong;
};

struct Slabs_t{
    SlabPod Velocity;
    SlabPod Density;
    SlabPod Pressure;
    SlabPod Temperature;
};

static struct Matrices_t{
    glm::mat4 Projection;
    glm::mat4 Modelview;
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 ViewNoRotate;
    glm::mat4 ModelviewProjection;
} Matrices;



static struct Vbos_t{
    GLuint CubeCenter;
    GLuint FullscreenQuad;
} Vbos;
#endif
