#include "EG_Main.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "SDL\SDL.h"

using namespace std;


#define SPHERE_EFFECT 1
#define CUBE_SPHERE_EFFECT 0

#define DEFERRED_SHADING 0
#define SMOKE_EFFECT 1
#define REFLECTION_EFFECT 1
#define MASS_LIGHTING 1
#define POINT_LIGHT_BULBS   0
#define MRT_DEMO_MODE 0
#define USING_INVERSE_MATRIX 0
#define SKY_BOX 1



glm::vec3 ImpulsePosition1( GridWidth / 2.0f, GridHeight - (int) SplatRadius / 2.0f, GridDepth / 2.0f);


//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

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


    g_bLeftMouseDown = false;
    g_bRightMouseDown = false;

    init_SDL_GLEW();
    init_OpenGL();
    initShader();


    init_Texture_and_FrameBuffer();

    smoke.init();


    r_DepthTexture_Render.init(1);


    r_Shadow_Render.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);
    r_TwoPass_Render.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);


    r_Reflection_Render.init(1);
    r_renderTexture.init(1);
    r_renderToDepthTexture.init(1);

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
    o_worldAxis.init();
    o_reflectionSphere.setPosition(ReflectiveSphere_Pos);

    initModels();
#if SPHERE_EFFECT
    l_SphereEffect.InitParticle();
#endif

#if CUBE_SPHERE_EFFECT
    l_Cube_SphereEffect.InitParticle(flag);
#endif
    thirdPersonPovCamera.init(m_pipeline);



    SDL_WM_SetCaption("Template", NULL);
 //   myThirdPOV_camera.lookAt(m_pipeline);
}




ExplosionGenerator::~ExplosionGenerator()
{
    delete quadRenderShader;
    delete scene;
    delete ground;
    delete sphere;
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

    shadowMap = utilityFunction.CreateTexture(shadowMapWidth, shadowMapheight, true);
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
    depthTexture = utilityFunction.CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, true);


    i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not OK, status=" << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}




void ExplosionGenerator::initLights()
{
    allLights.init();

#if DEFERRED_SHADING
    r_deferredShadingPointLightPass.enableShader(RENDER_PASS1);
        r_deferredShadingPointLightPass.setPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingPointLightPass.setColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingPointLightPass.setNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        r_deferredShadingPointLightPass.setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_deferredShadingPointLightPass.disableShader(RENDER_PASS1);

    r_deferredShadingDirectionalLightPass.enableShader(RENDER_PASS1);
        r_deferredShadingDirectionalLightPass.setPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingDirectionalLightPass.setColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingDirectionalLightPass.setNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        r_deferredShadingDirectionalLightPass.setStencilTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_STENCIL);

        r_deferredShadingDirectionalLightPass.setMatSpecularIntensity(1.0f);
        r_deferredShadingDirectionalLightPass.setMatSpecularPower(32.0f);
        r_deferredShadingDirectionalLightPass.setDirectionalLight(allLights.getDirectionalLight(0));
        r_deferredShadingDirectionalLightPass.setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_deferredShadingDirectionalLightPass.disableShader(RENDER_PASS1);



    r_deferredShadingPointLightPass_Skybox.enableShader(RENDER_PASS1);
        r_deferredShadingPointLightPass_Skybox.setPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingPointLightPass_Skybox.setColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingPointLightPass_Skybox.setNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
     //   r_deferredShadingPointLightPass_Skybox.setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        r_deferredShadingPointLightPass_Skybox.setScreenSize(512, 512);
    r_deferredShadingPointLightPass_Skybox.disableShader(RENDER_PASS1);

    r_deferredShadingDirectionalLightPass_Skybox.enableShader(RENDER_PASS1);
        r_deferredShadingDirectionalLightPass_Skybox.setPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingDirectionalLightPass_Skybox.setColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingDirectionalLightPass_Skybox.setNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        r_deferredShadingDirectionalLightPass_Skybox.setStencilTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_STENCIL);

        r_deferredShadingDirectionalLightPass_Skybox.setMatSpecularIntensity(1.0f);
        r_deferredShadingDirectionalLightPass_Skybox.setMatSpecularPower(32.0f);
        r_deferredShadingDirectionalLightPass_Skybox.setDirectionalLight(allLights.getDirectionalLight(0));
        r_deferredShadingDirectionalLightPass_Skybox.setScreenSize(512, 512);
    r_deferredShadingDirectionalLightPass_Skybox.disableShader(RENDER_PASS1);

