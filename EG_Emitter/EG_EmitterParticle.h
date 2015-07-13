#ifndef EG_EMITTER_PARTICLE_H_
#define EG_EMITTER_PARTICLE_H_


#include "define.h"



class EG_EmitterParticle
{
    public:
        EG_EmitterParticle(int id);
        void update(long time);
        void rotate(float angle, float &x, float &y);

        void render();

        int m_id;
        glm::vec3 m_color;
        glm::vec3 m_position;
        glm::vec3 m_velocity;
        glm::vec3 m_acceleration;
        glm::vec3 m_rotation;

        float m_prevTime;
        float m_curTime;

        float m_totalLife;
        float m_life;

        float m_alpha;
        float m_size;

        float m_bounciness;
        bool m_active;


};

#endif // EG_EMITTER_PARTICLE_H_
