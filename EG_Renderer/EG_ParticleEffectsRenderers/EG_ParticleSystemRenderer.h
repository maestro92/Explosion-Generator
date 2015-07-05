#ifndef EG_PARTICLE_SYSTEM_RENDERER_H_
#define EG_PARTICLE_SYSTEM_RENDERER_H_


#include "EG_Renderer.h"

using namespace std;

class EG_ParticleSystemRenderer : public EG_Renderer
{
    public:
        EG_ParticleSystemRenderer();
        ~EG_ParticleSystemRenderer();
        void init(int nShaders);

        /// Advection Shader
        FloatDataPair m_deltaTimeMillisDataPair;
        FloatDataPair m_timeDataPair;
        IntDataPair m_randomTextureDataPair;
        FloatDataPair m_launchLifeTimeDataPair;
        FloatDataPair m_shellLifeTimeDataPair;
        FloatDataPair m_secondaryShellLifeTimeDataPair;


/*
GLuint m_deltaTimeMillisLocation;
GLuint m_timeLocation;
GLuint m_randomTextureLocation;
GLuint m_launcherLifetimeLocation;
GLuint m_shellLifetimeLocation;
GLuint m_secondaryShellLifetimeLocation;
*/


        void setDeltaTime(float value);
        void setTime(float value);
        void setRandomTextureUnit(int unit);
        void setLaunchLifeTime(float value);
        void setShellLifeTime(float value);
        void setSecondaryShellLifeTime(float value);

        virtual void loadUniformLocations(pipeline& p, int pass);
};


#endif // EG_NOISE_BASED_PARTICLE_EFFECT_RENDERER_H_