#else
    r_Shadow_Render.enableShader(RENDER_PASS2);
        r_Shadow_Render.setMatSpecularIntensity(1.0f);
        r_Shadow_Render.setMatSpecularPower(32.0f);
        r_Shadow_Render.setDirectionalLight(allLights.getDirectionalLight(0));
        r_Shadow_Render.setPointLightCount(allLights.getPointLightsCount());

        for (unsigned int i=0; i<allLights.getPointLightsCount(); i++)
        {
            r_Shadow_Render.setPointLight(i, allLights.getPointLight(i));
        }
    r_Shadow_Render.disableShader(RENDER_PASS2);
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

    o_hugeWall        = new meshLoader("HugeWall.obj");

//    wall_positive_z = new meshLoader("Wall_PZ.obj");
//    wall_negative_x = new meshLoader("Wall_NX.obj");
//    wall_negative_z = new meshLoader("Wall_NZ.obj");


//    wall_positive_x = new meshLoader("Wall_NX.obj");
//    wall_positive_x = new meshLoader("Wall_NX_w25_h25.obj");


//    wall_negative_z = new meshLoader("Wall_PZ.obj");
//    wall_positive_x = new meshLoader("Wall_NZ.obj");

    deferredShadingQuad = new meshLoader("./Content/quad.obj");

    sphere = new meshLoader("./Sphere/sphere10_grey_flat.obj");
//    sphere = new meshLoader("./Content/phoenix_ugv.md2");
    pointLightSphere = new meshLoader("./Content/sphere.obj");
//    pointLightSphere = new meshLoader("./Sphere/sphere10_grey_flat.obj");

    light = new meshLoader("./Sphere/sphere10_grey_flat.obj");
    m_box = new meshLoader("./Content/box.obj");
//    sphere = new meshLoader("./Sphere/sphere10_grey_smootsz200h.obj");
//    smooth_sphere = new meshLoader("./Sphere/sphere10_grey_smooth.obj");
    smoothSphere = new meshLoader("./Sphere/sphere_grey.obj");
    cube = new meshLoader("cube.obj");
    monkey = new meshLoader("monkey.obj");
    mainCharacter = new meshLoader("./Characters/LEGO_Man.obj");
//    cube = new meshLoader("./Sphere/sphere10_grey_flat.obj");
//    meshLoader("./Content/phoenix_ugv.md2");
}

