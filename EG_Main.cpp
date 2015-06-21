#include "EG_Main.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "SDL\SDL.h"

using namespace std;



#define SPHERE_EFFECT 1
#define CUBE_SPHERE_EFFECT 0

#define DEFERRED_SHADING 0
#define SMOKE_EFFECT 1
#define REFLECTION_EFFECT 0
#define MASS_LIGHTING 1
#define POINT_LIGHT_BULBS   0
#define MRT_DEMO_MODE 0
#define USING_INVERSE_MATRIX 0
#define SKY_BOX 1

#define ORBIT_CAMERA_FLAG 1
#define ANIMATED_OBJECT_FLAG 0
#define INSTANCED_RENDERING 1

glm::vec3 ImpulsePosition1( GridWidth / 2.0f, GridHeight - (int) SplatRadius / 2.0f, GridDepth / 2.0f);


//Screen dimension constants
//const int SCREEN_WIDTH = 1024;
//const int SCREEN_HEIGHT = 768;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// frame rate
// https://sites.google.com/site/sdlgamer/intemediate/lesson-7
// FPS is 50
const int FRAMES_PER_SECOND = 60;
// So Interval is 1000/50 which is 20ms
// meaning my while loop runs 50 frames per second
// which is every other 20 ms
const int INTERVAL = 1000 / FRAMES_PER_SECOND;

using namespace std;


//display surface
SDL_Surface* pDisplaySurface = NULL;
//event structure
SDL_Event event;


static float runningTime = 0.0f;

int shadowMapWidth = SCREEN_WIDTH * 2;
int shadowMapheight = SCREEN_HEIGHT * 2;

bool space_bar = false;

int flag = 0;
//vector3d lightPosition(-11.0865, 13.8124, 21.3599);
glm::vec3 lightDirection(-29.0f, 330.6f, 0.0f);

float lightPosScale = 2.0f;
//glm::vec3 lightPosition(-19.1004*1.0f, 28.881*1.0f, 40.5246*1.0f);
glm::vec3 lightPosition(-19.1004*lightPosScale, 28.881*lightPosScale, 40.5246*lightPosScale);
//vector3d lightDirection(-37, 335.4);

int inc_flag = 1;
//glm::vec3 ReflectiveSphere_Pos(0,5,30);
glm::vec3 ReflectiveSphere_Pos(0,5,20);
// http://developer.download.nvidia.com/SDK/9.5/Samples/samples.html#gpgpu_fluid




ExplosionGenerator::ExplosionGenerator()
{
    angle = 0;
    isRunning = true;
    isFirstPersonCamera     = false;
    isStencilTextureMode    = false;
    isDepthTextureMode      = false;
    dvel                    = false;
    addSmoke                = false;
    holdKeyFlag             = false;
    toggleFlag              = false;
    overrideAddSmokeFlag    = false;

    m_increaseFlag = false;
    m_decreaseFlag = false;
    m_explodeFlag = false;

    m_GUIComponentsFlags = 0;
    m_GUIComponentsIDs = 0;

    m_smokeDuration = 800;
    m_orbitCamera.m_leftMouseDown = false;

    initSDL();
    initGLEW();
    initOpenGL();

    initFrameBuffers();
    smoke.init();
    initRenderers();
 //   initBillboardList();

#if DEFERRED_SHADING
    r_deferredShadingGeometryPass.init(1);
    r_deferredShadingSkybox.init(1);
    r_deferredShadingReflection.init(1);
    r_deferredShadingStencilPass.init(1);
    r_deferredShadingLightPos.init(1);
    r_deferredShadingPointLightPass.init(1);
    r_deferredShadingPointLightPass_Skybox.init(1, "/EG_DeferredShadingShaders/EG_DeferredShadingPointLightPass_Skybox.vs",
                                                        "/EG_DeferredShadingShaders/EG_DeferredShadingPointLightPass_Skybox.fs");

    r_deferredShadingDirectionalLightPass.init(1);
    r_deferredShadingDirectionalLightPass_Skybox.init(1, "/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_Skybox.vs",
                                                        "/EG_DeferredShadingShaders/EG_DeferredShadingDirectionalLightPass_Skybox.fs");


    gbuffer.init37(SCREEN_WIDTH, SCREEN_HEIGHT);
    skyboxGBuffer.init37(512, 512);
#endif
    initLights();

    m_skybox.init();

    o_reflectionSphere.setPosition(ReflectiveSphere_Pos);

    initModels();
    initObjects();
    initGUI();

    m_smokeSize = 10;
    m_testintSmokeMode = false;
#if SPHERE_EFFECT
    m_particleCount = 64;
    l_SphereEffect.InitParticles(m_particleCount);
#endif

#if CUBE_SPHERE_EFFECT
    l_Cube_SphereEffect.InitParticle(flag);
#endif


#if ORBIT_CAMERA_FLAG
    m_orbitCamera.init(m_pipeline);
#else
    thirdPersonPovCamera.init(m_pipeline);
#endif

    SDL_WM_SetCaption("Template", NULL);
 //   myThirdPOV_camera.lookAt(m_pipeline);
}




ExplosionGenerator::~ExplosionGenerator()
{
    delete scene;
    delete ground;
 //   delete sphere;
}


void ExplosionGenerator::initSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    pDisplaySurface = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32, SDL_SWSURFACE|SDL_OPENGL);
    //error check
    if (pDisplaySurface == NULL)
    {
        //report error
        cerr << "Could not set up display surface!" << SDL_GetError() << endl;
        exit(1);
    }
}

void ExplosionGenerator::initGLEW()
{
    // initialize Glew
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      cout << "Error: %s\n" << glewGetErrorString(err) << endl;
    }
}


void ExplosionGenerator::initOpenGL()
{
    //Initialize clear color
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    m_pipeline.perspective(45, SCREEN_WIDTH/SCREEN_HEIGHT, 0.5,1000.0);
    m_pipeline.matrixMode(MODEL_MATRIX);

    glEnable(GL_DEPTH_TEST);
}



void ExplosionGenerator::initFrameBuffers()
{
    /// shadowMap Texture
    FBO = EG_Utility::createFBO();
//    glGenFramebuffers(1,&FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);

    shadowMap = EG_Utility::createDepthTexture(shadowMapWidth, shadowMapheight);

    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	EG_Utility::errorCheckFBO();
	glBindFramebuffer(GL_FRAMEBUFFER,0);


    FBO1 = EG_Utility::createFBO();
//    glGenFramebuffers(1,&FBO1);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO1);
    /// depthTexture
    depthTexture = EG_Utility::createDepthTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    EG_Utility::errorCheckFBO();
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}


void ExplosionGenerator::initRenderers()
{
    r_depthRenderer.init(1);
    r_reflectionRenderer.init(1);
    r_renderTexture.init(1);
    r_fullColor.init(1);
    r_skyboxRenderer.init(1);

    r_sceneRenderer.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);
    r_twoPassRaycastingRenderer.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);

    r_skinning.init(2);
    r_buttonRenderer.init(3);
    r_instancedRenderer.init(2);

    r_billBoardRenderer.init(1);
    r_textureRenderer.init(1);

    r_nbpRenderer.init(2);
}


