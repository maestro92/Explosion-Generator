#ifndef _MAIN_H_
#define _MAIN_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "define.h"

#include "EG_PointLight.h"
#include "EG_SpotLight.h"
#include "EG_DirectionalLight.h"

#include "EG_DeferredShadingDirectionalLightPass.h"

#include "EG_Camera.h"
#include "EG_FirstPersonPovCamera.h"
#include "EG_ThirdPersonPovCamera.h"

#include "EG_DeferredShading.h"
#include "EG_DeferredShading2.h"
#include "EG_GBuffer.h"

#include "EG_utility.h"
#include "EG_RenderTechniques/EG_Skybox.h"
#include "EG_Shader.h"
#include "sceneLoader.h"
#include "EG_Technique_DepthTexture_Render.h"
#include "EG_Technique_TwoPass_RayCasting.h"
#include "EG_Technique_Shadow_Render.h"
#include "EG_Technique_Reflection.h"

#include "EG_WorldBox.h"
#include "EG_WorldSphere.h"

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

#include "EG_Smoke.h"
#include "CollisionDetection_HGrid.h"


using namespace std;

/// For style
/// use http://google-styleguide.googlecode.com/svn/trunk/cppguide.html#Function_Names


/// C++ style
/// http://geosoft.no/development/cppstyle.html


/// http://stackoverflow.com/questions/1228161/why-use-prefixes-on-member-variables-in-c-classes
///
/*
I use:

m for members
c for constants/readonlys
p for pointer (and pp for pointer to pointer)
v for volatile
s for static
i for indexes and iterators
e for events


*/



class ExplosionGenerator
{
    private:
        EG_RenderTechnique* r_Technique;
        Technique_Reflection r_Reflection_Render;
        Technique_Shadow_Render r_Shadow_Render;
        Technique_TwoPass_Raycasting r_TwoPass_Render;
        Technique_DepthTexture_Render r_DepthTexture_Render;
    //    EG_DeferredShading r_deferredShadingRenderTechnique;
        EG_DeferredShading2 r_deferredShadingRenderTechnique;
        EG_DeferredShadingDirectionalLightPass r_directionalLightPass;
        EG_GBuffer gbuffer;

        GLuint VBO;
        GLuint FBO;
        pipeline m_pipeline;

        // Material Property
        float specularIntensity;
        float specularPower;

    /*
        // Lighting
        DirectionalLight m_directionalLight;
        PointLight pl[2];
        SpotLight sl[1];
    */
        EG_Utility utilityFunction;

        EG_SkyBox m_skybox;

        L_SphereParticleEffect l_SphereEffect;
        L_Cube_Sphere_ParticleEffect l_Cube_SphereEffect;

        Smoke smoke;


        EG_FirstPersonPovCamera firstPersonPovCamera;
        EG_ThirdPersonPovCamera thirdPersonPovCamera;

        // models
        meshLoader* scene;
        meshLoader* ground;
        meshLoader* ground1;
        meshLoader* wall_negative_x;
        meshLoader* wall_positive_x;

        meshLoader* wall_negative_z;
        meshLoader* wall_positive_z;


        meshLoader* sphere;
        meshLoader* smoothSphere;
        meshLoader* cube;
        meshLoader* monkey;
        meshLoader* mainCharacter;
        meshLoader* light;
        mesh* quad;
        meshLoader* deferredShadingQuad;

        bool isRunning;
        bool isFirstPersonCamera;
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
        Shader* quadRenderShader;   // for rendering the texture into a quad, then displaying the quad into the screen under a orthographic view
        Shader* Depth_CameraRender;
        Shader* skyboxShader;

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

        EG_SpotLight spotLight;
        EG_DirectionalLight directionalLight;
        EG_PointLight pointLights[3];


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

        void initShader();
        void initModels();
        void initLights();

        void setupCamera();
        void setupColor_Texture();

        void init_Texture_and_FrameBuffer();

        void SetupRenderStage();
        void getDepthTexture_FromLightPosion();
        void getDepthTexture_FromLightPosion(pipeline temp_pipeline);

        void start();
        void update();
        void show();
        void RenderTexture(GLuint TextureId);
        void RenderTexture2(GLuint TextureId);
        void RenderScene();
        void RenderReflectiveObjects();
//        void RenderSmoke();
        void RenderSmoke(bool pass1, bool pass2, Matrices_t& Mat, unsigned int depthTextureId);


        void Render_to_CubeMapTexture();
        void Render_to_CubeMapFace(int face);


        void Render_to_CubeMapTexture2();
        void Render_to_CubeMapFace2(int face);

        void deferredShadingGeometryPass();
        void deferredShadingMrtDemoPass();
        void deferredShadingLightPass();

        void deferredShadingGeometryPass36();
   //     void deferredShadingStencilPass();
        void beginDeferredShadingLightPass();
        void deferredShadingPointLightPass();
        void deferredShadingDirectionalLightPass();
        void deferredShadingDirectionalLightPass36();
   //     void deferredShadingFinalPass();


        /// Basic Drawing functions
        void drawAxis(float fScale, glm::vec3 translate = glm::vec3(0));
        void drawAxis(float fScale, pipeline& m_pipeline, glm::vec3 translate = glm::vec3(0));
        void drawCube(float size);
        void drawCubeFrame(float size, int offset);
        void drawGround(float size, unsigned int textureId);


};





#endif