void ExplosionGenerator::start()
{
    cout << "Start" << endl;

    Uint32 startTime = SDL_GetTicks();
    Uint32 next_game_tick = 0;
    Uint32 delay_time = 0;
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
                            g_bLeftMouseDown = false;

                            SDL_GetMouseState(&tmpx,&tmpy);
                            g_MousePrevious = glm::vec2(tmpx, tmpy);
                            break;

                        case SDL_BUTTON_RIGHT:
                            cout << "clicking Up right" << endl;
                            g_bRightMouseDown = false;

                            SDL_GetMouseState(&tmpx,&tmpy);
                            g_MousePrevious = glm::vec2(tmpx, tmpy);
                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    firstPersonPovCamera.mouseIn(true);
                    thirdPersonPovCamera.mouseIn(true);

                    firstPersonPovCamera.lookAt(lightDirection.x, lightDirection.y);

                    switch(event.button.button)
                    {
                        int tmpx,tmpy;
                        case SDL_BUTTON_LEFT:
                            cout << "clicking left" << endl;
                            g_bLeftMouseDown = true;

                            SDL_GetMouseState(&tmpx,&tmpy);
                            g_MousePrevious = glm::vec2(tmpx, tmpy);
                            break;

                        case SDL_BUTTON_RIGHT:
                            cout << "clicking right" << endl;
                            g_bRightMouseDown = true;

                            SDL_GetMouseState(&tmpx,&tmpy);
                            g_MousePrevious = glm::vec2(tmpx, tmpy);
                            break;

                        case SDL_BUTTON_WHEELUP:
                            thirdPersonPovCamera.decreaseOffsetDistance();
                            cout << "wheel up" << endl;
                            break;

                        case SDL_BUTTON_WHEELDOWN:
                            thirdPersonPovCamera.increaseOffsetDistance();
                            cout << "wheel down" << endl;
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_x:   addSmoke = false;    break;

                        case SDLK_o:   holdKeyFlag = false; break;

                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:   isRunning = false;    break;
                        case SDLK_z:

#if SPHERE_EFFECT
                             l_SphereEffect.Reset();
#endif

#if CUBE_SPHERE_EFFECT
                            l_Cube_SphereEffect.Reset();
#endif
                            space_bar = false;
                            break;
                        case SDLK_SPACE:
                            space_bar = true;
                            break;
                        case SDLK_v:
                            dvel = !dvel;
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
                            firstPersonPovCamera.mouseIn(false);
//                            myThirdPOV_camera.mouseIn(false);
                            break;
                        case SDLK_x:
                            cout << "here" << endl;
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
    r_Technique = &r_renderToDepthTexture;
    r_Technique->enableShader(RENDER_PASS1);

        glClear(GL_DEPTH_BUFFER_BIT);

        o_wall.renderGroup(temp_pipeline, r_Technique, RENDER_PASS1, ground);
        o_wall.renderGroup(temp_pipeline, r_Technique, RENDER_PASS1, wall_negative_z);
        o_wall.renderGroup(temp_pipeline, r_Technique, RENDER_PASS1, wall_positive_x);

#if SPHERE_EFFECT
        l_SphereEffect.show(temp_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif
#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(temp_pipeline, shadow_FirstRender->getProgramId(), cube);
#endif

        o_reflectionSphere.renderGroup(temp_pipeline, r_Technique, RENDER_PASS1, smoothSphere);
        thirdPersonPovCamera.render(temp_pipeline, r_Technique, RENDER_PASS1);

    r_Technique->disableShader(RENDER_PASS1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_CULL_FACE);
    glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

}


void ExplosionGenerator::update()
{
    static ElapsedTime elapsedTime;
    float fDeltaTime = elapsedTime.GetElapsedTime();
    angle+=0.05f;


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
    smoke.update();
#endif

    if (space_bar)
    {
#if SPHERE_EFFECT
        l_SphereEffect.update();
#endif
#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.update();
#endif
    }
}






///************************Helper Functions**************************///


void ExplosionGenerator::initTempTexture()
{
    glShadeModel( GL_SMOOTH );

    glEnable(GL_TEXTURE_2D);
    textureID = utilityFunction.Load_Texture("red clay.jpg");
    if(textureID == false)
    {
        cout << "textureID is NULL" << glGetError << endl;
        exit(1);
    }
}


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

        m_pipeline.pushMatrix();
            m_pipeline.translate(0,5,0);
            m_pipeline.rotateZ(180);
            m_pipeline.scale(5);
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

        r_Shadow_Render.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());


        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, ground);
        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, wall_negative_z);
        o_wall.renderGroup(m_pipeline, r_Technique, RENDER_PASS2, wall_positive_x);

#if SPHERE_EFFECT
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS2, sphere);
#endif

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), cube);
        l_Cube_SphereEffect.DrawMyHgridFrames();
#endif

        thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS2);


        r_Shadow_Render.disableShader(RENDER_PASS2);
    m_pipeline.popMatrix();
}





