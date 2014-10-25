
#ifndef _MAIN_H_
#define _MAIN_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "define.h"

#include "shader.h"
#include "sceneLoader.h"
#include "TwoPass_RayCasting_Technique.h"


#include "pipeline.h"
#include "L_CubeEmitterEffect.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "ElapsedTime.h"

#include "ParticleEffect_Interface.h"

#include "Smoke.h"
#include "CollisionDetection_HGrid.h"
#include "t_camera.h"


using namespace std;


class ExplosionGenerator
{
    private:
        TwoPass_RayCasting_Technique TwoPass_Render;

        GLuint VBO;
        GLuint FBO;
        pipeline m_pipeline;

        // Material Property
        float Specular_Intensity;
        float Specular_Power;

    /*
        // Lighting
        DirectionalLight m_directionalLight;
        PointLight pl[2];
        SpotLight sl[1];
    */

      //  Lighting_Technique* m_LightingEffect;

        L_CubeEmitterEffect l_CubeEffect;
        Smoke smoke;

        t_camera cam;

        // models
        meshLoader* scene;
        meshLoader* ground;
        meshLoader* ground1;
        meshLoader* sphere;
        mesh* quad;

        bool running;
        bool dvel;
        bool addSmoke;

        /// textures
        unsigned int textureID;
        unsigned int groundTexture;
        unsigned int renderTexture;
        unsigned int depthTexture;
        unsigned int depth_TwoPassTexture;
        unsigned int shadow_depthTexture;
        GLuint shadowMapTexture;



        /// shaders
        shader* shadow_FirstRender;
        shader* shadow_SecondRender;
        shader* quadRenderShader;   // for rendering the texture into a quad, then displaying the quad into the screen under a orthographic view
        shader* Depth_CameraRender;


        /// Matrices
        glm::mat4 Light_ModelMatrix;
        glm::mat4 Light_ViewMatrix;
        glm::mat4 Light_ProjectionMatrix;
        glm::mat4 Light_ModelViewProjectionMatrix;
        glm::mat4 Light_BiasMatrix;

        glm::mat4 shadowMatrix;     // the matrix used to for conversion in the 2nd rendering pass
        //  convert Vertices in object space into texture coordinates in the light's perspective


        void GetLightPos_ModelView_Matrix();
        glm::mat4 LightPos_viewMatrix;
        glm::mat4 LightPos_modelMatrix;
        glm::mat4 LightPos_modelViewMatrix;




        // buttons
        bool g_bLeftMouseDown;
        bool g_bRightMouseDown;

        float angle;




    public:

        ExplosionGenerator();
        ~ExplosionGenerator();

        /// init functions
        void init_SDL_GLEW();
        void init_OpenGL();

        void init_Shader();
        void init_Models();
        void setupCamera();
        void setupColor_Texture();
        void init_Lighting();
        void init_Texture_and_FrameBuffer();

        void MotionGL();

        unsigned int loadTexture(string filename, int background = 0, bool generate=false);
        unsigned int createTexture(int w, int h, bool isDepth = false);


        void getDepthTexture_FromLightPosion();

        void start();
        void update();
        void show();
        void RenderTexture(GLuint TextureId);





        /// Basic Drawing functions
        void DrawAxis(float fScale, glm::vec3 translate = glm::vec3(0));
        void DrawAxis(float fScale, pipeline& m_pipeline, glm::vec3 translate = glm::vec3(0));
        void drawCube(float size);
        void drawCubeFrame(float size, int offset);
        void drawGround(float size, unsigned int textureId);


};





#endif