void ExplosionGenerator::initRendererLightParams(EG_Renderer* r_ptr)
{

}


void ExplosionGenerator::initLights()
{
    allLights.init();

#if 1
    r_sceneRenderer.enableShader(RENDER_PASS2);
        r_sceneRenderer.setMatSpecularIntensity(SPECULAR_INTENSITY);
        r_sceneRenderer.setMatSpecularPower(SPECULAR_POWER);
        r_sceneRenderer.setDirectionalLight(allLights.getDirectionalLight(0));
/*
        r_sceneRenderer.setPointLightCount(allLights.getPointLightsCount());

        for (unsigned int i=0; i<allLights.getPointLightsCount(); i++)
        {
            r_sceneRenderer.setPointLight(i, allLights.getPointLight(i));
        }
  */
    r_sceneRenderer.disableShader(RENDER_PASS2);


    r_instancedRenderer.enableShader(RENDER_PASS2);
        r_instancedRenderer.setMatSpecularIntensity(SPECULAR_INTENSITY);
        r_instancedRenderer.setMatSpecularPower(SPECULAR_POWER);
        r_instancedRenderer.setDirectionalLight(allLights.getDirectionalLight(0));
    r_instancedRenderer.disableShader(RENDER_PASS2);



    r_skinning.enableShader(RENDER_PASS2);
        r_skinning.setMatSpecularIntensity(SPECULAR_INTENSITY);
        r_skinning.setMatSpecularPower(SPECULAR_POWER);
        r_skinning.setDirectionalLight(allLights.getDirectionalLight(0));
    r_skinning.disableShader(RENDER_PASS2);
#endif


}




void ExplosionGenerator::initModels()
{
    // models
    scene = new meshLoader("shadow.obj");
    ground = new meshLoader("ground1.obj");
    wall_negative_z = new meshLoader("Wall_NZ.obj");
    wall_positive_x = new meshLoader("Wall_PX.obj");


//    wall_negative_z1.loadModel("./assets/Models/Wall_NZ.obj");
//    wall_positive_z1.loadModel("./assets/Models/Wall_PX.obj");



    o_hugeWall        = new meshLoader("HugeWall.obj");

    testSphere.loadModel("./Assets/Models/Sphere/sphere10_grey_flat.obj");
    // testSphere.loadModel("./models/Sphere.ctm");
    instancedSphere.loadModel("./Assets/Models/Sphere/sphere10_grey_flat.obj");

    bob.loadModel("./Assets/Models/Characters/boblampclean.md5mesh");
    legoMan.loadModel("./Assets/Models/Characters/Walking Lego.md5mesh");

    m_axisModel.init();

//    deferredShadingQuad = new meshLoader("./Content/quad.obj");
    smoothSphere = new meshLoader("./Sphere/sphere_grey.obj");

//    o_fullScreenQuad.init();
}


void ExplosionGenerator::initObjects()
{

#if ANIMATED_OBJECT_FLAG
    o_animatedLegoMan.setPosition(0,0,5);
  //  o_animationObject.setScale(0.1, 0.1, 0.1);
  //  o_animationObject.setOrientation(180, glm::vec3(0.0,1.0,0.0));
    o_animatedLegoMan.setOrientation(-90, glm::vec3(1.0,0.0,0.0));

    o_animatedBob.setPosition(0,0,0);
    o_animatedBob.setScale(0.1, 0.1, 0.1);
    o_animatedBob.setOrientation(-90, glm::vec3(1.0,0.0,0.0));
#endif
    m_billboardList.init("Assets/Images/monster_hellknight.png");

 //   nbp_particleSurface = EG_NoiseBasedParticleEffect::createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
 //   nbp_backgroundSurface = EG_NoiseBasedParticleEffect::createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
    m_nbpEffect.init(SCREEN_WIDTH, SCREEN_HEIGHT);
}



void ExplosionGenerator::initGUI()
{
    m_GUIComponentsFlags = 0;

    EG_Control::m_textEngine.initialize();

    int SLIDER_HEIGHT = 35;
    int BUTTON_HEIGHT = 35;


  //  m_frameRateLabel.setRect

    m_particleCountSlider.setID(m_GUIComponentsIDs);
    m_particleCountSlider.setRect(10, 400, 200, SLIDER_HEIGHT);
    m_particleCountSlider.setColor(DARK_GRAY);
    m_particleCountSlider.setSliderColor(GREEN);
    m_particleCountSlider.setLabel("Particle Count");
    m_particleCountSlider.setValue(&m_particleCount);
    m_particleCountSlider.setMaxValue(100);
    m_particleCountSlider.setMinValue(0.5);
    m_particleCountSlider.setValueType(EG_Slider::INT_TYPE);
    m_particleCountSlider.initColoredQuad();
    m_GUIComponents.push_back(&m_particleCountSlider);



    m_velocity = l_SphereEffect.m_maxVelocity;
    m_velocitySlider.setID(m_GUIComponentsIDs);
    m_velocitySlider.setRect(10, 350, 200, SLIDER_HEIGHT);
    m_velocitySlider.setColor(DARK_GRAY);
    m_velocitySlider.setSliderColor(GREEN);
    m_velocitySlider.setLabel("Max Velocity");
    m_velocitySlider.setValue(&m_velocity);
    m_velocitySlider.setMaxValue(10);
    m_velocitySlider.setMinValue(1);
    m_velocitySlider.initColoredQuad();
    m_GUIComponents.push_back(&m_velocitySlider);



    m_maxRadius = l_SphereEffect.m_maxRadius;
    m_maxRadiusSlider.setID(m_GUIComponentsIDs);
    m_maxRadiusSlider.setRect(10, 300, 200, SLIDER_HEIGHT);
    m_maxRadiusSlider.setColor(DARK_GRAY);
    m_maxRadiusSlider.setSliderColor(GREEN);
    m_maxRadiusSlider.setLabel("Max Particle Radius");
    m_maxRadiusSlider.setValue(&m_maxRadius);
    m_maxRadiusSlider.setMaxValue(2);
    m_maxRadiusSlider.setMinValue(0.5);
    m_maxRadiusSlider.initColoredQuad();
    m_GUIComponents.push_back(&m_maxRadiusSlider);


    m_smokeSizeSlider.setID(m_GUIComponentsIDs);
    m_smokeSizeSlider.setRect(10, 250, 200, SLIDER_HEIGHT);
    m_smokeSizeSlider.setColor(DARK_GRAY);
    m_smokeSizeSlider.setSliderColor(GREEN);
    m_smokeSizeSlider.setLabel("Smoke Size");
    m_smokeSizeSlider.setValue(&m_smokeSize);
    m_smokeSizeSlider.setMaxValue(20);
    m_smokeSizeSlider.setMinValue(2);
    m_smokeSizeSlider.initColoredQuad();
    m_GUIComponents.push_back(&m_smokeSizeSlider);
/*
    m_listBox.setRect(10, 400, 200, 100);
    m_listBox.setColor(GREEN);
    m_listBox.initColoredQuad();
    m_listBox.addItem("Nice");
    m_listBox.addItem("Nice");
    m_listBox.addItem("Nice");
    m_listBox.addItem("Nice");
*/

    m_resetButton.setID(m_GUIComponentsIDs);
    m_resetButton.setRect(10, 200, 200, BUTTON_HEIGHT);
    m_resetButton.setLabel("Reset");
    m_resetButton.setColor(GRAY);
    m_resetButton.initColoredQuad();
    m_GUIComponents.push_back(&m_resetButton);

    m_triggerButton.setID(m_GUIComponentsIDs);
    m_triggerButton.setRect(10, 150, 200, BUTTON_HEIGHT);
    m_triggerButton.setLabel("EXPLODE!");
    m_triggerButton.setColor(GRAY);
    m_triggerButton.initColoredQuad();
    m_GUIComponents.push_back(&m_triggerButton);

/*
    m_minimizeButton.setRect(0, SCREEN_HEIGHT - EG_Control::m_textEngine.getTextHeight(),
                            200, EG_Control::m_textEngine.getTextHeight());
    m_minimizeButton.setLabel("minimize");
    m_minimizeButton.setColor(GRAY);
    m_minimizeButton.initColoredQuad();
    m_GUIComponents.push_back(&m_minimizeButton);
*/
}


