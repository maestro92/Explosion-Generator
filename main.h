
#ifndef _MAIN_H_
#define _MAIN_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "define.h"

#include "EG_Camera.h"
#include "ThirdPersonPOV_camera.h"
#include "EG_utility.h"
#include "EG_RenderTechniques/EG_Skybox.h"
#include "shader.h"
#include "sceneLoader.h"
#include "EG_Technique_DepthTexture_Render.h"
#include "EG_Technique_TwoPass_RayCasting.h"
#include "EG_Technique_Shadow_Render.h"
#include "EG_Technique_Reflection.h"

#include "WorldBox.h"
#include "WorldSphere.h"

#include "pipeline.h"
#include "L_SphereParticleEffect.h"
#include "L_Cube_Sphere_ParticleEffect.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "ElapsedTime.h"

#include "L_ParticleEffect_Interface.h"

#include "Smoke.h"
#include "CollisionDetection_HGrid.h"
#include "t_camera.h"


using namespace std;


class ExplosionGenerator
{
    private:
        Technique* r_Technique;
        Technique_Reflection r_Reflection_Render;
        Technique_Shadow_Render r_Shadow_Render;
        Technique_TwoPass_Raycasting r_TwoPass_Render;
        Technique_DepthTexture_Render r_DepthTexture_Render;

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
        EG_utility utility_function;

        EG_SkyBox m_skybox;

        L_SphereParticleEffect l_SphereEffect;
        L_Cube_Sphere_ParticleEffect l_Cube_SphereEffect;

        Smoke smoke;

        t_camera cam;
        EG_Camera myOrbitCamera;

        // models
        meshLoader* scene;
        meshLoader* ground;
        meshLoader* ground1;
        meshLoader* sphere;
        meshLoader* smooth_sphere;
        meshLoader* cube;
        meshLoader* monkey;
        meshLoader* MainCharacter;
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

        unsigned int CubeMap_ColorTextureID_Dynamic;
        unsigned int CubeMap_DepthTextureID_Dynamic;
        unsigned int CubeMapTextureID;
        unsigned int CubeMapFBO;

        GLuint shadowMapTexture;



        /// shaders
        shader* shadow_FirstRender;
        shader* shadow_SecondRender;
        shader* quadRenderShader;   // for rendering the texture into a quad, then displaying the quad into the screen under a orthographic view
        shader* Depth_CameraRender;
        shader* ReflectionShader;
        shader* RefractionShader;
        shader* SkyboxShader;

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

        Matrices_t ReflectionSmoke;


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


        void getDepthTexture_FromLightPosion();

        void start();
        void update();
        void show();
        void RenderTexture(GLuint TextureId);
        void RenderScene();
        void RenderReflectiveObjects();
        void RenderSmoke();


        void Render_to_CubeMapTexture();
        void Render_to_CubeMapFace(int face);


        void Render_to_CubeMapTexture2();
        void Render_to_CubeMapFace2(int face);


        /// Basic Drawing functions
        void DrawAxis(float fScale, glm::vec3 translate = glm::vec3(0));
        void DrawAxis(float fScale, pipeline& m_pipeline, glm::vec3 translate = glm::vec3(0));
        void drawCube(float size);
        void drawCubeFrame(float size, int offset);
        void drawGround(float size, unsigned int textureId);


};





#endif
