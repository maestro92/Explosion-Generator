#pragma once
#include <glm/glm.hpp>
#include <cstdlib>
#include <vector>

using namespace glm;
struct Particle
{
    vec3    m_InitPosition;
    vec3    m_Position;
    vec3    m_Velocity;
  //  vec3    m_RotVelocity;
    vec4    m_Color;

    float   m_fRotate;
    float   m_fRadius;
    // float   m_fSize;
    float   m_fAge;
    float   m_fLifeTime;




    Particle()
        : m_InitPosition(0,0,0)
        , m_Position(0,0,0)     // Center point of particle
        , m_Velocity(0,0,0)
        , m_Color(0)
        , m_fRadius(1)
        , m_fRotate(0)
        , m_fAge(0)
        , m_fLifeTime(0)
    {}

};
typedef std::vector<Particle> ParticleBuffer;