void ExplosionGenerator::initEmitter()
{
  //  m_emitter = new Emitter();

}



void ExplosionGenerator::start()
{
    cout << "Start" << endl;

    Uint32 startTime = SDL_GetTicks();
    Uint32 next_game_tick = 0;
    Uint32 delay_time = 0;

 //   m_timeManager.m_startTime = SDL_GetTicks();
    m_timeManager.setStartTime(SDL_GetTicks());
//    m_startTime = SDL_GetTicks();


    while(isRunning)
    {
        startTime = SDL_GetTicks();

        while(SDL_PollEvent(&event))
		{
            int tmpx,tmpy;
            switch(event.type)
			{
                case SDL_QUIT:
                    isRunning = false;
                    cout << "quitting game" << endl;
                    break;

                case SDL_MOUSEBUTTONUP:
                    switch(event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            cout << "clicking Up left" << endl;

                            m_orbitCamera.m_leftMouseDown = false;

                            m_mouseState.m_leftButtonDown = false;

                            SDL_GetMouseState(&tmpx,&tmpy);
                  //          g_MousePrevious = glm::vec2(tmpx, tmpy);
                            m_orbitCamera.m_mousePrevious = glm::vec2(tmpx, tmpy);
                            break;

                        case SDL_BUTTON_RIGHT:
                            cout << "clicking Up right" << endl;

                            m_mouseState.m_rightButtonDown = false;

                            SDL_GetMouseState(&tmpx,&tmpy);
                     //       g_MousePrevious = glm::vec2(tmpx, tmpy);
                            m_orbitCamera.m_mousePrevious = glm::vec2(tmpx, tmpy);


                            m_mouseState.m_rightButtonDown = false;

                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:



                    firstPersonPovCamera.setMouseInFlag(true);

#if ORBIT_CAMERA_FLAG
                    m_orbitCamera.mouseIn(true);
#else
                    thirdPersonPovCamera.mouseIn(true);
#endif


                    switch(event.button.button)
                    {
                        int tmpx,tmpy;
                        case SDL_BUTTON_LEFT:
                            cout << "clicking left" << endl;


                            m_orbitCamera.m_leftMouseDown = true;

                            SDL_GetMouseState(&tmpx,&tmpy);
                            m_mouseState.m_leftButtonDown = true;
                            break;

                        case SDL_BUTTON_RIGHT:
                            cout << "clicking right" << endl;


                            SDL_GetMouseState(&tmpx,&tmpy);
                            m_mouseState.m_rightButtonDown = true;
                            break;

                        case SDL_BUTTON_WHEELUP:
#if ORBIT_CAMERA_FLAG
                            m_orbitCamera.decreaseOffsetDistance();
#else
                            thirdPersonPovCamera.decreaseOffsetDistance();
#endif

                            cout << "wheel up" << endl;
                            break;

                        case SDL_BUTTON_WHEELDOWN:

#if ORBIT_CAMERA_FLAG
                            m_orbitCamera.increaseOffsetDistance();
#else
                            thirdPersonPovCamera.increaseOffsetDistance();
#endif

                            cout << "wheel down" << endl;
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_x:
                            overrideAddSmokeFlag = false;
                            addSmoke = false;    break;

                        case SDLK_o:   holdKeyFlag = false; break;

                        case SDLK_e:
                            m_increaseFlag = false;
                            break;

                        case SDLK_q:
                            m_decreaseFlag = false;
                            break;



                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:   isRunning = false;    break;
                        case SDLK_z:
                            m_orbitCamera.m_pivotOffset.y = 3.0f;
#if SPHERE_EFFECT
                             l_SphereEffect.Reset();
#endif

#if CUBE_SPHERE_EFFECT
                            l_Cube_SphereEffect.Reset();
#endif
//                            space_bar = false;
                            m_explodeFlag = false;
                            addSmoke = false;
                            break;
                        case SDLK_SPACE:
                            m_explodeFlag = true;
                            m_smokeStartTime = SDL_GetTicks();
                            addSmoke = true;
  //                          space_bar = true;
                            break;
                        case SDLK_v:
                            dvel = !dvel;
                            break;
                        case SDLK_e:
                            m_increaseFlag = true;
                     //       EG_Utility::printGlm("Here", m_orbitCamera.m_pivotOffset);
                            break;

                        case SDLK_q:
                            m_decreaseFlag = true;
                            break;


                        case SDLK_l:
                            isDepthTextureMode = !isDepthTextureMode;
                            break;
                        case SDLK_m:
                            isFirstPersonCamera = !isFirstPersonCamera;
                            break;
                        case SDLK_n:
                            isStencilTextureMode = !isStencilTextureMode;
                            break;

                        case SDLK_i:
                            toggleFlag = !toggleFlag;
                            break;

                        case SDLK_o:
                            holdKeyFlag = true;
                            break;

                        case SDLK_p:
                            firstPersonPovCamera.setMouseInFlag(false);
//                            myThirdPOV_camera.mouseIn(false);
                            break;
                        case SDLK_x:
                            cout << "here" << endl;
                            overrideAddSmokeFlag = true;
                            addSmoke = true;

                        case SDLK_1:
                            flag = 1;
#if CUBE_SPHERE_EFFECT
                            l_Cube_SphereEffect.Reset(flag, angle);
#endif
                            break;
                        case SDLK_2:
                            flag = 2;
#if CUBE_SPHERE_EFFECT
                            l_Cube_SphereEffect.Reset(flag, angle);
#endif
                            break;
                        case SDLK_3:
                            flag = 3;
#if CUBE_SPHERE_EFFECT
                            l_Cube_SphereEffect.Reset(flag, angle);
#endif
                            break;
#if SPHERE_EFFECT
                            l_SphereEffect.myHgrid.removeParticleFromHGrid(&l_SphereEffect.e_ParticleBuffer[3]);
#endif
                            break;
                    }
                    break;
			}
        }

            update();

#if DEFERRED_SHADING
            deferredShadingShow();
#else
            forwardRender();
#endif

            SDL_GL_SwapBuffers();

/*
            if (next_game_tick > SDL_GetTicks())
                SDL_Delay(next_game_tick - SDL_GetTicks());
            next_game_tick = SDL_GetTicks() + INTERVAL;
*/
    }
}






void ExplosionGenerator::renderShadowMap()
{
    pipeline temp_pipeline;

    temp_pipeline.matrixMode(PROJECTION_MATRIX);
    temp_pipeline.loadIdentity();
    temp_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT, 0.5, 1000.0);


    /// Move to Light's point of view
    temp_pipeline.matrixMode(VIEW_MATRIX);
    temp_pipeline.loadIdentity();

    temp_pipeline.rotateX(lightDirection.x);
    temp_pipeline.rotateY(lightDirection.y);
    temp_pipeline.translate(lightPosition.x,lightPosition.y, lightPosition.z);




    /// get the MVP matrices
    glm::mat4 lightModelMat = temp_pipeline.getModelMatrix();
    glm::mat4 lightViewMat = temp_pipeline.getViewMatrix();
    glm::mat4 lightProjMat = temp_pipeline.getProjectionMatrix();
    glm::mat4 lightMVPMat = lightProjMat * lightViewMat * lightModelMat;

    temp_pipeline.updateLightMatrix(lightModelMat, lightViewMat, lightProjMat);
    m_pipeline.updateLightMatrix(lightModelMat, lightViewMat, lightProjMat);

    /// assign the Bias Matrix to convert from NDC coordinate [-1,1] to Texture coord [0,1]





/// render from the Light's point of view
    glViewport(0,0,shadowMapWidth, shadowMapheight);
	temp_pipeline.matrixMode(MODEL_MATRIX);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);       // we don't render the front, the moisser pattern doesn't appear in the front
 //   glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);

    // r_Technique = &r_renderToDepthTexture;
    r_Technique = &r_depthRenderer;
    r_Technique->enableShader(RENDER_PASS1);

        o_wall.renderGroup(temp_pipeline, r_Technique, RENDER_PASS1, ground);
        o_wall.renderGroup(temp_pipeline, r_Technique, RENDER_PASS1, wall_negative_z);
        o_wall.renderGroup(temp_pipeline, r_Technique, RENDER_PASS1, wall_positive_x);