void ExplosionGenerator::Render_to_CubeMapTexture2()
{
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, 512, 512);

    /// for some reason if I start at i=0, the positive X face doesn't work
    for(int i=-1; i<6; i++)
    {
        Render_to_CubeMapFace2(i);
        RenderScene();
  //      glDisable(GL_DEPTH_TEST);

    #if SMOKE_EFFECT
        if(i==NEGATIVE_Z)
            RenderSmoke(false, true, ReflectionSmoke, m_skybox.Dynamic_CubeMap_DepthTextureID);
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
        firstPersonPovCamera.UpdateCamera_Rotation(m_pipeline);
    #if SKY_BOX
        m_skybox.UpdateRotationOnly_View_Pipeline(m_pipeline);
    #endif
        firstPersonPovCamera.UpdateCamera_Translation(m_pipeline);
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
        thirdPersonPovCamera.Control(m_pipeline, m_skybox);
/*
        firstPersonPovCamera.setEyePoint(thirdPersonPovCamera.c_position.x,
                                         thirdPersonPovCamera.c_position.y + 5,
                                         thirdPersonPovCamera.c_position.z);

        firstPersonPovCamera.setPitch(thirdPersonPovCamera.m_pitchDegrees);
        firstPersonPovCamera.setYaw(thirdPersonPovCamera.m_yawDegrees);
  */
    }





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
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif
#if CUBE_SPHERE_EFFECT
            l_Cube_SphereEffect.show(m_pipeline, Depth_CameraRender->getProgramId(), cube);
#endif

        o_reflectionSphere.renderGroup(m_pipeline, r_Technique, RENDER_PASS1, smoothSphere);

        if(!isFirstPersonCamera)
            thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS1);

        r_Technique->disableShader(RENDER_PASS1);
        m_pipeline.popMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif


    renderShadowMap();


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




#if REFLECTION_EFFECT
    r_Technique = &r_Reflection_Render;
    if(isFirstPersonCamera)
        r_Reflection_Render.setCameraPosition(firstPersonPovCamera.getEyePoint());
    else
        r_Reflection_Render.setCameraPosition(thirdPersonPovCamera.getEyePoint());
    r_Reflection_Render.setReflectionTextureId(m_skybox.Dynamic_CubeMap_ColorTextureID);

    o_reflectionSphere.renderSingle(m_pipeline, r_Technique, RENDER_PASS1, smoothSphere);
#endif


    r_Technique = &r_Shadow_Render;
    o_worldAxis.renderSingle(m_pipeline, r_Technique, RENDER_PASS2);


#if SMOKE_EFFECT
    RenderSmoke(true, true, Matrices, depthTexture);
#endif


#endif
}























void ExplosionGenerator::deferredShadingShow()
{

#if REFLECTION_EFFECT
 //   deferredShadingRenderToCubeMapTexture();
#endif
 //   getDepthTexture_FromLightPosion();
#if 1

    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();
    m_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT, 0.5, 1000.0);

///camera motion
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	m_pipeline.matrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();




    if(isFirstPersonCamera)
    {
        firstPersonPovCamera.Control(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        firstPersonPovCamera.UpdateCamera();
        firstPersonPovCamera.UpdateCamera_Rotation(m_pipeline);
    #if SKY_BOX
        m_skybox.UpdateRotationOnly_View_Pipeline(m_pipeline);
    #endif
        firstPersonPovCamera.UpdateCamera_Translation(m_pipeline);
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
        thirdPersonPovCamera.Control(m_pipeline, m_skybox);
/*
        firstPersonPovCamera.setEyePoint(thirdPersonPovCamera.c_position.x,
                                         thirdPersonPovCamera.c_position.y + 5,
                                         thirdPersonPovCamera.c_position.z);

        firstPersonPovCamera.setPitch(thirdPersonPovCamera.m_pitchDegrees);
        firstPersonPovCamera.setYaw(thirdPersonPovCamera.m_yawDegrees);
  */
    }



    renderShadowMap();


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    /// sets the color
//    glClearColor(0,0,0.5,1);
    glClearColor(0,0,0,0);      glClear(GL_COLOR_BUFFER_BIT); // clears the buffer
    glDisable(GL_DEPTH_TEST);   glDisable(GL_CULL_FACE);


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);    glClear(GL_DEPTH_BUFFER_BIT);




    gbuffer.StartFrame37();
    deferredShadingGeometryPass37(gbuffer);

    #if MRT_DEMO_MODE
        deferredShadingMrtDemoPass();
    #else
        glEnable(GL_STENCIL_TEST);

        #if MASS_LIGHTING
            for(unsigned int i = 0; i < allLights.getPointLightsCount(); i++)
            {
                cout << "i'm here" << endl;
                deferredShadingStencilPass37(i, gbuffer);
                deferredShadingPointLightPass37(i, gbuffer);
            }
        #endif

        glDisable(GL_STENCIL_TEST);
        deferredShadingDirectionalLightPass37(gbuffer);
        deferredShadingFinalPass37(gbuffer);
        glDisable(GL_BLEND);

        #if SMOKE_EFFECT
            RenderSmoke(true, true, Matrices, gbuffer.m_textures[EG_GBuffer::GBUFFER_TEXTURE_TYPE_DEPTH]);
        #endif
    #endif



