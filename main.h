
#ifndef _MAIN_H_
#define _MAIN_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "define.h"

#include "shader.h"
#include "Lighting_Technique.h"
#include "PivotCamera.h"
#include "sceneLoader.h"

#include "pipeline.h"
#include "L_CubeEmitterEffect.h"
#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"




#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "ElapsedTime.h"


#include "ParticleEffect_Interface.h"
#include "CubeEmitterEffect.h"

#include "Smoke.h"
#include "CollisionDetection_HGrid.h"
#include "t_camera.h"
using namespace std;




class ExplosionGenerator
{

    GLuint VBO;
    GLuint FBO;
    pipeline m_pipeline;

    // Material Property
    float Specular_Intensity;
    float Specular_Power;


    // Lighting
    DirectionalLight m_directionalLight;
    PointLight pl[2];
    SpotLight sl[1];


    Lighting_Technique* m_LightingEffect;



    L_CubeEmitterEffect l_CubeEffect;
    CubeEmitterEffect e_CubeEffect;
    Smoke smoke;

    t_camera cam;

    // models
    meshLoader* scene;
    meshLoader* ground;
    meshLoader* ground1;
    meshLoader* sphere;
    meshLoader* monkeyMesh;
    mesh* quad;


    bool running;
    bool dvel;
    bool addSmoke;

    // textures
    unsigned int textureID;
    unsigned int groundTexture;
    unsigned int renderTexture;
    unsigned int depthTexture;
    unsigned int shadow_depthTexture;




    shader* ObjShader;
    shader* shadow_FirstRender;
    shader* shadow_SecondRender;
    shader* quadRenderShader;   // for rendering the texture into a quad, then displaying the quad into the screen under a orthographic view



    glm::mat4 Light_ModelMatrix;
    glm::mat4 Light_ViewMatrix;
    glm::mat4 Light_ProjectionMatrix;
    glm::mat4 Light_ModelViewProjectionMatrix;
    glm::mat4 Light_BiasMatrix;

    glm::mat4 shadowMatrix;     // the matrix used to for conversion in the 2nd rendering pass
    //  convert Vertices in object space into texture coordinates in the light's perspective




    // buttons
    bool g_bLeftMouseDown;
    bool g_bRightMouseDown;

    float angle;
    float field_length;
    public:


        ExplosionGenerator();
        ~ExplosionGenerator();

        void init_SDL_GLEW();
        void init_OpenGL();


        unsigned int createTexture(int w, int h, bool isDepth = false);

        void init_Shader();
        void init_Models();
        void FreeShader();

        void setupCamera();
        void setupColor_Texture();
        void setupParticleEmitter();
        void init_Lighting();
        void init_shadows();
        void init_FrameBuffer();
        void init_Texture_and_FrameBuffer();
        void setup_CollisionDetection_HGrid();


        void DrawAxis(float fScale, glm::vec3 translate = glm::vec3(0));
        void DrawAxis(float fScale, pipeline& m_pipeline, glm::vec3 translate = glm::vec3(0));

        void MotionGL();



        void update();
        void show();
        void show1();
        void shadow_show();
        void start();

        void PointLight_show();
        void SpotLight_show();

/// for Shadowmapping
        void shadow_getDepthTexture_FromLightPosion();
        void getDepthTexture_FromLightPosion();
        void updateShadowMatrix(unsigned int shaderId);


        void shadow_initOpenGL();
        void shadow_ProjectionMatrices();
        void shadow_ModelViewMatrices();
        void shadow_createTexture();


        void shadow_Render();
        void reshape();

        void drawCube(float size);
        void drawCubeFrame(float size, int offset);
        void drawGround(float size, unsigned int textureId);

        unsigned int loadTexture(string filename, int background = 0, bool generate=false);
};





#endif
