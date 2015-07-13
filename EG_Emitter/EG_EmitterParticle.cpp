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


void EG_EmitterParticle::rotate(float angle, float &x, float &y)
{
    float finalX = x * cos(angle) - y * sin(angle);
    float finalY = y * cos(angle) + x * sin(angle);

    x = finalX;
    y = finalY;
}


void EG_EmitterParticle::update(long time)
{
    if(m_active == false)
        return;

    if(m_prevTime == -1)
        m_prevTime = time;

    /// changing from millisecond to seconds
    float dt = (time - m_prevTime) / 1000.0f;

    m_velocity += m_acceleration * dt;
    m_position += m_velocity * dt;

    float x = m_position.x;
    float y = m_position.y;
    float z = m_position.z;

///######################################################
    /// Rotation around X
  //  rotate(m_rotation.x * time / 1000.0f, y, z);
    /// Rotation around Y
  //  rotate(m_rotation.y * time / 1000.0f, x, z);
    /// Rotation around Z
  //  rotate(m_rotation.z * time / 1000.0f, x, y);

    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
///######################################################





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

    float dlife = m_totalLife - m_life;

    /// as dlife -> 2.0, dlife / fadeInTime -> 1.0
    /// so this hits full alpha intensity
    if( dlife < fadeInTime)
    {
        /// 33:11
        /// glColor4f(color.x, color.y, color.z, (dlife/fadeInTime) * alpha )
        float temp = m_alpha;
        m_alpha = (dlife/fadeInTime) * temp;
    }

    /// when it's about to die in one second
    /// since life is below 1.0 and it's going towards zero
    /// we can use it for the color fading
    else if (m_life < 1.0f)
    {
        /// full color 33::11
        /// glColor4f(color.x, color.y, color.z, life * alpha)
        float temp = m_alpha;
        m_alpha = m_life * temp;
    }

    /// else it's just full regular alpha intensity
    else
    {
        /// glColor4f(color.x, color.y, color.z, alpha)

    }



    /// start decreasing the life
    m_life -= dt;

    if(m_life <= 0.0f)
        m_active = false;

    m_prevTime = time;

}
