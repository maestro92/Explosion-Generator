#ifndef PARTICLE_SYSTEM_H
#define	PARTICLE_SYSTEM_H

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include "EG_Utility/EG_Utility.h"

#include <iostream>
#include <string>

#include "define.h"



using namespace std;

struct ParticlePS
{
    float Type;
    glm::vec3 Pos;
    glm::vec3 Vel;
    float LifetimeMillis;
};


class EG_ParticleSystem
{
public:
    EG_ParticleSystem();

    ~EG_ParticleSystem();

    bool InitParticleSystem(glm::vec3 Pos);
    bool Init_Texture(const std::string& TexFilename);


    GLuint m_randomTexture;
    GLuint PS_texture;
    int m_time;


    void UpdateParticles();
    void RenderParticles();
    void Render();

// private:
    bool m_isFirst;
    GLuint m_currVB;
    GLuint m_currTFB;
    GLuint m_particleBuffer[2];
    GLuint m_transformFeedback[2];

//    void UpdateParticles(int DeltaTimeMillis);
// void Render(int DeltaTimeMillis, const Matrix4f& VP, const Vector3f& CameraPos);
//     void RenderParticles(const Matrix4f& VP, const Vector3f& CameraPos);
};

#endif	/* PARTICLE_SYSTEM_H */

