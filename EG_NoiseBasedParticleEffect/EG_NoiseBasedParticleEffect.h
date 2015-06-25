
#ifndef EG_NOISE_BASED_PARTICLE_EFFECT_H_
#define EG_NOISE_BASED_PARTICLE_EFFECT_H_

#include "EG_Math.h"
#include "EG_Noise.h"
#include "EG_Utility.h"
#include "EG_NoiseBasedParticle.h"

#include "EG_QuadModelABS.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <stdio.h>      /* printf */
#include <math.h>       /* sqrt */

using namespace std;

struct EG_SurfacePod
{
    GLuint FBO;
    GLuint colorTexture;
    GLuint depthTexture;
};


class EG_NoiseBasedParticleEffect
{
    public:
        float Time;

        EG_NoiseBasedParticleEffect();
        ~EG_NoiseBasedParticleEffect();

        EG_SurfacePod createSurface(int width, int height);
        void init(int width, int height);
        void update(float dt, float timeStep);
        glm::vec3 computeGradient(glm::vec3 p);
        glm::vec3 computeGradient2(glm::vec3 p);

        glm::vec3 computeCurl(glm::vec3 p);
        glm::vec3 computeCurl2(glm::vec3 p);

        float sampleDistance(glm::vec3 p);
        float sampleDistance2(glm::vec3 p);

        glm::vec3 samplePotential(glm::vec3 p);
        glm::vec3 samplePotential2(glm::vec3 p);

        glm::vec3 getPerlinNoise3D(glm::vec3 src);
        bool ZOrderPredicate(const EG_NoiseBasedParticle& p0, const EG_NoiseBasedParticle& p1);
        void seedParticles(float dt);
/*
        static EG_SurfacePod createSurface(int width, int height);
        void init(int width, int height);
        void update(float dt, float timeStep);
        static glm::vec3 computeGradient(glm::vec3 p);
        static glm::vec3 computeCurl(glm::vec3 p);
        static float sampleDistance(glm::vec3 p);
        static glm::vec3 samplePotential(glm::vec3 p);

        static glm::vec3 getPerlinNoise3D(glm::vec3 src);
        static void seedParticles(float dt);
*/

        glm::vec3 m_sphereCenter;
        float m_sphereRadius;

        EG_QuadModelABS m_screenQuad;

        GLuint m_backgroundTexture;
        GLuint m_spriteTexture;
        EG_NoiseBasedParticleList m_particles;

        EG_SurfacePod m_backgroundSurface;
        EG_SurfacePod m_particleSurface;
};


#endif // EG_NOISE_BASED_COMMON_H



