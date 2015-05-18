#ifndef _SMOKE_H_
#define _SMOKE_H_

#include "EG_Shader.h"

#include "EG_Technique_TwoPass_RayCasting.h"
#include "Smoke_struct.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include "EG_ParticleEffect.h"
#include "define.h"

#include "EG_WorldObject.h"


const int ViewportWidth = 320;
const int ViewportHeight = ViewportWidth;

const int GridWidth = 32;
const int GridHeight = GridWidth;
const int GridDepth = GridWidth;




const float CellSize = 1.25f;
//const float CellSize = 0.01f;
const float SplatRadius = GridWidth / 8.0f;
const float AmbientTemperature = 0.0f;
const float ImpulseTemperature = 10.0f;
const float ImpulseDensity = 1.0f;
const int NumJacobiIterations = 10;
const float TimeStep = 0.25f;
const float SmokeBuoyancy = 0.5f;
const float SmokeWeight = 0.0125f;
const float GradientScale = 1.125f / CellSize;
/*
const float TemperatureDissipation = 0.99f;
const float VelocityDissipation = 0.99f;
const float DensityDissipation = 0.9995f;
*/
const float ImpulseVelocity = 10.0f;

const float TemperatureDissipation = 0.95f;
const float VelocityDissipation = 0.95f;
const float DensityDissipation = 0.95f;


//glm::vec3 ImpulsePosition( GridWidth / 2.0f, GridHeight - (int) SplatRadius / 2.0f, GridDepth / 2.0f);


using namespace std;

/*
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
*/



// class Smoke //: public L_ParticleEffect_Interface
class Smoke : public WorldObject
{
    public:
        // side size
//        Technique_TwoPass_RayCasting TwoPass_Render;

        Advect3D_Location_t Advect3D_Location;
        Jacobi3D_Location_t Jacobi3D_Location;
        SubtractGradient3D_Location_t SubtractGradient3D_Location;
        ApplyImpulse3D_Location_t ApplyImpulse3D_Location;
        ApplyBuoyancy3D_Location_t ApplyBuoyancy3D_Location;
        ComputeDivergence3D_Location_t ComputeDivergence3D_Location;


        Vbos_t myVbos;
        Slabs_t f_Slab;
        Surfaces_t f_Surfaces;
        Smoke();
        ~Smoke();

        void init();


        void update(bool toggle = false);
        void show(bool toggle = false);
        void Reset();


        GLuint getUniform(Shader* s, const char* name);
        GLuint getUniform(GLuint programID, const char* name);

        void setUniform(GLuint location, int value);
        void setUniform(GLuint location, float value);
        void setUniform(GLuint location, float x, float y);
        void setUniform(GLuint location, glm::vec3 value);
        void setUniform(GLuint location, glm::mat4 value);
/*
        void SetUniform(GLuint programID, const char* name, int value);
        void SetUniform(GLuint programID, const char* name, float value);
        void SetUniform(GLuint programID, const char* name, float x, float y);
        void SetUniform(GLuint progarmID, const char* name, glm::vec3 value);
        void SetUniform(GLuint programID, const char* name, glm::mat4 value);
*/

        void Advect3D_SameFBO(SurfacePod velocity, SurfacePod source, SurfacePod obstacles, SurfacePod dest, float dissipation, GLuint p);
        void Jacobi3D_SameFBO(SurfacePod pressure, SurfacePod divergence, SurfacePod obstacles, SurfacePod dest, GLuint p);
        void SubtractGradient3D_SameFBO(SurfacePod velocity, SurfacePod pressure, SurfacePod obstacles, SurfacePod dest, GLuint p);
        void ApplyBuoyancy3D_SameFBO(SurfacePod velocity, SurfacePod temperature, SurfacePod density, SurfacePod dest, GLuint p);
        void ComputeDivergence3D_SameFBO(SurfacePod velocity, SurfacePod obstacles, SurfacePod dest, GLuint p);
        void ApplyImpulse3D_SameFBO(SurfacePod dest, glm::vec3 position, float value, GLuint p);

        GLuint CreatePointVbo(float x, float y, float z);
        GLuint CreateQuadVbo();
        SlabPod CreateSlab3D(GLsizei width, GLsizei height, GLsizei depth, int numComponents);
        SurfacePod CreateVolume(GLsizei width, GLsizei height, GLsizei depth, int numComponents);

        SlabPod CreateSlab3D_SameFBO(GLsizei width, GLsizei height, GLsizei depth, int numComponents);
        SurfacePod CreateVolume_SameFBO(GLsizei width, GLsizei height, GLsizei depth, int numComponents);
        void ClearSurface_SameFBO(GLuint tex_id, float v);
        void CreateObstacles_SameFBO(SurfacePod dest);
        void ResetState_SameFBO();

        void CreateObstacles(SurfacePod dest);
        void ClearSurface(SurfacePod s, float v);
        void SwapSurfaces(SlabPod* slab);
        void ResetState();

        GLuint CreatePyroclasticVolume(int n, float r);

    private:
        GLuint fbo;

        Shader* Eulerian3D_Advect;
        Shader* Eulerian3D_Fill;

        Shader* Eulerian3D_Jacobi;
        Shader* Eulerian3D_SubtractGradient;
        Shader* Eulerian3D_ComputeDivergence;
        Shader* Eulerian3D_ApplyImpulse;
        Shader* Eulerian3D_ApplyBuoyancy;


};









#endif