#if SPHERE_EFFECT
    #if INSTANCED_RENDERING

    #else
         l_SphereEffect.render(temp_pipeline, r_Technique, RENDER_PASS1, testSphere);
    #endif
#endif


#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(temp_pipeline, shadow_FirstRender->getProgramId(), cube);
#endif

  //      o_reflectionSphere.renderGroup(temp_pipeline, r_Technique, RENDER_PASS1, smoothSphere);
     //   thirdPersonPovCamera.render(temp_pipeline, r_Technique, RENDER_PASS1);
    r_Technique->disableShader(RENDER_PASS1);


#if INSTANCED_RENDERING
    r_Technique = &r_instancedRenderer;
    r_Technique->enableShader(RENDER_PASS1);

        l_SphereEffect.updateMatrices(temp_pipeline);
        l_SphereEffect.instancedRender(temp_pipeline, r_Technique, RENDER_PASS1, instancedSphere);

    r_Technique->disableShader(RENDER_PASS1);
#endif


#if ANIMATED_OBJECT_FLAG
        renderAnimatedObject(temp_pipeline, RENDER_PASS1);
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_CULL_FACE);
    glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
}


void ExplosionGenerator::update()
{
  //  m_GUIComponentsFlags = 0;
    float fDeltaTime = m_timeManager.GetElapsedTime();
    angle+=0.05f;


    if(m_increaseFlag)
        m_orbitCamera.m_pivotOffset.y+=1;


    if(m_decreaseFlag)
        m_orbitCamera.m_pivotOffset.y-=1;



    int mx, my;
    SDL_GetMouseState(&mx,&my);
    m_mouseState.m_pos = glm::vec2(mx, SCREEN_HEIGHT - my);

    bool sliding = false;




//    std::bitset<32> flag(m_GUIComponentsFlags);
//    cout << flag << endl;



  //  if(m_GUIComponentsFlags == 0)
    {
        sliding = m_particleCountSlider.update(m_mouseState, m_GUIComponentsFlags) || m_maxRadiusSlider.update(m_mouseState, m_GUIComponentsFlags);
        if(sliding)
        {
            m_explodeFlag = false;
            int c = (int)m_particleCountSlider.getValue();
            float r = m_maxRadiusSlider.getValue();

            if( (l_SphereEffect.m_particlesCount != c) || (l_SphereEffect.m_maxRadius != r) )
            {
           //     EG_Utility::debug("newCount", newCount);
                l_SphereEffect.m_particlesCount = c;
                l_SphereEffect.m_maxRadius = r;
                l_SphereEffect.Reset();
            }
        }
    }




  //  if(m_GUIComponentsFlags == 0)
  //  {
        sliding = m_velocitySlider.update(m_mouseState, m_GUIComponentsFlags);
        if(sliding)
        {
            float newV = m_velocitySlider.getValue();
            l_SphereEffect.m_maxVelocity = newV;

            if(m_explodeFlag == false)
                l_SphereEffect.resetParticleVelocity();
        }
 //   }

  //  if(m_GUIComponentsFlags == 0)
    {
        sliding = m_smokeSizeSlider.update(m_mouseState, m_GUIComponentsFlags);
        if(sliding)
        {
            m_testintSmokeMode = true;
            overrideAddSmokeFlag = true;
            addSmoke = true;
            m_smokeSize = m_smokeSizeSlider.getValue();

        }
        else
        {
            if (m_testintSmokeMode)
            {
                addSmoke = false;
                m_testintSmokeMode = false;
                overrideAddSmokeFlag = false;
            }

        }
    }


 //   if(m_GUIComponentsFlags == 0)
    {
        if(!sliding)
        {
            bool b = m_triggerButton.update(m_mouseState, m_GUIComponentsFlags);
            if(b)
            {
                m_explodeFlag = b;
                m_explodeFlag = true;
                m_smokeStartTime = SDL_GetTicks();
                addSmoke = true;
            }
        }
    }

  //  if(m_GUIComponentsFlags == 0)
    {
        if(!sliding)
        {
            bool b = m_resetButton.update(m_mouseState, m_GUIComponentsFlags);
            if(b)
            {
                m_orbitCamera.m_pivotOffset.y = 3.0f;
                l_SphereEffect.Reset();
                m_explodeFlag = false;
                addSmoke = false;
            }
        }
    }

    if(addSmoke)
    {
        if(inc_flag)
        {
            ReflectiveSphere_Pos.z+=0.1;
            if(ReflectiveSphere_Pos.z>=17)   inc_flag=0;
        }
        else
        {
            ReflectiveSphere_Pos.z-=0.1;
            if(ReflectiveSphere_Pos.z<13)   inc_flag=1;
        }
    }


    glDisable(GL_BLEND);

#if SMOKE_EFFECT

    // if(m_explodeFlag)

 //   if(m_explodeFlag)

    unsigned int diff = 0;
    unsigned int curTick = SDL_GetTicks();
    if(m_explodeFlag)
    {
        diff = (curTick - m_smokeStartTime);
 //       EG_Utility::debug("curTick", curTick);
   //     EG_Utility::debug("smoke start time", m_smokeStartTime);
     //   EG_Utility::debug("diff", diff);
        if(diff > m_smokeDuration)
            addSmoke = false;
    }


    if(overrideAddSmokeFlag)
    {
        addSmoke = true;

    }

    smoke.update(addSmoke);

    m_nbpEffect.update(0.0013, 5.0f);

#endif

    if (m_explodeFlag)
    {
#if SPHERE_EFFECT
        l_SphereEffect.update();
#endif
#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.update();
#endif
    }


    runningTime = (float)((double)SDL_GetTicks() - (double)m_timeManager.getStartTime()) / 1000.0f;
#if ANIMATED_OBJECT_FLAG

//    legoMan.boneTransform(runningTime, o_animatedLegoMan.m_boneTransforms);
  //  bob.boneTransform(runningTime, o_animatedBob.m_boneTransforms);





//    mainAvatar.boneTransform(runningTime);
//    bob.boneTransform(runningTime, r_skinning)
//    thirdPersonPovCamera.m_mainAvatar.boneTransform(runningTime, r_skinning.m_boneTransforms);
#endif
}






