#ifndef _MAIN_H_
#define _MAIN_H_
#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include "ParticleEffect.h"
#include "SphereEmitter.h"
#include "ElapsedTime.h"
#include "define.h"


class ParticleEffect;


class ExplosionGenerator
{



    bool running;



    // buttons
    bool g_bLeftMouseDown;
    bool g_bRightMouseDown;


    SphereEmitter g_ParticleEmitter;
    ParticleEffect g_ParticleEffect;
    ParticleEffect::ColorInterpolator Fire_colors;



    public:


        ExplosionGenerator();


        void init();
        void initOpenGL();
        void setupCamera();
        void setupColor_Texture();

        void DrawAxis(float fScale, glm::vec3 translate = glm::vec3(0));

        void MotionGL();


        void update();
        void show();
        void start();



};


#endif
