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
#include "EG_AllLights.h"


#include "EG_DeferredShadingGeometryPass.h"
#include "EG_DeferredShadingSkybox.h"
#include "EG_DeferredShadingReflection.h"
#include "EG_DeferredShadingLightPos.h"
#include "EG_DeferredShadingStencilPass.h"
#include "EG_DeferredShadingPointLightPass.h"
#include "EG_DeferredShadingDirectionalLightPass.h"


#include "EG_Camera.h"
#include "EG_FirstPersonPovCamera.h"
#include "EG_ThirdPersonPovCamera.h"

#include "EG_DeferredShading.h"
#include "EG_GBuffer.h"

#include "EG_utility.h"
//#include "EG_RenderTechniques/EG_Skybox.h"
#include "EG_Skybox.h"
#include "EG_Shader.h"
#include "sceneLoader.h"

#include "EG_Technique_TwoPass_RayCasting.h"
#include "EG_Technique_Shadow_Render.h"
#include "EG_Technique_Reflection.h"
#include "EG_RenderTechnique_RenderTexture.h"
#include "EG_RenderTechnique_RenderDepthToTexture.h"

#include "EG_WorldAxis.h"
#include "EG_WorldBox.h"
#include "EG_WorldSphere.h"
#include "EG_Quad.h"

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
        EG_RenderTechnique*             r_Technique;
        EG_DeferredShading*             r_deferredShading;
        Technique_Reflection            r_Reflection_Render;
        Technique_Shadow_Render         r_Shadow_Render;
        Technique_TwoPass_Raycasting    r_TwoPass_Render;
   //     Technique_DepthTexture_Render   r_DepthTexture_Render;
        EG_RenderTechnique_RenderDepthToTexture   r_DepthTexture_Render;

        EG_RenderTechnique_RenderDepthToTexture   r_renderToDepthTexture;
    //    EG_DeferredShading r_deferredShadingRenderTechnique;
    //    EG_DeferredShading2 r_deferredShadingRenderTechnique;

        EG_DeferredShadingGeometryPass          r_deferredShadingGeometryPass;
        EG_DeferredShadingSkybox                r_deferredShadingSkybox;
        EG_DeferredShadingReflection            r_deferredShadingReflection;
        EG_DeferredShadingStencilPass           r_deferredShadingStencilPass;
        EG_DeferredShadingLightPos              r_deferredShadingLightPos;

        EG_DeferredShadingPointLightPass        r_deferredShadingPointLightPass;
        EG_DeferredShadingPointLightPass        r_deferredShadingPointLightPass_Skybox;

        EG_DeferredShadingDirectionalLightPass  r_deferredShadingDirectionalLightPass;
        EG_DeferredShadingDirectionalLightPass  r_deferredShadingDirectionalLightPass_Skybox;
        EG_RenderTechnique_RenderTexture        r_renderTexture;


        EG_GBuffer gbuffer;
        EG_GBuffer skyboxGBuffer;
        EG_AllLights allLights;


        GLuint VBO;
        GLuint FBO;
        GLuint FBO1;
        pipeline m_pipeline;

        // Material Property
        float specularIntensity;
        float specularPower;


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
        meshLoader* o_hugeWall;

        meshLoader* sphere;
        meshLoader* smoothSphere;
        meshLoader* cube;
        meshLoader* monkey;
        meshLoader* mainCharacter;
        meshLoader* light;
        meshLoader* pointLightSphere;
        meshLoader* m_box;
//        mesh* quad;


        EG_WorldAxis    o_worldAxis;
        WorldSphere     o_reflectionSphere;
        WorldBox        o_wall;
        EG_Quad         o_fullScreenQuad;

        meshLoader* deferredShadingQuad;

        bool holdKeyFlag;
        bool toggleFlag;

        bool isRunning;
        bool isFirstPersonCamera;
        bool dvel;
        bool addSmoke;
        bool isStencilTextureMode;
        bool isDepthTextureMode;

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

        GLuint shadowMap;



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
//        glm::vec3 m_boxPositions[15];

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
        void initTempTexture();

        void init_Texture_and_FrameBuffer();

        void SetupRenderStage();
        void getDepthTexture_FromLightPosion(pipeline temp_pipeline);

        void start();
        void update();

        void forwardRender();
        void deferredShadingShow();

        void RenderTexture(GLuint TextureId);
        void RenderTexture2();
        void RenderScene();

//        void RenderSmoke();
        void RenderSmoke(bool pass1, bool pass2, Matrices_t& Mat, unsigned int depthTextureId);
        void RenderQuad(GLuint TextureId);


        void Render_to_CubeMapTexture2();
        void Render_to_CubeMapFace2(int face);

        // void renderShadowMap(pipeline temp_pipeline);
        void renderShadowMap();

        void deferredShadingMrtDemoPass();

/*
        void deferredShadingGeometryPass37();
        void deferredShadingStencilPass37(int index);
        void deferredShadingPointLightPass37(int index);
        void deferredShadingDirectionalLightPass37();
        void deferredShadingFinalPass37();
*/
        void deferredShadingGeometryPass37(EG_GBuffer& GBuffer);
        void deferredShadingStencilPass37(int index, EG_GBuffer& GBuffer);
        void deferredShadingPointLightPass37(int index, EG_GBuffer& GBuffer);
        void deferredShadingDirectionalLightPass37(EG_GBuffer& GBuffer);
        void deferredShadingFinalPass37(EG_GBuffer& GBuffer);

        void deferredShadingGeometryPass37_Skybox(EG_GBuffer& GBuffer);
        void deferredShadingGeometryPass37_Skybox(EG_GBuffer& GBuffer, pipeline tempPipeline);

        void deferredShadingPointLightPass37_Skybox(int index, EG_GBuffer& GBuffer);
        void deferredShadingDirectionalLightPass37_Skybox(EG_GBuffer& GBuffer);


        void deferredShadingRenderToCubeMapTexture();
        void deferredShadingRenderToCubeMapTextureFace(int face);

};





#endif
