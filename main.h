#ifndef _MAIN_H_
#define _MAIN_H_
#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "ElapsedTime.h"
#include "define.h"

#include "ParticleEmitter.h"
#include "CubeEmitter.h"
#include "Smoke.h"
/*

http://www.gamedev.net/page/resources/_/creative/visual-arts/make-a-particle-explosion-effect-r2701



*/

class ParticleEffect;


class ExplosionGenerator
{


    CubeEmitter e_CubeEmitter;
    Smoke smoke;


    bool running;
    bool dvel;
    bool addSmoke;


    // Shaders
    unsigned int VertexShader;
    unsigned int FragmentShader;
    unsigned int ShaderObjectID;




    // buttons
    bool g_bLeftMouseDown;
    bool g_bRightMouseDown;


    public:


        ExplosionGenerator();
        ~ExplosionGenerator();

        void initSDL_GLEW();
        void initOpenGL();

        // Shaders
        void initShader(const char* vname, const char*fname);
        void loadFile(const char* fn, string & str);
        unsigned int loadShader(string& source, unsigned int shaderType);
        void FreeShader();


        void setupCamera();
        void setupColor_Texture();
        void setupParticleEmitter();
        void init_Lighting();

        void DrawAxis(float fScale, glm::vec3 translate = glm::vec3(0));

        void MotionGL();



        void update();
        void show();
        void start();

        void drawCube(float size);
        void drawCubeFrame(float size, int offset);
};


#endif
