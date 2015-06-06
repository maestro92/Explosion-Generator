#include "EG_EmitterParticle.h"

EG_EmitterParticle::EG_EmitterParticle(int id)
{
    m_id = id;
    m_totalLife = 1.0f;    m_life = 1.0f;

    m_alpha = 1.0f;
    m_size = 1.0f;

    m_bounciness = 0.9f;

    m_active = true;
    m_prevTime = -1;
}


void EG_EmitterParticle::update(long time)
{
    if(m_active == false)
        return;

    if(m_prevTime == -1)
        m_prevTime = time;

    float change = (time - m_prevTime) / 1000.0f;

    m_velocity += m_acceleration * change;
    m_position += m_velocity * change;

    /// collision with the plane
    if( m_position.y < 0.0f )
    {
        m_velocity.y = m_velocity.y * -m_bounciness;
        m_position.y = 0.0f;
    }

    const float fadeInTime = 0.2f;

    /// when it just came into existence
    /// and it hasn't even lived for 0.2 seconds yet
    /// we allow them to fade in
    if( m_totalLife - m_life < fadeInTime)
    {
        /// 33:11
    }

    /// when it's about to die
    else if (m_life < 1.0f)
    {
        /// full color 33::11
    }

    /// else it's just full alpha intensity
    else
    {

    }



}
