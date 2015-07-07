#ifndef EG_PARTICLE_SYSTEM_EFFECT_H_
#define EG_PARTICLE_SYSTEM_EFFECT_H_

#include <stdio.h>
#include "EG_Utility.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "EG_Renderer.h"
#include "pipeline.h"

#include <stdio.h>      /* printf */
#include <math.h>       /* sqrt */


#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f


using namespace std;

struct EG_ParticleSystemParticle
{
    float type;
    glm::vec3 position;
    glm::vec3 velocity;
    float lifeTimeMillis;
};

class EG_ParticleSystemEffect
{
    public:
        EG_ParticleSystemEffect();
        ~EG_ParticleSystemEffect();



        GLuint m_randomTexture;
        GLuint m_particleTexture;

        int m_time;

        bool m_isFirst;
        GLuint m_currVB;
        GLuint m_currTFB;
        GLuint m_particleBuffers[2];
        GLuint m_transformFeedbackObjects[2];

        void init();
        void update();
        void render();
        void render(pipeline& p, EG_Renderer* Renderer, int pass);

        void initCurlNoiseStyle();
        void updateCurlNoiseStyle();
        void renderCurlNoiseStyle();
};

#endif // EG_PARTICLE_SYSTEM_EFFECT_H_
