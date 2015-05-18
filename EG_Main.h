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
#include "EG_ImportedAnimatedModel.h"


#include "EG_Camera.h"
#include "EG_FirstPersonPovCamera.h"
#include "EG_ThirdPersonPovCamera.h"
#include "EG_OrbitCamera.h"

#include "EG_Model.h"
#include "EG_ImportedModel.h"
#include "EG_InstancedModel.h"

#include "EG_DeferredShading.h"
#include "EG_GBuffer.h"

#include "EG_utility.h"
//#include "EG_RenderTechniques/EG_Skybox.h"
#include "EG_Skybox.h"
#include "EG_Shader.h"
#include "sceneLoader.h"


/// GUI
#include "EG_Label.h"


/// Render Techniques
#include "EG_Technique_TwoPass_RayCasting.h"
#include "EG_Technique_Shadow_Render.h"
#include "EG_Technique_Reflection.h"
#include "EG_RenderTechnique_RenderTexture.h"
#include "EG_RenderTechnique_RenderDepthToTexture.h"
#include "EG_RenderTechnique_Skinning.h"
#include "EG_Renderer_Text.h"
#include "EG_Renderer_Button.h"
#include "EG_FullColorRenderer.h"
#include "EG_Button.h"
#include "EG_InstancedRenderer.h"

#include "EG_WorldAnimatedObject.h"
#include "EG_XYZAxisModel.h"
#include "EG_WorldBox.h"
#include "EG_WorldSphere.h"
#include "EG_QuadModel.h"
#include "EG_ListBox.h"
#include "EG_Slider.h"

#include "pipeline.h"
#include "EG_SphereParticleEffect.h"
#include "L_Cube_Sphere_ParticleEffect.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "EG_TimeManager.h"

#include "EG_ParticleEffect.h"

#include "EG_Smoke.h"
#include "CollisionDetection_HGrid.h"

#define FRAME_VALUES 10

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
        Technique_Shadow_Render         r_Shadow_Render;
        Technique_TwoPass_Raycasting    r_TwoPass_Render;
        EG_RenderTechnique_RenderDepthToTexture   r_DepthTexture_Render;
        EG_RenderTechnique_RenderDepthToTexture   r_renderToDepthTexture;

        EG_FullColorRenderer                    r_fullColor;
        EG_InstancedRenderer                    r_instancedRenderer;


        /// GUI
        EG_Renderer_Button  r_buttonRenderer;
        EG_Label m_frameRateLabel;
        EG_Button m_triggerButton;
        EG_Button m_resetButton;
        EG_Button m_minimizeButton;
        EG_Button m_pauseButton;


        EG_Slider m_smokeSizeSlider;
        float m_smokeSize;
        bool m_testintSmokeMode;

        EG_Slider m_particleCountSlider;
        float m_particleCount;

        EG_Slider m_velocitySlider;
        float m_velocity;

        EG_Slider m_maxRadiusSlider;
        float m_maxRadius;



        EG_ListBox m_listBox;

        int m_GUIComponentsIDs;
        unsigned int m_GUIComponentsFlags;
        vector<EG_Control*> m_GUIComponents;


        EG_TimeManager m_timeManager;
        long long m_runningTime;

        EG_GBuffer gbuffer;
        EG_GBuffer skyboxGBuffer;
        EG_AllLights allLights;


        MouseState m_mouseState;
        GLuint VBO;
        GLuint FBO;
        GLuint FBO1;
        pipeline m_pipeline;

        // Material Property
        float specularIntensity;
        float specularPower;


        EG_Utility utilityFunction;

        EG_SkyBox m_skybox;

        EG_SphereParticleEffect l_SphereEffect;
        L_Cube_Sphere_ParticleEffect l_Cube_SphereEffect;

        Smoke smoke;


        EG_FirstPersonPovCamera firstPersonPovCamera;
        EG_ThirdPersonPovCamera thirdPersonPovCamera;
        EG_OrbitCamera m_orbitCamera;

        // models
        meshLoader* ground;
        meshLoader* wall_negative_x;
        meshLoader* wall_positive_x;
        meshLoader* wall_negative_z;
        meshLoader* wall_positive_z;


        EG_Model    testSphere;
        EG_Model*   modelPtr;
        EG_ModelABS*    p_modelPtr;
        EG_XYZAxisModel m_axisModel;


        EG_InstancedModel  instancedSphere;
        EG_WorldAnimatedObject  o_animatedLegoMan;
        EG_WorldAnimatedObject  o_animatedBob;


        WorldObject     o_worldAxis;
        WorldSphere     o_reflectionSphere;
        WorldBox        o_wall;
        EG_QuadModel    o_fullScreenQuad;


        bool holdKeyFlag;
        bool toggleFlag;

        bool overrideAddSmokeFlag;

        float m_fps;
        float m_curTick;
        float m_prevTick;
        float m_iterRefreshRate;
        float m_curIter;
        unsigned int m_frameCount;
        unsigned int m_frameTicks[FRAME_VALUES];
        unsigned int m_frameTicksIndex;
        unsigned int m_prevFrameTick;


        bool isRunning;
        bool isFirstPersonCamera;
        bool dvel;
        bool addSmoke;
        bool isStencilTextureMode;
        bool isDepthTextureMode;

        bool m_explodeFlag;

        float m_smokeStartTime;
        float m_smokeDuration;

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


        bool m_increaseFlag;
        bool m_decreaseFlag;

        float angle;

        Matrices_t ReflectionSmoke;

        EG_Label m_lb;
    public:

        ExplosionGenerator();
        ~ExplosionGenerator();


        /// init functions
        void init_SDL_GLEW();
        void init_OpenGL();

        void initShader();
        void initModels();
        void initObjects();

        void initRenderers();
        void initRendererLightParams(EG_RenderTechnique* r_ptr);
        void initLights();

        void initGUI();


        void init_Texture_and_FrameBuffer();

        void SetupRenderStage();
        void getDepthTexture_FromLightPosion(pipeline temp_pipeline);


        void start();
        void update();

        void forwardRender();
        void deferredShadingShow();

        void RenderScene();
        void RenderGUI();

        void setupGUIRenderStage();

        void RenderSmoke(bool pass1, bool pass2, Matrices_t& Mat, unsigned int depthTextureId);
        void renderShadowMap();
};





#endif
