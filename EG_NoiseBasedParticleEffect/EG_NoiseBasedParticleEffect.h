
#ifndef EG_NOISE_BASED_PARTICLE_EFFECT_H_
#define EG_NOISE_BASED_PARTICLE_EFFECT_H_


#include "EG_Utility.h"
#include "EG_NoiseBasedParticle.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>


struct EG_SurfacePod
{
    GLuint FBO;
    GLuint colorTexture;
    GLuint depthTexture;
};


class EG_NoiseBasedParticleEffect
{
    public:
        static EG_SurfacePod createSurface(int width, int height);

        void init(int width, int height, string tex);

        GLuint m_spriteTexture;
        EG_NoiseBasedParticleList m_particles;

        EG_SurfacePod m_backgroundSurface;
        EG_SurfacePod m_particleSurface;
};


#endif // EG_NOISE_BASED_COMMON_H



