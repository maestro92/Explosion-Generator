
#ifndef _MAIN_H_
#define _MAIN_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>



#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "ElapsedTime.h"
#include "define.h"

#include "ParticleEffect_Interface.h"
#include "CubeEmitterEffect.h"
#include "L_CubeEmitterEffect.h"
#include "Smoke.h"
#include "CollisionDetection_HGrid.h"

using namespace std;





class ExplosionGenerator
{


    L_CubeEmitterEffect l_CubeEffect;
    CubeEmitterEffect e_CubeEffect;
    Smoke smoke;


    bool running;
    bool dvel;
    bool addSmoke;

    // textures
    unsigned int textureID;


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
        void setup_CollisionDetection_HGrid();


        void DrawAxis(float fScale, glm::vec3 translate = glm::vec3(0));

        void MotionGL();



        void update();
        void show();
        void start();

        void drawCube(float size);
        void drawCubeFrame(float size, int offset);
        void drawGround(float size, unsigned int textureId);

        unsigned int loadTexture(string filename, int background = 0, bool generate=false);
};





#endif
