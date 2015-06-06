#ifndef EG_EMITTER_H_
#define EG_EMITTER_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <list>

#include "EG_Utility.h"

#include "EG_EmitterParticle.h"

using namespace std;

class EG_Emitter
{

    public:
        EG_Emitter();
        ~EG_Emitter();

        void update(long time);
        void setTexture();
        void render();
    protected:
        void addParticle();

    public:
        //vector<EG_EmitterParticle*> m_particles;
        list<EG_EmitterParticle*> m_particles;

        GLuint m_textureID;

        long m_prevTime;
        long m_curTime;

        /// Attributes
        ///==========================
        float m_emissionRate;
        float m_emissionRadius;

        float m_life;
        float m_lifeRange;

        float m_size;
        float m_sizeRange;

        float m_saturation;
        float m_alpha;

        float m_spread;

        float m_gravity;

        glm::vec3 m_position;
        glm::vec3 m_wind;

        glm::vec3 m_rotation;
};


#endif // EG_EMITTER_H