///************************Helper Functions**************************///







void ExplosionGenerator::RenderSmoke(bool pass1, bool pass2, Matrices_t& Mat, unsigned int depthTextureId)
{
    if(pass1)
    {
        /// First pass of the RayCasting
        Matrices.View = m_pipeline.getViewMatrix();
     //   Matrices.View = glm::mat4(1.0);

     //   float scale = 10;
        m_pipeline.pushMatrix();
            m_pipeline.translate(0,m_smokeSize,0);
            m_pipeline.rotateZ(180);
            m_pipeline.scale(m_smokeSize);
            Matrices.Model = m_pipeline.getModelMatrix();
            Matrices.Modelview = Matrices.View * Matrices.Model;
        m_pipeline.popMatrix();

        Matrices.Projection = m_pipeline.getProjectionMatrix();
        Matrices.ModelviewProjection = Matrices.Projection * Matrices.Modelview;

        glEnable(GL_CULL_FACE);

        glBindBuffer(GL_ARRAY_BUFFER, smoke.myVbos.CubeCenter);
        glEnableVertexAttribArray(SlotPosition);
        glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        /// getting the Front and Back of the cube
        r_twoPassRaycastingRenderer.Render_TwoPass_RayCasting_1(Matrices);


  //      glBindBuffer(GL_ARRAY_BUFFER,0);
  //      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  //      glDisableVertexAttribArray(SlotPosition);
    }

    if(pass2)
    {
        /// Second pass of the RayCasting
        glBindBuffer(GL_ARRAY_BUFFER, smoke.myVbos.CubeCenter);
        glEnableVertexAttribArray(SlotPosition);
        glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, smoke.f_Slab.Density.Ping.ColorTexture);

        /// the volume RayCasting part
        r_twoPassRaycastingRenderer.Render_TwoPass_RayCasting_2(Mat, depthTextureId);


 //       glBindBuffer(GL_ARRAY_BUFFER,0);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 //       glDisableVertexAttribArray(SlotPosition);
    }

}





enum CubeMapOrientation
{   POSITIVE_X=0, NEGATIVE_X, POSITIVE_Y,
    NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z};



int main(int argc, char *argv[])
{
    ExplosionGenerator Martin;

    Martin.start();

    SDL_FreeSurface(pDisplaySurface);
    SDL_Quit();
    //normal termination
    cout << "Terminating normally." << endl;
    return EXIT_SUCCESS;
}


void ExplosionGenerator::renderAnimatedObject(pipeline& p, int pass)
{
    r_Technique = &r_skinning;
    modelPtr = &legoMan;
        r_skinning.setBoneTransforms(o_animatedLegoMan.m_boneTransforms);
    //    o_animatedLegoMan.renderSingle(p, r_Technique, pass, modelPtr);

        // if(!isFirstPersonCamera)
    //    r_skinning.setBoneTransforms(thirdPersonPovCamera.m_characterObject.m_boneTransforms);

#if ORBIT_CAMERA_FLAG

#else
        thirdPersonPovCamera.render1(p, r_Technique, pass, modelPtr);
#endif // ORBIT_CAMERA_FLAG



    modelPtr = &bob;
        r_skinning.setBoneTransforms(o_animatedBob.m_boneTransforms);
        o_animatedBob.renderSingle(p, r_Technique, pass, modelPtr);
}

void ExplosionGenerator::RenderScene()
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    /// 2nd Render pass of shadowMapping: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);
    m_pipeline.pushMatrix();


    r_Technique = &r_sceneRenderer;
    r_sceneRenderer.enableShader(RENDER_PASS2);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(r_sceneRenderer.shadowMap_UniLoc,6);

//        r_sceneRenderer.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());
#if ORBIT_CAMERA_FLAG
        r_sceneRenderer.setEyeWorldPos(m_orbitCamera.getEyePoint());
#else
        r_sceneRenderer.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());
#endif


        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, ground);
        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, wall_negative_z);
        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, wall_positive_x);

#if SPHERE_EFFECT
    #if INSTANCED_RENDERING

    #else
        l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS2, testSphere);
    #endif\
//        l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS2, testSphere);

   //     l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS2, instancedSphere);
#endif

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), cube);
        l_Cube_SphereEffect.DrawMyHgridFrames();
#endif

//        thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS2);


     //   thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS2);

        r_sceneRenderer.disableShader(RENDER_PASS2);
    m_pipeline.popMatrix();



    // l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS2, testSphere);



#if INSTANCED_RENDERING
    r_Technique = &r_instancedRenderer;
    r_instancedRenderer.enableShader(RENDER_PASS2);
    m_pipeline.matrixMode(MODEL_MATRIX);
    m_pipeline.pushMatrix();

   //     glActiveTexture(GL_TEXTURE6);
   //     glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(r_instancedRenderer.shadowMap_UniLoc,6);
        r_instancedRenderer.setEyeWorldPos(m_orbitCamera.getEyePoint());


        l_SphereEffect.instancedRender(m_pipeline, r_Technique, RENDER_PASS2, instancedSphere);
    m_pipeline.popMatrix();
    r_instancedRenderer.disableShader(RENDER_PASS2);