#endif
}


void ExplosionGenerator::deferredShadingMrtDemoPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gbuffer.bindForReading();

    int WINDOW_WIDTH = SCREEN_WIDTH;
    int WINDOW_HEIGHT = SCREEN_HEIGHT;

    GLint HalfWidth = (GLint)(WINDOW_WIDTH / 2.0f);
    GLint HalfHeight = (GLint)(WINDOW_HEIGHT / 2.0f);

    /// look at the glBlitFramebuffer specs:   1st four param is sourceXY, 2nd four param is destXY
    /// the 9th parameter takes one of the three: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, or GL_STENCIL_BUFFER_BIT

    /// Lower Left
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);    /// 0
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    /// Upper Left
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);     /// 1
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, HalfHeight, HalfWidth, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);


    if(!isDepthTextureMode)
    {
        /// Upper Right
        gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);      /// 2
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    else
    {
        /// Upper Right
        gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DEPTH);      /// 5
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }

    if(!isStencilTextureMode)
    {
        /// Lower Right
        gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);    /// 3
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    else
    {
        /// Lower Right
        gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_STENCIL);    /// 4
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
}




void ExplosionGenerator::deferredShadingGeometryPass37(EG_GBuffer& GBuffer)
{
    /// 2nd Render pass of shadowMapping: camera's point of view


    GBuffer.BindForGeomPass37();
    m_pipeline.matrixMode(MODEL_MATRIX);

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if SKY_BOX
    r_Technique = &r_deferredShadingSkybox;
    r_deferredShading = &r_deferredShadingSkybox;
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
        r_deferredShading->setStencilFlag(glm::vec3(1.0,0.0,0.0));
        r_deferredShadingSkybox.setCubeMapTextureUnit(0);
        m_skybox.render(m_pipeline, r_Technique, RENDER_PASS1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
#endif

    glEnable(GL_DEPTH_TEST);
    r_deferredShading = &r_deferredShadingReflection;


    glm::vec3 eyePoint;
    if(isFirstPersonCamera)
        eyePoint = firstPersonPovCamera.getEyePoint();
    else
        eyePoint = thirdPersonPovCamera.getEyePoint();
/*
    o_reflectiveSphere.render(m_pipeline,
                          r_deferredShadingReflection,
                          RENDER_PASS1,
                          m_skybox.Dynamic_CubeMap_ColorTextureID,
                          smoothSphere,
                          eyePoint);
*/
#if POINT_LIGHT_BULBS
    r_deferredShading = &r_deferredShadingLightPos;
    r_deferredShadingLightPos.enableShader(RENDER_PASS1);
    m_pipeline.pushMatrix();
        r_deferredShading->setStencilFlag(glm::vec3(1.0,0.0,0.0));

        for(unsigned int i = 0; i < allLights.getPointLightsCount(); i++)
        {
            m_pipeline.pushMatrix();
            EG_PointLight l = allLights.getPointLight(i);
            m_pipeline.translate(l.position);
                m_pipeline.scale(0.2,0.2,0.2);
                r_deferredShadingLightPos.setLightColor(l.color);
                r_deferredShadingLightPos.loadUniformLocations(m_pipeline, RENDER_PASS1);
                sphere->draw();
            m_pipeline.popMatrix();
        }

    m_pipeline.popMatrix();
    r_deferredShadingLightPos.disableShader(RENDER_PASS1);
#endif


    r_deferredShadingGeometryPass.enableShader(RENDER_PASS1);
    m_pipeline.pushMatrix();

        r_deferredShadingGeometryPass.setStencilFlag(glm::vec3(0.0,0.0,0.0));


        r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();

      //  glDisable(GL_CULL_FACE);
        m_pipeline.pushMatrix();
            r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
            wall_negative_z->draw();
        m_pipeline.popMatrix();

        m_pipeline.pushMatrix();
            r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
            wall_positive_x->draw();
        m_pipeline.popMatrix();

        if(toggleFlag)
        {
            m_pipeline.pushMatrix();
                r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
                o_hugeWall->draw();
            m_pipeline.popMatrix();
        }


      //  glEnable(GL_CULL_FACE);

        r_Technique = &r_deferredShadingGeometryPass;

#if SPHERE_EFFECT
        l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

        thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS1);

        r_deferredShadingGeometryPass.setStencilFlag(glm::vec3(1.0,0.0,0.0));
        o_worldAxis.render(m_pipeline, r_Technique, RENDER_PASS1);


        glDepthMask(GL_FALSE);
        r_deferredShadingGeometryPass.disableShader(RENDER_PASS1);

    m_pipeline.popMatrix();

}





