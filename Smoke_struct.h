#ifndef _SMOKE_STRUCT_H_
#define _SMOKE_STRUCT_H_

#include "define.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>



struct Advect3D_Location_t
{
    GLuint InverseSize;
    GLuint TimeStep;
    GLuint Dissipation;
    GLuint SourceTexture;
    GLuint Obstacles;
};


struct Jacobi3D_Location_t
{
    GLuint Alpha;
    GLuint InverseBeta;
    GLuint Divergence;
    GLuint Obstacles;
};


struct SubtractGradient3D_Location_t
{
    GLuint GradientScale;
    GLuint HalfInverseCellSize;
    GLuint Pressure;
    GLuint Obstacles;
};


struct ApplyImpulse3D_Location_t
{
    GLuint Point;
    GLuint Radius;
    GLuint FillColor;
};

struct ApplyBuoyancy3D_Location_t
{
    GLuint Temperature;
    GLuint Density;
    GLuint AmbientTemperature;
    GLuint TimeStep;
    GLuint Sigma;
    GLuint Kappa;
};


struct ComputeDivergence3D_Location_t
{
    GLuint HalfInverseCellSize;
    GLuint Obstacles;
};






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




static struct Vbos_t{
    GLuint CubeCenter;
    GLuint FullscreenQuad;
} Vbos;
#endif