#endif

}





void ExplosionGenerator::Render_to_CubeMapTexture2()
{
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, 512, 512);


    /// for some reason if I start at i=0, the positive X face doesn't work
    for(int i=-1; i<6; i++)
    {
//        Render_to_CubeMapFace2(i);
 //       RenderScene();
#if ANIMATED_OBJECT_FLAG
        renderAnimatedObject(m_pipeline, RENDER_PASS2);
#endif


    #if SMOKE_EFFECT
 //       if(i==NEGATIVE_Z)
  //          RenderSmoke(false, true, ReflectionSmoke, m_skybox.Dynamic_CubeMap_DepthTextureID);
    #endif

  //      glEnable(GL_DEPTH_TEST);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
}

void ExplosionGenerator::Render_to_CubeMapFace2(int face)
{
    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();

    m_pipeline.perspective(90,       // the camera angle
                            1,              // width to height ratio
                            0.5,            // the near z clippFing coordinate
                            1000.0);


    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadIdentity();

    m_pipeline.RotateForReflection(face);


    glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
///*********************************
    m_pipeline.pushMatrix();
        m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);
        ReflectionSmoke.View = m_pipeline.getViewMatrix();
    m_pipeline.popMatrix();

    m_pipeline.matrixMode(MODEL_MATRIX);
    m_pipeline.pushMatrix();
        m_pipeline.translate(0,5,0);
        m_pipeline.rotateZ(180);
        m_pipeline.scale(5);
        ReflectionSmoke.Model = m_pipeline.getModelMatrix();
        ReflectionSmoke.Modelview = ReflectionSmoke.View * ReflectionSmoke.Model;
    m_pipeline.popMatrix();

    ReflectionSmoke.Projection = m_pipeline.getProjectionMatrix();
    ReflectionSmoke.ModelviewProjection = ReflectionSmoke.Projection * ReflectionSmoke.Modelview;

    glEnable(GL_CULL_FACE);

    glBindBuffer(GL_ARRAY_BUFFER, smoke.myVbos.CubeCenter);
    glEnableVertexAttribArray(SlotPosition);
    glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    /// getting the Front and Back of the cube
    r_twoPassRaycastingRenderer.Render_TwoPass_RayCasting_1(ReflectionSmoke);
//    r_TwoPass_Render.Render_TwoPass_RayCasting_1_draft(ReflectionSmoke);
    glDisable(GL_CULL_FACE);
///*********************************



  //  glClearColor(0.0,0.0,0.5,1.0);
  //  glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0,0,512,512);



    glBindFramebuffer(GL_FRAMEBUFFER, m_skybox.CubeMapFBO);



    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)face, m_skybox.Dynamic_CubeMap_ColorTextureID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_skybox.Dynamic_CubeMap_DepthTextureID, 0);

  //  if(face != POSITIVE_X)
  //  glClearColor(0.0,0.0,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    m_pipeline.matrixMode(MODEL_MATRIX);
#if SKY_BOX
//    m_skybox.RenderSkyBox(skyboxShader, m_pipeline);
#endif
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);

    m_pipeline.matrixMode(MODEL_MATRIX);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

}




void ExplosionGenerator::forwardRender()
{



#if REFLECTION_EFFECT
    Render_to_CubeMapTexture2();
#endif



#if 1
    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();
    m_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT, 0.5, 1000.0);


///camera motion
	m_pipeline.matrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();


    if(isFirstPersonCamera)
    {
        firstPersonPovCamera.Control(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        firstPersonPovCamera.UpdateCamera();
        firstPersonPovCamera.UpdateCameraRotation(m_pipeline);
    #if SKY_BOX
        m_skybox.UpdateRotationOnly_View_Pipeline(m_pipeline);
    #endif
        firstPersonPovCamera.UpdateCameraTranslation(m_pipeline);
       // thirdPersonPovCamera.c_position = firstPersonPovCamera.getEyePoint();
        /*
        thirdPersonPovCamera.setCharacterPosition(firstPersonPovCamera.getEyePoint().x,
                                                  firstPersonPovCamera.getEyePoint().y-5,
                                                  firstPersonPovCamera.getEyePoint().z);

        thirdPersonPovCamera.setPitch(firstPersonPovCamera.getPitch());
        thirdPersonPovCamera.setYaw(firstPersonPovCamera.getYaw());
  */
    }


    else
    {

#if ORBIT_CAMERA_FLAG
     //   bool GuiFlag = m_particleCountSlider.getDraggingFlag() || m_velocitySlider.getDraggingFlag() || m_maxRadiusSlider.getDraggingFlag() || m_smokeSizeSlider.getDraggingFlag();
        bool GUIFlag = (m_GUIComponentsFlags != 0);
        m_orbitCamera.Control(m_pipeline, m_skybox, GUIFlag);
#else
        thirdPersonPovCamera.Control(m_pipeline, m_skybox);
#endif
    }





    renderShadowMap();
    l_SphereEffect.updateMatrices(m_pipeline);

#if SMOKE_EFFECT
        r_Technique = &r_depthRenderer;
        /// getting the depth of the scene
        m_pipeline.matrixMode(MODEL_MATRIX);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO1);
    //    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            m_pipeline.pushMatrix();
            r_Technique->enableShader(RENDER_PASS1);

            o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS1, ground);
            o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS1, wall_negative_z);
            o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS1, wall_positive_x);

    #if SPHERE_EFFECT
        #if INSTANCED_RENDERING
          //      l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
        #else
              l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS1, testSphere);
        #endif
    #endif

    #if CUBE_SPHERE_EFFECT
                l_Cube_SphereEffect.show(m_pipeline, Depth_CameraRender->getProgramId(), cube);
    #endif
            o_reflectionSphere.renderGroup(m_pipeline, r_Technique, RENDER_PASS1, smoothSphere);
          //  if(!isFirstPersonCamera)
          //      thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS1);
            r_Technique->disableShader(RENDER_PASS1);



    #if SPHERE_EFFECT
        #if INSTANCED_RENDERING
                r_Technique = &r_instancedRenderer;
                l_SphereEffect.instancedRender(m_pipeline, r_Technique, RENDER_PASS1, instancedSphere);
        #endif
    #endif



    #if ANIMATED_OBJECT_FLAG
     //   r_skinning.m_boneTransforms = mainAvatar.m_boneTransforms;
            renderAnimatedObject(m_pipeline, RENDER_PASS1);
    #endif
        m_pipeline.popMatrix();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif


 //   renderShadowMap();


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    /// sets the color
//    glClearColor(0,0,0.5,1);
    glClearColor(0,0,0,0);      glClear(GL_COLOR_BUFFER_BIT); // clears the buffer
    glDisable(GL_DEPTH_TEST);   glDisable(GL_CULL_FACE);