void ExplosionGenerator::deferredShadingGeometryPass37_Skybox(EG_GBuffer& GBuffer, pipeline tempPipeline)
{
    /// 2nd Render pass of shadowMapping: camera's point of view


    GBuffer.BindForGeomPass37();
    m_pipeline.matrixMode(MODEL_MATRIX);

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


#if SKY_BOX
    r_Technique = &r_deferredShadingSkybox;
    r_deferredShading = &r_deferredShadingSkybox;
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
        r_deferredShading->setStencilFlag(glm::vec3(1.0,0.0,0.0));
        r_deferredShadingSkybox.setCubeMapTextureUnit(0);

        m_skybox.renderStatic(tempPipeline, r_Technique, RENDER_PASS1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
#endif


    m_pipeline.matrixMode(MODEL_MATRIX);
    glEnable(GL_DEPTH_TEST);

    r_deferredShadingGeometryPass.enableShader(RENDER_PASS1);
    m_pipeline.pushMatrix();


        r_deferredShadingGeometryPass.setStencilFlag(glm::vec3(0.0,0.0,0.0));


        r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();


        m_pipeline.pushMatrix();
            wall_negative_z->draw();
            wall_positive_x->draw();
        m_pipeline.popMatrix();


        r_Technique = &r_deferredShadingGeometryPass;

#if SPHERE_EFFECT
        l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

        thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS1);

        r_deferredShadingGeometryPass.setStencilFlag(glm::vec3(1.0,0.0,0.0));
        o_worldAxis.render(m_pipeline, r_Technique, RENDER_PASS1);


        glDepthMask(GL_FALSE);


    m_pipeline.popMatrix();
    r_deferredShadingGeometryPass.disableShader(RENDER_PASS1);

}



