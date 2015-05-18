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



glm::vec2 g_MouseCurrent = glm::vec2(0);
glm::vec2 g_MousePrevious = glm::vec2(0);
glm::vec2 g_MouseDelta = glm::vec2(0);


// -10.4551 7.70642 15.5772
//vector3d camPosition(-10.4551, 7.70642, 15.5772);
glm::vec3 camPosition(0, 5, 20);
//vector3d camPosition(0, 0, 0);

glm::quat g_meshOrientation;


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

    m_smokeDuration = 600;


    m_orbitCamera.m_leftMouseDown = false;

    init_SDL_GLEW();
    init_OpenGL();
    initShader();


    init_Texture_and_FrameBuffer();

    smoke.init();

    initRenderers();


/*
    r_Shadow_Render.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);
    r_TwoPass_Render.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);

    r_Reflection_Render.init(1);
    r_renderTexture.init(1);
    r_renderToDepthTexture.init(1);

    r_skinning.init(2);
*/

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

    o_fullScreenQuad.init();
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
    delete quadRenderShader;
    delete scene;
    delete ground;
 //   delete sphere;
}


void ExplosionGenerator::init_SDL_GLEW()
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

    // initialize Glew
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      cout << "Error: %s\n" << glewGetErrorString(err) << endl;
    }

}


void ExplosionGenerator::init_Texture_and_FrameBuffer()
{
    /// shadowMap Texture


    glGenFramebuffers(1,&FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);

    // shadowMap = utilityFunction.CreateTexture(shadowMapWidth, shadowMapheight, true);
    shadowMap = utilityFunction.createDepthTexture(shadowMapWidth, shadowMapheight);

    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);


    int i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not OK, status=" << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);



    glGenFramebuffers(1,&FBO1);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO1);
    /// depthTexture
//    depthTexture = utilityFunction.CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, true);
    depthTexture = utilityFunction.createDepthTexture(SCREEN_WIDTH, SCREEN_HEIGHT);


    i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not OK, status=" << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}


void ExplosionGenerator::initRenderers()
{
    r_DepthTexture_Render.init(1);
    r_Shadow_Render.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);
    r_TwoPass_Render.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);

    r_Reflection_Render.init(1);
    r_renderTexture.init(1);
    r_renderToDepthTexture.init(1);

    r_fullColor.init(1);

    r_skinning.init(2);
    r_buttonRenderer.init(3);

    r_instancedRenderer.init(2);
}


void ExplosionGenerator::initRendererLightParams(EG_RenderTechnique* r_ptr)
{

}


void ExplosionGenerator::initLights()
{
    allLights.init();

#if 1
    r_Shadow_Render.enableShader(RENDER_PASS2);
        r_Shadow_Render.setMatSpecularIntensity(1.0f);
        r_Shadow_Render.setMatSpecularPower(32.0f);
        r_Shadow_Render.setDirectionalLight(allLights.getDirectionalLight(0));
/*
        r_Shadow_Render.setPointLightCount(allLights.getPointLightsCount());

        for (unsigned int i=0; i<allLights.getPointLightsCount(); i++)
        {
            r_Shadow_Render.setPointLight(i, allLights.getPointLight(i));
        }
  */
    r_Shadow_Render.disableShader(RENDER_PASS2);


    r_instancedRenderer.enableShader(RENDER_PASS2);
        r_instancedRenderer.setMatSpecularIntensity(1.0f);
        r_instancedRenderer.setMatSpecularPower(32.0f);
        r_instancedRenderer.setDirectionalLight(allLights.getDirectionalLight(0));
    r_instancedRenderer.disableShader(RENDER_PASS2);



    r_skinning.enableShader(RENDER_PASS2);
        r_skinning.setMatSpecularIntensity(1.0f);
        r_skinning.setMatSpecularPower(32.0f);
        r_skinning.setDirectionalLight(allLights.getDirectionalLight(0));
    r_skinning.disableShader(RENDER_PASS2);
#endif


}



void ExplosionGenerator::init_OpenGL()
{
    //Initialize clear color
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    m_pipeline.perspective(45, SCREEN_WIDTH/SCREEN_HEIGHT, 0.5,1000.0);
    m_pipeline.matrixMode(MODEL_MATRIX);

    glEnable(GL_DEPTH_TEST);
}