#if SKY_BOX
    r_Technique = &r_skyboxRenderer;
    r_skyboxRenderer.setCubeMapTextureUnit(0);
    m_skybox.render(r_Technique, RENDER_PASS1);
#endif


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);    glClear(GL_DEPTH_BUFFER_BIT);

    RenderScene();


#if ANIMATED_OBJECT_FLAG
    renderAnimatedObject(m_pipeline, RENDER_PASS2);
    r_skinning.renewVector();
#endif


#if REFLECTION_EFFECT
    r_Technique = &r_reflectionRenderer;
    if(isFirstPersonCamera)
        r_reflectionRenderer.setCameraPosition(firstPersonPovCamera.getEyePoint());
    else
    {

#if ORBIT_CAMERA_FLAG
        r_reflectionRenderer.setCameraPosition(m_orbitCamera.getEyePoint());
#else
        r_reflectionRenderer.setCameraPosition(thirdPersonPovCamera.getEyePoint());
#endif
    }


    r_reflectionRenderer.setReflectionTextureId(m_skybox.Dynamic_CubeMap_ColorTextureID);
    o_reflectionSphere.renderSingle(m_pipeline, r_Technique, RENDER_PASS1, smoothSphere);
#endif

  //  glDepthFunc(GL_LEQUAL);
    r_Technique = &r_fullColor;
    p_modelPtr = &m_axisModel;
    o_worldAxis.renderSingle(m_pipeline, r_Technique, RENDER_PASS1, p_modelPtr);



#if 0
    r_Technique = &r_billBoardRenderer;

    m_pipeline.pushMatrix();

        m_pipeline.translate(0,5,0);
        m_pipeline.scale(5.0);

/*
        glm::mat4 mv = m_pipeline.getModelViewMatrix();
        mv[0][0] = 5; mv[0][1] = 0; mv[0][2] = 0;
        mv[1][0] = 0; mv[1][1] = 5; mv[1][2] = 0;
        mv[2][0] = 0; mv[2][1] = 0; mv[2][2] = 5;
        glm::mat4 mvp = m_pipeline.getProjectionMatrix() * mv;
*/

        glm::mat4 mvp = m_pipeline.getModelViewProjectionMatrix();
        r_billBoardRenderer.setModelViewProjectionMatrix(mvp);
        r_billBoardRenderer.setCameraPosition(m_orbitCamera.getEyePoint());
        r_billBoardRenderer.setCameraViewDir(m_orbitCamera.m_zAxis);
      //  glm::mat4 vp = m_pipeline.getProjectionMatrix() * m_pipeline.getViewMatrix();
     //   r_billBoardRenderer.setViewProjectionMatrix(vp);
     //   r_billBoardRenderer.setCameraPosition(m_orbitCamera.getEyePoint());

        m_billboardList.render(m_pipeline, r_Technique, RENDER_PASS1);
    m_pipeline.popMatrix();

//    r_Technique =
 //   o_fullScreenQuad.render();
#endif


 //   glDepthFunc(GL_LESS);
#if SMOKE_EFFECT
    RenderSmoke(true, true, Matrices, depthTexture);
#endif
/*
    r_textureRenderer.setTextureUnit(0);
    r_textureRenderer.setPixelSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_textureRenderer.renderFullScreen(m_billboardList.m_billboardTexture, o_fullScreenQuad);
*/




    renderNoiseBasedParticleEffect2();


    renderGUI();

#endif

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ExplosionGenerator::initGUIRenderStage()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    m_pipeline.reset();
    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();
    m_pipeline.ortho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);

    m_pipeline.matrixMode(MODEL_MATRIX);
    m_pipeline.loadIdentity();
}





void ExplosionGenerator::renderNoiseBasedParticleEffect2()
{


    r_Technique = &r_nbpRenderer;
    r_textureRenderer.setTextureUnit(0);
    r_textureRenderer.setPixelSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_textureRenderer.renderFullScreen(m_nbpEffect.m_backgroundTexture, m_nbpEffect.m_backgroundSurface.FBO);
    r_Technique = &r_nbpRenderer;


    glEnable(GL_BLEND);
   // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
  //  glBindFramebuffer(GL_FRAMEBUFFER, m_nbpEffect.m_particleSurface.FBO);

    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);

        r_nbpRenderer.enableShader(RENDER_PASS1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_nbpEffect.m_backgroundSurface.depthTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_nbpEffect.m_spriteTexture);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);



            m_pipeline.pushMatrix();
                m_pipeline.translate(0,3,0);
                m_pipeline.scale(3.0);
                glm::mat4 mvp = m_pipeline.getModelViewProjectionMatrix();
                r_nbpRenderer.setModelViewProjectionMatrix(mvp);
                r_nbpRenderer.setCameraPosition(m_orbitCamera.getEyePoint());
                r_nbpRenderer.setCameraViewDir(m_orbitCamera.m_zAxis);
            m_pipeline.popMatrix();


            r_nbpRenderer.setColor(glm::vec4(1.0,1.0,0.0,1.0));
            r_nbpRenderer.setFadeRate(5.0f * 0.75f);
            r_nbpRenderer.setDepthTextureUnit(0);
            r_nbpRenderer.setSpriteTextureUnit(1);
            r_nbpRenderer.setTime(runningTime);
            r_nbpRenderer.setPointSize(0.2f);
            r_nbpRenderer.setInverseSize(glm::vec2(1.0f/SCREEN_WIDTH, 1.0f/SCREEN_HEIGHT));
            r_nbpRenderer.loadUniformLocations(m_pipeline, RENDER_PASS1);




            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glEnableVertexAttribArray(SlotPosition);
            glEnableVertexAttribArray(SlotBirthTime);
            glEnableVertexAttribArray(SlotVelocity);
            unsigned char* pData = (unsigned char*) &m_nbpEffect.m_particles[0].px;
            glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), pData);
            glVertexAttribPointer(SlotBirthTime, 1, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), 12 + pData);
            glVertexAttribPointer(SlotVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), 16 + pData);
                glDrawArrays(GL_POINTS, 0, m_nbpEffect.m_particles.size());
            glDisableVertexAttribArray(SlotPosition);
            glDisableVertexAttribArray(SlotBirthTime);
            glDisableVertexAttribArray(SlotVelocity);
            glDisable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, 0);

        r_nbpRenderer.disableShader(RENDER_PASS1);
/*
    r_Technique = &r_billBoardRenderer;
    m_pipeline.pushMatrix();
        m_pipeline.translate(0,3,0);
        m_pipeline.scale(3.0);
        glm::mat4 mvp = m_pipeline.getModelViewProjectionMatrix();
        r_billBoardRenderer.setModelViewProjectionMatrix(mvp);
        r_billBoardRenderer.setCameraPosition(m_orbitCamera.getEyePoint());
        r_billBoardRenderer.setCameraViewDir(m_orbitCamera.m_zAxis);
        m_billboardList.render(m_pipeline, r_Technique, RENDER_PASS1);
    m_pipeline.popMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
*/




