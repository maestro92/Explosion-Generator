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
#include "EG_BillboardList.h"
#include "EG_NoiseBasedParticleEffect.h"

#include "EG_DeferredShadingGeometryPass.h"
#include "EG_DeferredShadingSkybox.h"
#include "EG_DeferredShadingReflection.h"
#include "EG_DeferredShadingLightPos.h"
#include "EG_DeferredShadingStencilPass.h"
#include "EG_DeferredShadingPointLightPass.h"
#include "EG_DeferredShadingDirectionalLightPass.h"

#include "EG_Emitter.h"



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
//#include "EG_Renderers/EG_Skybox.h"
#include "EG_Skybox.h"
#include "EG_Shader.h"
#include "sceneLoader.h"


/// GUI
#include "EG_Label.h"


/// Renderers
#include "EG_TwoPassRayCastingRenderer.h"
#include "EG_SceneRenderer.h"
#include "EG_ReflectionRenderer.h"
#include "EG_TextureRenderer.h"
#include "EG_DepthRenderer.h"
#include "EG_SkinningRenderer.h"
#include "EG_TextRenderer.h"
#include "EG_ButtonRenderer.h"
#include "EG_FullColorRenderer.h"
#include "EG_InstancedRenderer.h"
#include "EG_SkyboxRenderer.h"
#include "EG_BillboardRenderer.h"
#include "EG_ParticleSystemRenderer.h"
#include "EG_NoiseBasedParticleEffectRenderer.h"
#include "EG_NoiseBasedParticleEffectRendererGPU.h"

#include "EG_WorldAnimatedObject.h"

#include "EG_Button.h"
#include "EG_XYZAxisModel.h"
#include "EG_WorldBox.h"
#include "EG_WorldSphere.h"
#include "EG_ListBox.h"
#include "EG_Slider.h"

#include "pipeline.h"
#include "EG_SphereParticleEffect.h"

#include "EG_ParticleEffectRenderer.h"

#include "L_Cube_Sphere_ParticleEffect.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "EG_TimeManager.h"

#include "EG_ParticleEffect.h"
#include "EG_ParticleSystemEffect.h"

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
  //      EG_ParticleSystem  m_particleSystem;

    //    Shader* AdvectProgram;







        EG_Renderer*                    r_Technique;
        EG_DeferredShading*             r_deferredShading;
        EG_ReflectionRenderer           r_reflectionRenderer;
        EG_SceneRenderer                r_sceneRenderer;
        EG_TwoPassRaycastingRenderer    r_twoPassRaycastingRenderer;
        EG_DepthRenderer                r_depthRenderer;
        EG_SkyboxRenderer               r_skyboxRenderer;


        EG_SkinningRenderer                     r_skinning;
        EG_TextureRenderer                      r_renderTexture;
        EG_FullColorRenderer                    r_fullColor;
        EG_InstancedRenderer                    r_instancedRenderer;

        EG_BillboardRenderer                    r_billBoardRenderer;
        EG_TextureRenderer                      r_textureRenderer;
        EG_NoiseBasedParticleEffectRenderer     r_nbpRenderer;
        EG_NoiseBasedParticleEffectRendererGPU  r_nbpRendererGPU;

        EG_ParticleSystemRenderer               r_particleSystemRenderer;


/*
       // EG_DepthTextureRenderer
//        EG_DepthRenderer       r_depthRenderer;

//        EG_RenderTechnique_RenderDepthToTexture   r_renderToDepthTexture;
    //    EG_DeferredShading r_deferredShadingRenderer;
    //    EG_DeferredShading2 r_deferredShadingRenderer;
        EG_DeferredShadingGeometryPass          r_deferredShadingGeometryPass;
        EG_DeferredShadingSkybox                r_deferredShadingSkybox;
        EG_DeferredShadingReflection            r_deferredShadingReflection;
        EG_DeferredShadingStencilPass           r_deferredShadingStencilPass;
        EG_DeferredShadingLightPos              r_deferredShadingLightPos;

        EG_DeferredShadingPointLightPass        r_deferredShadingPointLightPass;
        EG_DeferredShadingPointLightPass        r_deferredShadingPointLightPass_Skybox;

        EG_DeferredShadingDirectionalLightPass  r_deferredShadingDirectionalLightPass;
        EG_DeferredShadingDirectionalLightPass  r_deferredShadingDirectionalLightPass_Skybox;
*/


        /// GUI
        EG_ButtonRenderer  r_buttonRenderer;



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
        float m_specularIntensity;
        float m_specularPower;

        EG_SkyBox m_skybox;

        EG_SphereParticleEffect l_SphereEffect;
        L_Cube_Sphere_ParticleEffect l_Cube_SphereEffect;

        Smoke smoke;


        EG_FirstPersonPovCamera firstPersonPovCamera;
        EG_ThirdPersonPovCamera thirdPersonPovCamera;
        EG_OrbitCamera m_orbitCamera;

        // models
        meshLoader* scene;
        meshLoader* ground;

        meshLoader* wall_negative_x;
        meshLoader* wall_positive_x;
        meshLoader* wall_negative_z;
        meshLoader* wall_positive_z;
        meshLoader* o_hugeWall;

