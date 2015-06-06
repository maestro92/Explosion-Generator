#include "EG_Emitter.h"

EG_Emitter::EG_Emitter()
{
    ///
    /// 30 particles per second seems good
    m_prevTime = -1;
    m_textureID = -1;

    m_emissionRate = 30.0f;
    m_emissionRadius = 0.0f;

    m_life = 2.0f;
    m_lifeRange = 0.5f;

    m_size = 5.0f;
    m_sizeRange = 2.0f;

    m_saturation = 1.0f;
    m_alpha = 0.5;

    m_spread = 1.0f;
    m_gravity = 0.0f;

}

EG_Emitter::~EG_Emitter()
{
    for(list<EG_EmitterParticle*>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
        delete (*it);

 //   for(int i=0; i<m_particles.size(); i++)
 //       delete m_particles[i];

}

void EG_Emitter::update(long time)
{
    /// without texture, we leave immediately
    if(m_textureID == -1)
        return;

    if(m_prevTime == -1)
        m_prevTime = time;

    int numEmission = (int) ( (float) (time-m_prevTime) / 1000.0f * m_emissionRate );

    for (int i=0; i<numEmission; i++)
        addParticle();

    if(numEmission > 0)
        m_prevTime = time;

    m_curTime = time;
}

void EG_Emitter::setTexture()
{

}

void EG_Emitter::addParticle()
{
    EG_EmitterParticle* particle = new EG_EmitterParticle((int)m_particles.size());

    float r = EG_Utility::randFloat() * m_saturation + (1-m_saturation);
    float g = EG_Utility::randFloat() * m_saturation + (1-m_saturation);
    float b = EG_Utility::randFloat() * m_saturation + (1-m_saturation);

    particle->m_color = glm::vec3(r,g,b);

    particle->m_life = EG_Utility::randFloat(m_life - m_lifeRange, m_life + m_lifeRange);
    particle->m_totalLife = particle->m_life;

    float vx = EG_Utility::randFloat(-m_spread, m_spread);
    float vy = EG_Utility::randFloat(3,20);
    float vz = EG_Utility::randFloat(-m_spread, m_spread);
    particle->m_velocity = glm::vec3(vx, vy, vz);

    particle->m_acceleration = glm::vec3(0.0f, -m_gravity, 0.0f);

    particle->m_position.x = EG_Utility::randFloat(-m_emissionRadius, m_emissionRadius);
    particle->m_position.z = EG_Utility::randFloat(-m_emissionRadius, m_emissionRadius);

    m_particles.push_back(particle);
}


void EG_Emitter::render()
{
    for(list<EG_EmitterParticle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
    {
        EG_EmitterParticle* particle = (*it);
        particle->m_acceleration.y = -m_gravity;
        particle->m_acceleration += m_wind;
        particle->m_alpha = m_alpha;
        particle->m_rotation = m_rotation;

        particle->update(m_curTime);

        if(particle->m_active == false)
        {
            delete particle;
            list<EG_EmitterParticle*>::iterator pTemp = it--;
            m_particles.erase(pTemp);
        }

    }

    /*
    for(int i=0; i<m_particles.size(); i++)
    {
        EG_EmitterParticle* particle = m_particles[i];

        particle->m_acceleration.y = -m_gravity;
        particle->m_acceleration += m_wind;
        particle->m_alpha = m_alpha;
        particle->m_rotation = m_rotation;

        particle->update(m_curTime);

        if(m_particles[i])
    }
    */
}