void ExplosionGenerator::initShader()
{
    // init other shaders
//	quadRenderShader=new Shader("quadRender.vs","quadRender.frag"); // rendering texture as a quad
	quadRenderShader=new Shader("quadRender2.vs","quadRender2.fs"); // rendering texture as a quad
    skyboxShader = new Shader("skybox.vs", "skybox.frag");
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

    testSphere.loadModel("./models/Sphere/sphere10_grey_flat.obj");
    // testSphere.loadModel("./models/Sphere.ctm");
    instancedSphere.loadModel("./models/Sphere/sphere10_grey_flat.obj");

    bob.loadModel("./models/Characters/boblampclean.md5mesh");
    legoMan.loadModel("./models/Characters/Walking Lego.md5mesh");

    m_axisModel.init();
//    mainAvatar.loadModel("./models/Characters/miracle3.md5mesh");

 //   mainAvatar = new EG_DynamicModel();
 //   mainAvatar->loadModel("./models/Content/boblampclean.md5mesh");

//    wall_positive_z = new meshLoader("Wall_PZ.obj");
//    wall_negative_x = new meshLoader("Wall_NX.obj");
//    wall_negative_z = new meshLoader("Wall_NZ.obj");


//    wall_positive_x = new meshLoader("Wall_NX.obj");
//    wall_positive_x = new meshLoader("Wall_NX_w25_h25.obj");


//    wall_negative_z = new meshLoader("Wall_PZ.obj");
//    wall_positive_x = new meshLoader("Wall_NZ.obj");

    deferredShadingQuad = new meshLoader("./Content/quad.obj");

//    sphere = new meshLoader("./Sphere/sphere10_grey_flat.obj");
//    sphere = new meshLoader("./Content/phoenix_ugv.md2");
    pointLightSphere = new meshLoader("./Content/sphere.obj");
//    pointLightSphere = new meshLoader("./Sphere/sphere10_grey_flat.obj");

    light = new meshLoader("./Sphere/sphere10_grey_flat.obj");
    m_box = new meshLoader("./Content/box.obj");
//    sphere = new meshLoader("./Sphere/sphere10_grey_smootsz200h.obj");
//    smooth_sphere = new meshLoader("./Sphere/sphere10_grey_smooth.obj");
    smoothSphere = new meshLoader("./Sphere/sphere_grey.obj");
//    cube = new meshLoader("cube.obj");


//    monkey = new meshLoader("monkey.obj");
//    mainCharacter = new meshLoader("./Characters/LEGO_Man.obj");


//    cube = new meshLoader("./Sphere/sphere10_grey_flat.obj");
//    meshLoader("./Content/phoenix_ugv.md2");
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




void ExplosionGenerator::start()
{
    cout << "Start" << endl;

    Uint32 startTime = SDL_GetTicks();
    Uint32 next_game_tick = 0;
    Uint32 delay_time = 0;

 //   m_timeManager.m_startTime = SDL_GetTicks();
    m_timeManager.setStartTime(SDL_GetTicks());
//    m_startTime = SDL_GetTicks();
    m_curTick = SDL_GetTicks();
    m_prevTick = SDL_GetTicks();

    while(isRunning)
    {
        startTime = SDL_GetTicks();
        m_curTick = SDL_GetTicks();



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
            m_prevTick = m_curTick;

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
    Light_ModelMatrix = temp_pipeline.getModelMatrix();
    Light_ViewMatrix = temp_pipeline.getViewMatrix();
    Light_ProjectionMatrix = temp_pipeline.getProjectionMatrix();
    Light_ModelViewProjectionMatrix = Light_ProjectionMatrix * Light_ViewMatrix * Light_ModelMatrix;

    temp_pipeline.updateLightMatrix(Light_ModelMatrix, Light_ViewMatrix, Light_ProjectionMatrix);
    m_pipeline.updateLightMatrix(Light_ModelMatrix, Light_ViewMatrix, Light_ProjectionMatrix);

    /// assign the Bias Matrix to convert from NDC coordinate [-1,1] to Texture coord [0,1]

    shadowMatrix = LIGHT_BIAS_MATRIX * Light_ModelViewProjectionMatrix;



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

    r_Technique = &r_renderToDepthTexture;
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
/*
    r_Technique = &r_skinning;
    modelPtr = &mainAvatar;

    //    temp_pipeline.pushMatrix();

   //         temp_pipeline.translate(0,0,5);
  //          temp_pipeline.rotateX(270);
   //         temp_pipeline.scale(0.1);
            r_skinning.setBoneTransforms(o_animatedLegoMan.m_boneTransforms);
            o_animatedLegoMan.renderSingle(temp_pipeline, r_Technique, RENDER_PASS1, modelPtr);
*/


#if ANIMATED_OBJECT_FLAG
        renderAnimatedObject(temp_pipeline, RENDER_PASS1);
#endif

/*
            m_pipeline.translate(0,0,5);
            m_pipeline.rotateX(270);
            m_pipeline.scale(0.1);
            o_animationObject.renderSingle(m_pipeline, r_Technique, RENDER_PASS1, modelPtr);
*/
     //   temp_pipeline.popMatrix();
 //   r_skinning.renewVector();







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




    std::bitset<32> flag(m_GUIComponentsFlags);
    cout << flag << endl;



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





    float runningTime = (float)((double)SDL_GetTicks() - (double)m_timeManager.getStartTime()) / 1000.0f;
//    legoMan.boneTransform(runningTime, o_animatedLegoMan.m_boneTransforms);
  //  bob.boneTransform(runningTime, o_animatedBob.m_boneTransforms);




//    mainAvatar.boneTransform(runningTime);
//    bob.boneTransform(runningTime, r_skinning)
//    thirdPersonPovCamera.m_mainAvatar.boneTransform(runningTime, r_skinning.m_boneTransforms);

}






///************************Helper Functions**************************///




void ExplosionGenerator::GetLightPos_ModelView_Matrix()
{
    LightPos_viewMatrix = m_pipeline.getViewMatrix();
    m_pipeline.pushMatrix();
        m_pipeline.translate(lightPosition.x, lightPosition.y, lightPosition.z);
        LightPos_modelMatrix = m_pipeline.getModelMatrix();
    m_pipeline.popMatrix();
    LightPos_modelViewMatrix = LightPos_viewMatrix * LightPos_modelMatrix;
}




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
        r_TwoPass_Render.Render_TwoPass_RayCasting_1(Matrices);


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
        r_TwoPass_Render.Render_TwoPass_RayCasting_2(Mat, depthTextureId);


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
    GetLightPos_ModelView_Matrix();

    /// 2nd Render pass of shadowMapping: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);
    m_pipeline.pushMatrix();


    r_Technique = &r_Shadow_Render;
    r_Shadow_Render.enableShader(RENDER_PASS2);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(r_Shadow_Render.shadowMap_UniLoc,6);

//        r_Shadow_Render.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());
#if ORBIT_CAMERA_FLAG
        r_Shadow_Render.setEyeWorldPos(m_orbitCamera.getEyePoint());
#else
        r_Shadow_Render.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());
#endif


        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, ground);
        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, wall_negative_z);
        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, wall_positive_x);