void ExplosionGenerator::deferredShadingStencilPass37(int index, EG_GBuffer& GBuffer)
{
    r_deferredShadingStencilPass.enableShader(RENDER_PASS1);

        // Disable color/depth write and enable stencil
        GBuffer.BindForStencilPass37();
        glEnable(GL_DEPTH_TEST);

        glDisable(GL_CULL_FACE);

        glClear(GL_STENCIL_BUFFER_BIT);

        // We need the stencil test to be enabled but we want it
        // to succeed always. Only the depth test matters.
        glStencilFunc(GL_ALWAYS, 0, 0);

        /// setting front and/or back stencil test actions
        // glStencilOpSeparate is different from glStencilOp where it is specifying specifying either front or back stencil test actions
        // but of course you can set but front and back with different test actions by using two glStencilOpSeparate calls
        /// three calls
        /// sfail: stencil test fail
        /// dpfail: stencil test passes but depth test fail
        /// both stencil and depth test passes
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
//        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);
//        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);


        m_pipeline.matrixMode(MODEL_MATRIX);
        m_pipeline.pushMatrix();




        EG_PointLight l = allLights.getPointLight(index);
        glm::vec3 transMatrix = l.position;
        m_pipeline.translate(transMatrix);
        m_pipeline.scale(r_deferredShadingPointLightPass.computePointLightBoundingSphere(l));


//        m_pipeline.scale(2);
        r_deferredShadingStencilPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        pointLightSphere->draw();
        m_pipeline.popMatrix();

    r_deferredShadingStencilPass.disableShader(RENDER_PASS1);
}



void ExplosionGenerator::deferredShadingPointLightPass37(int index, EG_GBuffer& GBuffer)
{
    GBuffer.BindForLightPass37();

    r_deferredShadingPointLightPass.enableShader(RENDER_PASS1);

        r_deferredShadingPointLightPass.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());


        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);


        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);


        m_pipeline.pushMatrix();
        //    cout << "I'm drawing Point Light" << endl;



        EG_PointLight l = allLights.getPointLight(index);
        glm::vec3 transMatrix = l.position;
        m_pipeline.translate(transMatrix);
        m_pipeline.scale(r_deferredShadingPointLightPass.computePointLightBoundingSphere(l));

        r_deferredShadingPointLightPass.setPointLight(l);
        r_deferredShadingPointLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);


        pointLightSphere->draw();


        glCullFace(GL_BACK);
        glDisable(GL_BLEND);

        m_pipeline.popMatrix();
    r_deferredShadingPointLightPass.disableShader(RENDER_PASS1);
}





void ExplosionGenerator::deferredShadingPointLightPass37_Skybox(int index, EG_GBuffer& GBuffer)
{
    GBuffer.BindForLightPass37();

    r_deferredShadingPointLightPass_Skybox.enableShader(RENDER_PASS1);
        r_deferredShadingPointLightPass_Skybox.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());

//        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);


        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);


        m_pipeline.pushMatrix();
        //    cout << "I'm drawing Point Light" << endl;



        EG_PointLight l = allLights.getPointLight(index);
        glm::vec3 transMatrix = l.position;
        m_pipeline.translate(transMatrix);
        m_pipeline.scale(r_deferredShadingPointLightPass_Skybox.computePointLightBoundingSphere(l));

        r_deferredShadingPointLightPass_Skybox.setPointLight(l);
        r_deferredShadingPointLightPass_Skybox.loadUniformLocations(m_pipeline, RENDER_PASS1);


        pointLightSphere->draw();


        glCullFace(GL_BACK);
        glDisable(GL_BLEND);

        m_pipeline.popMatrix();
    r_deferredShadingPointLightPass_Skybox.disableShader(RENDER_PASS1);
}


void ExplosionGenerator::deferredShadingDirectionalLightPass37(EG_GBuffer& GBuffer)
{
    r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->useShader();
        GBuffer.BindForLightPass37();

        pipeline tempPipeline = m_pipeline;
        tempPipeline.loadIdentity();

        tempPipeline.matrixMode(PROJECTION_MATRIX);
        tempPipeline.pushMatrix();
        tempPipeline.ortho(-1,1,-1,1,-1,1);


        r_deferredShadingDirectionalLightPass.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());


        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, shadowMap);

        r_deferredShadingDirectionalLightPass.setShadowMapTextureUnit(6);
        r_deferredShadingDirectionalLightPass.loadUniformLocations(tempPipeline, RENDER_PASS1);

        o_fullScreenQuad.quad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());
        tempPipeline.popMatrix();


        glDisable(GL_BLEND);

	r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->delShader();
}