#if 0
    pipeline temp_pipeline;
    temp_pipeline.loadIdentity();
    temp_pipeline.ortho(0,1,0,1,-1,1);

    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nbpEffect.m_backgroundSurface.colorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_nbpEffect.m_particleSurface.colorTexture);

    r_nbpRenderer.enableShader(RENDER_PASS2);
        r_nbpRenderer.setBackgroundTextureUnit(0);
        r_nbpRenderer.setParticlesTextureUnit(1);
        r_nbpRenderer.loadUniformLocations(temp_pipeline, RENDER_PASS2);
        r_textureRenderer.o_fullScreenQuad.render();
    r_nbpRenderer.disableShader(RENDER_PASS2);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
#endif
}




void ExplosionGenerator::renderNoiseBasedParticleEffect()
{
  //  glDisable(GL_BLEND);
  //  glDisable(GL_DEPTH_TEST);
 //   glBindFramebuffer(GL_FRAMEBUFFER, m_nbpEffect.m_backgroundSurface.FBO);
  //  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*
    r_Technique = &r_nbpRenderer;
    r_nbpRenderer.setDepth(1.0f);
    r_nbpRenderer.setScrollOffsetTime(runningTime);
*/


 //   glDisable(GL_BLEND);

    r_Technique = &r_nbpRenderer;
    r_textureRenderer.setTextureUnit(0);
    r_textureRenderer.setPixelSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_textureRenderer.renderFullScreen(m_nbpEffect.m_backgroundTexture, m_nbpEffect.m_backgroundSurface.FBO);
//    r_textureRenderer.renderFullScreen(m_nbpEffect.m_backgroundTexture, 0);
//    r_textureRenderer.renderFullScreen(m_billboardList.m_billboardTexture);
    r_Technique = &r_nbpRenderer;
/*
    r_textureRenderer.setTextureUnit(0);
    r_textureRenderer.setPixelSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_textureRenderer.renderFullScreen(m_nbpEffect.m_backgroundSurface.colorTexture, 0);
    r_Technique = &r_nbpRenderer;
*/

    EG_Utility::debug("Particle Size ", m_nbpEffect.m_particles.size());
    EG_Utility::debug("\n\n\n");


    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nbpEffect.m_particleSurface.FBO);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);

    r_Technique = &r_billBoardRenderer;
    m_pipeline.pushMatrix();
        m_pipeline.translate(0,3,0);
        m_pipeline.scale(3.0);
        glm::mat4 mvp = m_pipeline.getModelViewProjectionMatrix();
        r_billBoardRenderer.setModelViewProjectionMatrix(mvp);
        r_billBoardRenderer.setCameraPosition(m_orbitCamera.getEyePoint());
        r_billBoardRenderer.setCameraViewDir(m_orbitCamera.m_zAxis);
        m_billboardList.render(m_pipeline, r_Technique, RENDER_PASS1);
    m_pipeline.popMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);

#if 0
    glBindFramebuffer(GL_FRAMEBUFFER, m_nbpEffect.m_particleSurface.FBO);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

        r_nbpRenderer.enableShader(RENDER_PASS1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_nbpEffect.m_backgroundSurface.depthTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_nbpEffect.m_spriteTexture);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            r_nbpRenderer.setColor(glm::vec4(1.0,1.0,0.0,1.0));
            r_nbpRenderer.setFadeRate(5.0f * 0.75f);
            r_nbpRenderer.setDepthTextureUnit(0);
            r_nbpRenderer.setSpriteTextureUnit(1);
            r_nbpRenderer.setTime(runningTime);
            r_nbpRenderer.setPointSize(0.2f);
            r_nbpRenderer.setInverseSize(glm::vec2(1.0f/SCREEN_WIDTH, 1.0f/SCREEN_HEIGHT));
            r_nbpRenderer.loadUniformLocations(m_pipeline, RENDER_PASS1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glEnableVertexAttribArray(SlotPosition);
            glEnableVertexAttribArray(SlotBirthTime);
            glEnableVertexAttribArray(SlotVelocity);
            unsigned char* pData = (unsigned char*) &m_nbpEffect.m_particles[0].px;
            glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), pData);
            glVertexAttribPointer(SlotBirthTime, 1, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), 12 + pData);
            glVertexAttribPointer(SlotVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), 16 + pData);
                glDrawArrays(GL_POINTS, 0, m_nbpEffect.m_particles.size());
            glDisableVertexAttribArray(SlotPosition);
            glDisableVertexAttribArray(SlotBirthTime);
            glDisableVertexAttribArray(SlotVelocity);
            glDisable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, 0);

        r_nbpRenderer.disableShader(RENDER_PASS1);
#endif
 //   glEnable(GL_BLEND);
#if 1
    pipeline temp_pipeline;
    temp_pipeline.loadIdentity();
    temp_pipeline.ortho(0,1,0,1,-1,1);

    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nbpEffect.m_backgroundSurface.colorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_nbpEffect.m_particleSurface.colorTexture);

    r_nbpRenderer.enableShader(RENDER_PASS2);
        r_nbpRenderer.setBackgroundTextureUnit(0);
        r_nbpRenderer.setParticlesTextureUnit(1);
        r_nbpRenderer.loadUniformLocations(temp_pipeline, RENDER_PASS2);
        r_textureRenderer.o_fullScreenQuad.render();
    r_nbpRenderer.disableShader(RENDER_PASS2);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
#endif

 //   m_nbpEffect
  //  glEnable(GL_BLEND);
  //  glEnable(GL_DEPTH_TEST);
}



void ExplosionGenerator::renderGUI()
{
    initGUIRenderStage();

    /// http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement
    unsigned int getTicks = SDL_GetTicks();
    m_timeManager.addTick(getTicks);
    m_fps = m_timeManager.computeAverageFPS();


    string final_str = "Explosion Generator  FPS:" + EG_Utility::floatToStr(m_fps);
    EG_Control::m_textEngine.render(m_pipeline, 0, 10, final_str.c_str());


    r_Technique = &r_buttonRenderer;
    /*
    m_triggerButton.render(m_pipeline, r_Technique, RENDER_PASS1);
    m_resetButton.render(m_pipeline, r_Technique, RENDER_PASS1);
    m_minimizeButton.render(m_pipeline, r_Technique, RENDER_PASS1);
 */
 //   m_listBox.render(m_pipeline, r_Technique, RENDER_PASS1);


  //  m_particleCountSlider.render(m_pipeline, r_Technique, RENDER_PASS1);
  //  m_velocitySlider.render(m_pipeline, r_Technique, RENDER_PASS1);


    for(int i=0; i<m_GUIComponents.size(); i++)
    {
        EG_Control* control = m_GUIComponents[i];
        control->render(m_pipeline, r_Technique, RENDER_PASS1);
    }

}