/*
        EG_ImportedModel wall_negative_x1;
        EG_ImportedModel wall_positive_x1;
        EG_ImportedModel wall_negative_z1;
        EG_ImportedModel wall_positive_z1;
        // meshLoader* o_hugeWall;
*/


   //     meshLoader* sphere;
        meshLoader* smoothSphere;
//        meshLoader* cube;

//        meshLoader* mainCharacter;
        meshLoader* light;
        meshLoader* pointLightSphere;
        meshLoader* m_box;
//        mesh* quad;

        EG_Model    testSphere;
        EG_ImportedAnimatedModel legoMan;
        EG_ImportedAnimatedModel bob;
        EG_Model*   modelPtr;
        EG_ModelABS*    p_modelPtr;
        EG_XYZAxisModel m_axisModel;


        EG_InstancedModel  instancedSphere;

//        EG_DynamicModel* mainAvatar;
//        WorldObject     o_animationObject;



        EG_WorldAnimatedObject  o_animatedLegoMan;
        EG_WorldAnimatedObject  o_animatedBob;

        EG_QuadModelABS     o_fullScreenQuad;

        WorldObject     o_worldAxis;
        WorldSphere     o_reflectionSphere;
        WorldBox        o_wall;

//        meshLoader* deferredShadingQuad;

        bool holdKeyFlag;
        bool toggleFlag;

        bool overrideAddSmokeFlag;

        float m_fps;
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
        unsigned int depthTexture;


        GLuint shadowMap;



        /// Matrices
        // the matrix used to for conversion in the 2nd rendering pass
        //  convert Vertices in object space into texture coordinates in the light's perspective

        bool m_increaseFlag;
        bool m_decreaseFlag;

        float angle;

        Matrices_t ReflectionSmoke;

        EG_Label m_lb;


        EG_Emitter m_fireEffect;

        EG_ParticleSystemEffect     m_particleSystemEffect;
        EG_NoiseBasedParticleEffect m_nbpEffect;

        EG_SurfacePod m_sceneSurface;

    public:

        ExplosionGenerator();
        ~ExplosionGenerator();


        EG_BillboardList m_billboardList;


        /// init functions
        void initSDL();
        void initGLEW();
        void initOpenGL();

    //    void initBillboardList();
 //       void initShader();
        void initModels();
        void initObjects();

        void initRenderers();
        void initRendererLightParams(EG_Renderer* r_ptr);
        void initLights();
  //      void initNoiseBasedParticles();

        void initGUI();


        void initFrameBuffers();

        void SetupRenderStage();
        void getDepthTexture_FromLightPosion(pipeline temp_pipeline);

        void start();
        void update();

     //   void forwardRenderDRAFT();
        void forwardRender();
        void forwardRender2();


        void RenderScene();
        void initGUIRenderStage();
        void renderAnimatedObject(pipeline& p, int pass);
        void renderShadowMap();
        void renderGUI();
        void RenderSmoke(bool pass1, bool pass2, Matrices_t& Mat, unsigned int depthTextureId);

        void Render_to_CubeMapTexture2();
        void Render_to_CubeMapFace2(int face);

        void renderParticleSystemEffect();
        void renderNoiseBasedParticleEffect();
        void renderNoiseBasedParticleEffectGPU();
        void renderNoiseBasedParticleEffectGPU2();
    //    void renderNoiseBasedParticleEffect2();


        void initTransformFeedBackTest();
        void initTransformFeedBackTestBackup();

        void updateTransformFeedbackTest();
        void transformFeedBackTest();

/*
        void deferredShadingShow();
        void deferredShadingMrtDemoPass();

        void deferredShadingGeometryPass37();
        void deferredShadingStencilPass37(int index);
        void deferredShadingPointLightPass37(int index);
        void deferredShadingDirectionalLightPass37();
        void deferredShadingFinalPass37();

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
*/
};





#endif