void ExplosionGenerator::deferredShadingDirectionalLightPass37_Skybox(EG_GBuffer& GBuffer)
{
    r_deferredShadingDirectionalLightPass_Skybox.progShaders[RENDER_PASS1]->useShader();
        GBuffer.BindForLightPass37();

        pipeline tempPipeline = m_pipeline;
        tempPipeline.loadIdentity();

        tempPipeline.matrixMode(PROJECTION_MATRIX);
        tempPipeline.pushMatrix();
        tempPipeline.ortho(-1,1,-1,1,-1,1);


        r_deferredShadingDirectionalLightPass_Skybox.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());


        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, shadowMap);

        r_deferredShadingDirectionalLightPass.setShadowMapTextureUnit(6);
        r_deferredShadingDirectionalLightPass_Skybox.loadUniformLocations(tempPipeline, RENDER_PASS1);

        o_fullScreenQuad.quad->draw(r_deferredShadingDirectionalLightPass_Skybox.progShaders[RENDER_PASS1]->getProgramId());
        tempPipeline.popMatrix();


        glDisable(GL_BLEND);

	r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->delShader();
}

void ExplosionGenerator::deferredShadingFinalPass37(EG_GBuffer& GBuffer)
{
    GBuffer.BindForFinalPass37();
    glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                      0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}




void ExplosionGenerator::deferredShadingRenderToCubeMapTexture()
{
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, 512, 512);

    /// for some reason if I start at i=0, the positive X face doesn't work
//    for(int i=-1; i<6; i++)
//    for(int i=-1; i<6; i++)
    for(int i=0; i<6; i++)
    {
//        if( i == NEGATIVE_Z || i == POSITIVE_X || i == NEGATIVE_X)
  //      if( i == NEGATIVE_Z )
     //   if( i != POSITIVE_X)
        {
            deferredShadingRenderToCubeMapTextureFace(i);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
}



void ExplosionGenerator::deferredShadingRenderToCubeMapTextureFace(int face)
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

    glViewport(0,0,512,512);






    skyboxGBuffer.StartFrame37();

    glClear(GL_COLOR_BUFFER_BIT);

    pipeline tempPipeline = m_pipeline;

    m_pipeline.matrixMode(MODEL_MATRIX);
    tempPipeline.matrixMode(MODEL_MATRIX);
 //   m_skybox.RenderSkyBox(skyboxShader, m_pipeline);

    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);

    m_pipeline.matrixMode(MODEL_MATRIX);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);



    deferredShadingGeometryPass37_Skybox(skyboxGBuffer, tempPipeline);

    glEnable(GL_STENCIL_TEST);
    for(unsigned int i = 0; i < allLights.getPointLightsCount(); i++)
    {
        cout << "i'm here" << endl;
        deferredShadingStencilPass37(i, skyboxGBuffer);
        deferredShadingPointLightPass37_Skybox(i, skyboxGBuffer);
    }
    glDisable(GL_STENCIL_TEST);


    deferredShadingDirectionalLightPass37_Skybox(skyboxGBuffer);


    #if SMOKE_EFFECT
        if(face==NEGATIVE_Z)
            RenderSmoke(false, true, ReflectionSmoke, skyboxGBuffer.m_textures[EG_GBuffer::GBUFFER_TEXTURE_TYPE_DEPTH]);
    #endif



    glDepthMask(GL_TRUE);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_skybox.CubeMapFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)face, m_skybox.Dynamic_CubeMap_ColorTextureID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_skybox.Dynamic_CubeMap_DepthTextureID, 0);


	// set our G_Buffer as the source
    glBindFramebuffer(GL_READ_FRAMEBUFFER, skyboxGBuffer.m_fbo);
#if 1
    glReadBuffer(GL_COLOR_ATTACHMENT7);
    glBlitFramebuffer(0, 0, 512, 512,
                      0, 0, 512, 512, GL_COLOR_BUFFER_BIT, GL_LINEAR);
#else
    /// Upper Left
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);     /// 1
    glBlitFramebuffer(0, 0, 512, 512,
                      0, 0, 512, 512,  GL_COLOR_BUFFER_BIT, GL_LINEAR);
#endif


}





