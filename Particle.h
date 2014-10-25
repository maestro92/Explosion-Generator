#pragma once
#include "glm/glm.hpp"
#include <cstdlib>
#include <stdint.h>
#include <vector>

using namespace glm;


typedef union {

 // for some reason unsigned long int doesn't work

 //   unsigned long int value;
	uint64_t value;
	struct {
		short int x;
		short int y;
		short int z;
		unsigned short int level;
	} data;
	unsigned char c[8];
} hash_t;



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



/// specifically designed for hgrid implementation
struct h_Particle
{
    h_Particle *pPrevParticle;
    h_Particle *pNextParticle;      // embedded link to next hgrid particle


    vec3    m_Position;
    vec3    m_Pre_Velocity;
    vec3    m_Velocity;

  //  vec3    m_RotVelocity;
    vec4    m_Color;

    float   m_fRotate;
    float   m_fRadius;
    float   m_fmass;
    int                 m_id;
 //   uint64_t            m_hash;                         // hash bucket
    hash_t              m_hash;


    // float   m_fSize;
    float   m_fAge;
    float   m_fLifeTime;
    int     state;

    h_Particle()
        : m_Position(0,0,0)     // Center point of particle
        , m_Pre_Velocity(0,1000,0)
        , m_Velocity(0,0,0)

        , m_Color(0)
        , m_fRadius(1)
        , m_fRotate(0)
        , m_fAge(0)
        , m_fLifeTime(0)
        , m_fmass(1)
        , m_id(0)
    {
        pPrevParticle = NULL;
        pNextParticle = NULL;

    }
};

typedef std::vector<h_Particle> h_ParticleBuffer;