#if SPHERE_EFFECT
    #if INSTANCED_RENDERING

    #else
        l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS2, testSphere);
    #endif
//        l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS2, testSphere);

   //     l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS2, instancedSphere);
#endif

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), cube);
        l_Cube_SphereEffect.DrawMyHgridFrames();
#endif

//        thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS2);


     //   thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS2);

        r_Shadow_Render.disableShader(RENDER_PASS2);
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
  //      Render_to_CubeMapFace2(i);
 //       RenderScene();
#if ANIMATED_OBJECT_FLAG
        renderAnimatedObject(m_pipeline, RENDER_PASS2);
#endif
  //      glDisable(GL_DEPTH_TEST);

    #if SMOKE_EFFECT
//        if(i==NEGATIVE_Z)
  //          RenderSmoke(false, true, ReflectionSmoke, m_skybox.Dynamic_CubeMap_DepthTextureID);
    #endif

   //     glEnable(GL_DEPTH_TEST);
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
    r_TwoPass_Render.Render_TwoPass_RayCasting_1(ReflectionSmoke);
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
    m_skybox.RenderSkyBox(skyboxShader, m_pipeline);

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
    r_Technique = &r_DepthTexture_Render;
    /// getting the depth of the scene
    m_pipeline.matrixMode(MODEL_MATRIX);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
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


    r_Technique = &r_instancedRenderer;
    r_Technique->enableShader(RENDER_PASS1);

#if SPHERE_EFFECT
    #if INSTANCED_RENDERING
            l_SphereEffect.instancedRender(m_pipeline, r_Technique, RENDER_PASS1, instancedSphere);
    #endif
#endif

    r_Technique->disableShader(RENDER_PASS1);


 //   r_skinning.m_boneTransforms = mainAvatar.m_boneTransforms;
#if ANIMATED_OBJECT_FLAG
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
    m_skybox.RenderSkyBox(skyboxShader);
#endif
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);    glClear(GL_DEPTH_BUFFER_BIT);


    RenderScene();



#if ANIMATED_OBJECT_FLAG
    renderAnimatedObject(m_pipeline, RENDER_PASS2);
    r_skinning.renewVector();
#endif


#if REFLECTION_EFFECT
    r_Technique = &r_Reflection_Render;
    if(isFirstPersonCamera)
        r_Reflection_Render.setCameraPosition(firstPersonPovCamera.getEyePoint());
    else
    {

#if ORBIT_CAMERA_FLAG
        r_Reflection_Render.setCameraPosition(m_orbitCamera.getEyePoint());
#else
        r_Reflection_Render.setCameraPosition(thirdPersonPovCamera.getEyePoint());
#endif
    }


    r_Reflection_Render.setReflectionTextureId(m_skybox.Dynamic_CubeMap_ColorTextureID);
    o_reflectionSphere.renderSingle(m_pipeline, r_Technique, RENDER_PASS1, smoothSphere);
#endif

  //  glDepthFunc(GL_LEQUAL);
    r_Technique = &r_fullColor;
    p_modelPtr = &m_axisModel;
    o_worldAxis.renderSingle(m_pipeline, r_Technique, RENDER_PASS1, p_modelPtr);


 //   glDepthFunc(GL_LESS);
#if SMOKE_EFFECT
    RenderSmoke(true, true, Matrices, depthTexture);
#endif




    RenderGUI();

#endif

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ExplosionGenerator::setupGUIRenderStage()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    m_pipeline.reset();
    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();
    m_pipeline.ortho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
//    m_pipeline.ortho(-1, 1, -1, 1, -1, 1);

    m_pipeline.matrixMode(MODEL_MATRIX);
    m_pipeline.loadIdentity();
}


void ExplosionGenerator::RenderGUI()
{
    setupGUIRenderStage();

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
