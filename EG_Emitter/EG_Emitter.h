#ifndef EG_EMITTER_H_
#define EG_EMITTER_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <list>



#include "EG_QuadModelABS.h"
#include "EG_Utility.h"
#include "pipeline.h"
#include "EG_EmitterParticle.h"



#include "EG_GeneralRenderer.h"

using namespace std;

class EG_Emitter
{

    public:
        EG_Emitter();
        ~EG_Emitter();

        void update(long time);

        void init();
        void initRenderer();

        void render(pipeline& m_pipeline, GLuint sceneDepthTexture);

    protected:
        void addParticle();

    public:
        //vector<EG_EmitterParticle*> m_particles;
        list<EG_EmitterParticle*> m_particles;
        vector<EG_EmitterParticle> m_vec_particles;


        GLuint m_particleSprite;

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


        EG_QuadModelABS m_quad;
        EG_GeneralRenderer m_renderer;

};


#endif // EG_EMITTER_H
