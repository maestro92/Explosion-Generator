#pragma once
// serves teh same purpose as #include guards, but with several advantages, including less code, avoidance of name clashes, and sometimes improved compile speed

// we assume all the particles will share the same texture
// for fire and smoke particle would require at least two different particle effects
// one for the fire and one for the smoke


#include "Particle.h"



#include <SOIL.h>
#include "camera.h"

#include "SDL\SDL.h"
#include "SDL\SDL_image.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#define NUM_PARTICLES 100
#include "Interpolator.h"

class Camera;
class ParticleEmitter;

/**
 * ParticleEffect class that demonstrates one possible implementation for
 * billboard particles.
 * The vertex definition will be used to render the particle effect to the GPU
 */

class ParticleEffect
{
public:

    // A vertex for the particle
    struct Vertex
    {
        Vertex()
            : m_Pos(0,10,0)
            , m_Diffuse(0)
            , m_Tex0(0)
        {}

        glm::vec3   m_Pos;      // Vertex position
        glm::vec4   m_Diffuse;  // Diffuse color
        glm::vec2   m_Tex0;     // Texture coordinate
    };
    typedef std::vector<Vertex> VertexBuffer;

	// interpolator is a typedef
	// here we have typedef of 4D vectors
    typedef Interpolator<glm::vec4> ColorInterpolator;

    ParticleEffect( unsigned int numParticles = NUM_PARTICLES );
    virtual ~ParticleEffect();

	// used to orient the particle facing the camera
    void SetCamera( Camera* pCamera );

	// this determines the position and velocity of newly emitted particles
    void SetParticleEmitter( ParticleEmitter* pEmitter );

	// used to specify a reference to an interpolator type
	// used to interpolate the color of the particle over it's lifetime
	// the author used it to make a "rainbow" effect
    void SetColorInterplator( const ColorInterpolator& colors );

    // Test method to randomize the particles in an interesting way
    void RandomizeParticles();
    void EmitParticles();

    virtual void Update( float fDeltaTime );
    virtual void Render();

    bool LoadTexture( const std::string& fileName );
    static unsigned int loadTexture(string filename, int background = 0, int generate = 0);
    static unsigned int loadTexture_Normal(string filename, int background = 0, int generate = 0);

    // Resize the particle buffer with numParticles
	// this adjustes the particle effect
    void Resize( unsigned int numParticles );

protected:
    void RandomizeParticle( Particle& particle );
    void EmitParticle( Particle& particle );
public:
    // Build the vertex buffer from the particle buffer
    void BuildVertexBuffer();
private:
    Camera*             m_pCamera;
    ParticleEmitter*    m_pParticleEmitter;
    ColorInterpolator   m_ColorInterpolator;

	// see Particle.h last line
	// this is just a typedef of a vector of particles
	// vector<Particle> ParticleBuffer;
    ParticleBuffer      m_Particles;


	// see Particle.h last line
	// this is just a typedef of a vector of vertex
	// vector<Vertex> VertexBuffer;
    VertexBuffer        m_VertexBuffer;
    glm::mat4x4         m_LocalToWorldMatrix;
    GLuint              m_TextureID;

    // Apply this force to every particle in the effect
	// to give it some motion
    glm::vec3           m_Force;
};
