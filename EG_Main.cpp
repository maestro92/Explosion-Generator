#include "EG_Main.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "SDL\SDL.h"

using namespace std;


#define SPHERE_EFFECT 1
#define CUBE_SPHERE_EFFECT 0

#define DEFERRED_SHADING 1
#define SMOKE_EFFECT 0
#define REFLECTION_EFFECT 0

#define USING_INVERSE_MATRIX 0

#define DS37    1

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

float FieldOfView = SCREEN_WIDTH/SCREEN_HEIGHT;
bool cam_first = true;

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

//glm::vec3 g_DefaultCameraTranslate( 0, 0, 400 );
glm::vec3 g_DefaultCameraTranslate( 0, 0, 0 );
// so positive y axis faces downwards
glm::vec3 g_DefaultCameraRotate( 0, 0, 0 );
glm::vec3 g_DefaultCameraPivot( 0, 0, 0 );


int shadowMapWidth = SCREEN_WIDTH * 2;
int shadowMapheight = SCREEN_HEIGHT * 2;

bool space_bar = false;

int flag = 0;
//vector3d lightPosition(-11.0865, 13.8124, 21.3599);
glm::vec3 lightDirection(-29.0f, 330.6f, 0.0f);

glm::vec3 lightPosition(-19.1004, 28.881, 40.5246);
//vector3d lightDirection(-37, 335.4);

int inc_flag = 1;
glm::vec3 ReflectiveSphere_Pos(0,5,15);

// http://developer.download.nvidia.com/SDK/9.5/Samples/samples.html#gpgpu_fluid




ExplosionGenerator::ExplosionGenerator()
{
    angle = 0;
    isRunning = true;
    isFirstPersonCamera = false;
    dvel = false;
    addSmoke = false;


    g_bLeftMouseDown = false;
    g_bRightMouseDown = false;

    init_SDL_GLEW();
    init_OpenGL();

    initShader();


    init_Texture_and_FrameBuffer();

    smoke.init();


    r_Shadow_Render.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);
    r_TwoPass_Render.init(SCREEN_WIDTH, SCREEN_HEIGHT, 2);
    r_DepthTexture_Render.init(1);
    r_Reflection_Render.init(1);
    r_deferredShadingRenderTechnique.init(5);

    r_deferredShadingGeometryPass.init(1);
    r_deferredShadingStencilPass.init(1);
    r_deferredShadingPointLightPass.init(1);
    r_deferredShadingDirectionalLightPass.init(1);

#if DS37
    gbuffer.init37(SCREEN_WIDTH, SCREEN_HEIGHT);
#else
    gbuffer.init(SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

    initLights();

    setupCamera();
    setupColor_Texture();

    m_skybox.init();

    initModels();
#if SPHERE_EFFECT
    l_SphereEffect.InitParticle();
#endif

#if CUBE_SPHERE_EFFECT
    l_Cube_SphereEffect.InitParticle(flag);
#endif
    thirdPersonPovCamera.init(m_pipeline);

    r_deferredShadingRenderTechnique.enableShader(RENDER_PASS1);
    r_deferredShadingRenderTechnique.SetColorTextureUnit(0);
    r_deferredShadingRenderTechnique.disableShader(RENDER_PASS1);

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

glm::vec3 Vertices[4];

void ExplosionGenerator::init_Texture_and_FrameBuffer()
{

    Vertices[0] = glm::vec3(-1,1,-4);
    Vertices[1] = glm::vec3(-1,-1,-4);
    Vertices[2] = glm::vec3(1,-1,-4);
    Vertices[3] = glm::vec3(1,1,-4);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /// shadowMap Texture
    shadow_depthTexture = utilityFunction.Create_Texture(shadowMapWidth, shadowMapheight, true);
    glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);


    glGenFramebuffers(1,&FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);

    int i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not OK, status=" << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);


    /// depthTexture
    depthTexture = utilityFunction.Create_Texture(SCREEN_WIDTH, SCREEN_HEIGHT, true);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);


    /// assigning a quad for rendering textures
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		//1.
		tmp.position = glm::vec3(-1.0,1.0,0.0);
		tmp.U=0;
		tmp.V=1;
		vertices.push_back(tmp);
		//2.
		tmp.position = glm::vec3(-1.0,-1.0,0.0);
		tmp.U=0;
		tmp.V=0;
		vertices.push_back(tmp);
		//3.
		tmp.position = glm::vec3(1.0,-1.0,0.0);
		tmp.U=1;
		tmp.V=0;
		vertices.push_back(tmp);
		//4.
		tmp.position = glm::vec3(1.0,1.0,0.0);
		tmp.U=1;
		tmp.V=1;
		vertices.push_back(tmp);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		quad=new mesh(&vertices,&indices);
	}




    /// init skybox textures
    std::string cube_filename[6];

    cube_filename[0]="./images/Skybox pictures/Skybox_Moon_Right.png";   /// positive x
	cube_filename[1]="./images/Skybox pictures/Skybox_Moon_Left.png";    /// negative x
    cube_filename[2]="./images/Skybox pictures/Skybox_Moon_Top.png";     /// positive y
	cube_filename[3]="./images/Skybox pictures/Skybox_Moon_Bottom.png";     /// negative y
	cube_filename[4]="./images/Skybox pictures/Skybox_Moon_Front.png";   /// positive z
	cube_filename[5]="./images/Skybox pictures/Skybox_Moon_Back.png";    /// negative z
    CubeMapTextureID = utilityFunction.Create_CubemapTexture(cube_filename);



    /// create the fbo
    /// 1 is to specify the number of framebuffer objects to be specified
    glGenFramebuffers(1,&CubeMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);


    CubeMap_ColorTextureID_Dynamic = utilityFunction.Create_CubemapTexture();




    /// create the uniform depth buffer
    CubeMap_DepthTextureID_Dynamic = utilityFunction.Create_Texture(512, 512, true);
    glBindTexture(GL_TEXTURE_2D, CubeMap_DepthTextureID_Dynamic);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);




    /// attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, CubeMapFBO);

    /// attach only the +X cubemap texture (for completeness)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                           CubeMap_ColorTextureID_Dynamic, 0);

 //   glClear(GL_COLOR_BUFFER_BIT);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "TextureFBO1 error" << endl;
        exit(1);
    }

    /// disable
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return ;
    }
}


void ExplosionGenerator::initLights()
{
//    allLights.init();


    spotLight.ambientIntensity = 0.0f;
    spotLight.diffuseIntensity = 0.9f;
    spotLight.color = COLOR_WHITE;
    spotLight.attenuation.linear = 0.01f;
    spotLight.position  = glm::vec3(-20.0, 20.0, 5.0f);
    spotLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
    spotLight.cutoff =  20.0f;


    directionalLight.ambientIntensity = 0.01f;
//    directionalLight.color = COLOR_CYAN;
    directionalLight.color = COLOR_WHITE;
    directionalLight.diffuseIntensity = 0.9f;
    directionalLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
//    directionalLight.direction = glm::vec3(-19.1004, 28.881, 40.5246);

    pointLights[0].diffuseIntensity = 10.0f;
    pointLights[0].color = COLOR_GREEN;
    pointLights[0].position = glm::vec3(0.0f, 1.5f, 5.0f);
    pointLights[0].attenuation.constant = 0.0f;
    pointLights[0].attenuation.linear = 0.0f;
    pointLights[0].attenuation.exp = 0.3f;

    pointLights[1].diffuseIntensity = 10.0f;
    pointLights[1].color = COLOR_RED;
    pointLights[1].position = glm::vec3(2.0f, 0.0f, 5.0f);
    pointLights[1].attenuation.constant = 0.0f;
    pointLights[1].attenuation.linear = 0.0f;
    pointLights[1].attenuation.exp = 0.3f;

    pointLights[2].diffuseIntensity = 6.0f;
    pointLights[2].color = COLOR_BLUE;
    pointLights[2].position = glm::vec3(0.0f, 0.0f, 3.0f);
    pointLights[2].attenuation.constant = 0.0f;
    pointLights[2].attenuation.linear = 0.0f;
    pointLights[2].attenuation.exp = 0.3f;

/*
    pointLights[2].diffuseIntensity = 0.2f;
    pointLights[2].color = COLOR_BLUE;
    pointLights[2].position = glm::vec3(0.0f, 0.0f, 3.0f);
    pointLights[2].attenuation.constant = 0.0f;
    pointLights[2].attenuation.linear = 0.0f;
    pointLights[2].attenuation.exp = 0.3f;
*/



    r_deferredShadingPointLightPass.enableShader(RENDER_PASS1);
        r_deferredShadingPointLightPass.SetPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingPointLightPass.SetColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingPointLightPass.SetNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        r_deferredShadingPointLightPass.SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_deferredShadingPointLightPass.disableShader(RENDER_PASS1);


    r_deferredShadingDirectionalLightPass.enableShader(RENDER_PASS1);
        r_deferredShadingDirectionalLightPass.SetPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingDirectionalLightPass.SetColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingDirectionalLightPass.SetNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
//        r_deferredShadingDirectionalLightPass.SetColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
//        r_deferredShadingDirectionalLightPass.SetNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingDirectionalLightPass.SetMatSpecularIntensity(1.0f);
        r_deferredShadingDirectionalLightPass.SetMatSpecularPower(32.0f);
        r_deferredShadingDirectionalLightPass.SetDirectionalLight(directionalLight);
//        r_deferredShadingDirectionalLightPass.SetDirectionalLight(allLights.getDirectionalLight(0));
        r_deferredShadingDirectionalLightPass.SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_deferredShadingDirectionalLightPass.disableShader(RENDER_PASS1);
}



void ExplosionGenerator::init_OpenGL()
{
    //Initialize clear color
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    m_pipeline.perspective(45, SCREEN_WIDTH/SCREEN_HEIGHT, 0.5,1000.0);
    m_pipeline.matrixMode(MODEL_MATRIX);

    //Initialize Projection Mode    (We initialize the camera lens)
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // creates a pyramid
    gluPerspective( 45,             // the camera angle
                    SCREEN_WIDTH/SCREEN_HEIGHT,    // width to height ratio
                    0.5,            // the near z clippFing coordinate
                    1000.0);         // the far z clipping coordinate
                                    // clipping coordinate: the cutoff range

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
 //   glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
}


void ExplosionGenerator::initShader()
{
    // init other shaders
	quadRenderShader=new Shader("quadRender.vs","quadRender.frag"); // rendering texture as a quad
    skyboxShader = new Shader("skybox.vs", "skybox.frag");
}

void ExplosionGenerator::initModels()
{
    // models
    scene = new meshLoader("shadow.obj");
    ground = new meshLoader("ground1.obj");
    wall_negative_z = new meshLoader("Wall_NZ.obj");
    wall_positive_x = new meshLoader("Wall_PX.obj");

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
                        case SDLK_w:   addSmoke = false;    break;
                        case SDLK_s:   addSmoke = false;    break;
                        case SDLK_a:   addSmoke = false;    break;
                        case SDLK_d:   addSmoke = false;    break;

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
                        case SDLK_m:
                            isFirstPersonCamera = !isFirstPersonCamera;
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

       //     Uint32 pre_show_time = SDL_GetTicks();
       //     show();

     //       SDL_GL_SwapBuffers();
      //      cout << "delay is " << (SDL_GetTicks() - pre_show_time) << endl;

/*
            glm::mat4 myMatrix = glm::translate(10.0f, -10.0f, 5.0f);
            cout << myMatrix[0][0] << " " << myMatrix[1][0] << " " << myMatrix[2][0] << " " << myMatrix[3][0] << " " << endl;
            cout << myMatrix[0][1] << " " << myMatrix[1][1] << " " << myMatrix[2][1] << " " << myMatrix[3][1] << " " << endl;
            cout << myMatrix[0][2] << " " << myMatrix[1][2] << " " << myMatrix[2][2] << " " << myMatrix[3][2] << " " << endl;
            cout << myMatrix[0][3] << " " << myMatrix[1][3] << " " << myMatrix[2][3] << " " << myMatrix[3][3] << " " << endl;
*/
            update();
      //      Uint32 pre_show_time = SDL_GetTicks();
            show();
            SDL_GL_SwapBuffers();
       //     cout << "delay is " << (SDL_GetTicks() - pre_show_time) << endl;

         //   next_game_tick += INTERVAL;
         //   delay_time = next_game_tick - SDL_GetTicks();

            if (next_game_tick > SDL_GetTicks())
                SDL_Delay(next_game_tick - SDL_GetTicks());
            next_game_tick = SDL_GetTicks() + INTERVAL;

    }
}




void ExplosionGenerator::getDepthTexture_FromLightPosion()
{
 //   m_pipeline.pushMatrix();
    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();
    m_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT, 0.5, 1000.0);


    /// Move to Light's point of view
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadIdentity();

    m_pipeline.rotateX(lightDirection.x);
    m_pipeline.rotateY(lightDirection.y);
    m_pipeline.translate(lightPosition.x,lightPosition.y, lightPosition.z);

    /// get the MVP matrices
    Light_ModelMatrix = m_pipeline.getModelMatrix();
    Light_ViewMatrix = m_pipeline.getViewMatrix();
    Light_ProjectionMatrix = m_pipeline.getProjectionMatrix();
    Light_ModelViewProjectionMatrix = Light_ProjectionMatrix * Light_ViewMatrix * Light_ModelMatrix;

    m_pipeline.updateLightMatrix(Light_ModelMatrix, Light_ViewMatrix, Light_ProjectionMatrix);

    /// assign the Bias Matrix to convert from NDC coordinate [-1,1] to Texture coord [0,1]
    Light_BiasMatrix[0][0]=0.5;Light_BiasMatrix[0][1]=0.0;Light_BiasMatrix[0][2]=0.0;Light_BiasMatrix[0][3]=0.0;
	Light_BiasMatrix[1][0]=0.0;Light_BiasMatrix[1][1]=0.5;Light_BiasMatrix[1][2]=0.0;Light_BiasMatrix[1][3]=0.0;
	Light_BiasMatrix[2][0]=0.0;Light_BiasMatrix[2][1]=0.0;Light_BiasMatrix[2][2]=0.5;Light_BiasMatrix[2][3]=0.0;
	Light_BiasMatrix[3][0]=0.5;Light_BiasMatrix[3][1]=0.5;Light_BiasMatrix[3][2]=0.5;Light_BiasMatrix[3][3]=1.0;

    shadowMatrix = Light_BiasMatrix * Light_ModelViewProjectionMatrix;


    /// render from the Light's point of view
    glViewport(0,0,shadowMapWidth, shadowMapheight);
	m_pipeline.matrixMode(MODEL_MATRIX);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);       // we don't render the front, the moisser pattern doesn't appear in the front
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_depthTexture, 0);

    glEnable(GL_DEPTH_TEST);

    r_Technique = &r_Shadow_Render;
    r_Technique->enableShader(RENDER_PASS1);

        glClear(GL_DEPTH_BUFFER_BIT);
        r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();

#if SPHERE_EFFECT
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(m_pipeline, shadow_FirstRender->getProgramId(), cube);
#endif

        m_pipeline.pushMatrix();
            m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);
            r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);

            smoothSphere->draw();
        m_pipeline.popMatrix();


        if(!isFirstPersonCamera)
        {
            m_pipeline.pushMatrix();
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

    r_Technique->disableShader(RENDER_PASS1);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_CULL_FACE);
    glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

 //   m_pipeline.popMatrix();
}




void ExplosionGenerator::getDepthTexture_FromLightPosion(pipeline temp_pipeline)
{
 //   temp_pipeline.pushMatrix();
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
    Light_BiasMatrix[0][0]=0.5;Light_BiasMatrix[0][1]=0.0;Light_BiasMatrix[0][2]=0.0;Light_BiasMatrix[0][3]=0.0;
	Light_BiasMatrix[1][0]=0.0;Light_BiasMatrix[1][1]=0.5;Light_BiasMatrix[1][2]=0.0;Light_BiasMatrix[1][3]=0.0;
	Light_BiasMatrix[2][0]=0.0;Light_BiasMatrix[2][1]=0.0;Light_BiasMatrix[2][2]=0.5;Light_BiasMatrix[2][3]=0.0;
	Light_BiasMatrix[3][0]=0.5;Light_BiasMatrix[3][1]=0.5;Light_BiasMatrix[3][2]=0.5;Light_BiasMatrix[3][3]=1.0;

    shadowMatrix = Light_BiasMatrix * Light_ModelViewProjectionMatrix;


    /// render from the Light's point of view
    glViewport(0,0,shadowMapWidth, shadowMapheight);
	temp_pipeline.matrixMode(MODEL_MATRIX);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);       // we don't render the front, the moisser pattern doesn't appear in the front
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_depthTexture, 0);

    glEnable(GL_DEPTH_TEST);

    r_Technique = &r_Shadow_Render;
    r_Technique->enableShader(RENDER_PASS1);

        glClear(GL_DEPTH_BUFFER_BIT);
        r_Shadow_Render.loadUniformLocations(temp_pipeline, RENDER_PASS1);
        ground->draw();

#if SPHERE_EFFECT
        l_SphereEffect.show(temp_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(temp_pipeline, shadow_FirstRender->getProgramId(), cube);
#endif

        temp_pipeline.pushMatrix();
            temp_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);
            r_Shadow_Render.loadUniformLocations(temp_pipeline, RENDER_PASS1);

            smoothSphere->draw();
        temp_pipeline.popMatrix();

        if(!isFirstPersonCamera)
        {
            temp_pipeline.pushMatrix();
                temp_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
                temp_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_Shadow_Render.loadUniformLocations(temp_pipeline, RENDER_PASS1);
                thirdPersonPovCamera.m_character->draw();
            temp_pipeline.popMatrix();
        }

    r_Technique->disableShader(RENDER_PASS1);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_CULL_FACE);
    glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

//   temp_pipeline.popMatrix();
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
 //   Eulerian3D_update_SameFBO();
    if (space_bar)
    {
#if SPHERE_EFFECT
        l_SphereEffect.update();
#endif
#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.update();
#endif
    }

//    cout << cam.getLocation().x << " " << cam.getLocation().y << " " << cam.getLocation().z << endl;
}



void ExplosionGenerator::SetupRenderStage()
{



}

void ExplosionGenerator::show()
{
#if REFLECTION_EFFECT
    Render_to_CubeMapTexture2();
#endif

//    getDepthTexture_FromLightPosion();
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
        thirdPersonPovCamera.setCharacterPosition(firstPersonPovCamera.getEyePoint().x,
                                                  firstPersonPovCamera.getEyePoint().y-5,
                                                  firstPersonPovCamera.getEyePoint().z);

        thirdPersonPovCamera.setPitch(firstPersonPovCamera.getPitch());
        thirdPersonPovCamera.setYaw(firstPersonPovCamera.getYaw());
    }

    else
    {
        thirdPersonPovCamera.Control(m_pipeline);
     //   cout << "Character Position is " << thirdPersonPovCamera.c_position.x << " "
     //                                << thirdPersonPovCamera.c_position.y << " "
     //                                << thirdPersonPovCamera.c_position.z << endl;
        firstPersonPovCamera.setEyePoint(thirdPersonPovCamera.c_position.x,
                                         thirdPersonPovCamera.c_position.y + 5,
                                         thirdPersonPovCamera.c_position.z);

        firstPersonPovCamera.setPitch(thirdPersonPovCamera.m_pitchDegrees);
        firstPersonPovCamera.setYaw(thirdPersonPovCamera.m_yawDegrees);
    }


    pipeline temp_pipeline;
    temp_pipeline.perspective(45, SCREEN_WIDTH/SCREEN_HEIGHT, 0.5,1000.0);
    temp_pipeline.matrixMode(MODEL_MATRIX);

    ///First render pass: light's point of view
    getDepthTexture_FromLightPosion(temp_pipeline);


#if SMOKE_EFFECT
    r_Technique = &r_DepthTexture_Render;
    /// getting the depth of the scene
    m_pipeline.matrixMode(MODEL_MATRIX);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        m_pipeline.pushMatrix();
        r_Technique->enableShader(RENDER_PASS1);

            r_DepthTexture_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);
            ground->draw();



#if SPHERE_EFFECT
            l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif
#if CUBE_SPHERE_EFFECT
            l_Cube_SphereEffect.show(m_pipeline, Depth_CameraRender->getProgramId(), cube);
#endif

            m_pipeline.pushMatrix();
                m_pipeline.translate(ReflectiveSphere_Pos.x, ReflectiveSphere_Pos.y, ReflectiveSphere_Pos.z);
                r_DepthTexture_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);

                smoothSphere->draw();
            m_pipeline.popMatrix();


        if(!isFirstPersonCamera)
        {
            m_pipeline.pushMatrix();
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_DepthTexture_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

        r_Technique->disableShader(RENDER_PASS1);
        m_pipeline.popMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    /// sets the color
//    glClearColor(0,0,0.5,1);
    glClearColor(0,0,0,0);    glClear(GL_COLOR_BUFFER_BIT); // clears the buffer
    glDisable(GL_DEPTH_TEST);   glDisable(GL_CULL_FACE);

#if SKY_BOX
    m_skybox.RenderSkyBox(skyboxShader);
#endif
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);    glClear(GL_DEPTH_BUFFER_BIT);




#if DEFERRED_SHADING

#if DS37


    gbuffer.StartFrame37();
    deferredShadingGeometryPass37();
    glEnable(GL_STENCIL_TEST);

    for(unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(pointLights); i++)
  //  for(unsigned int i = 0; i < 1; i++)
    {
        deferredShadingStencilPass37(i);
        deferredShadingPointLightPass37(i);
    }

    glDisable(GL_STENCIL_TEST);
    deferredShadingDirectionalLightPass37();
    deferredShadingFinalPass37();
    glDisable(GL_BLEND);


#else
//    deferredShadingGeometryPass36();
    deferredShadingGeometryPass36();
 //   deferredShadingMrtDemoPass();
//    deferredShadingLightPass();
    beginDeferredShadingLightPass();
//    deferredShadingDirectionalLightPass();
    deferredShadingPointLightPass();
    deferredShadingDirectionalLightPass36();
    glDisable(GL_BLEND);
#endif



#else
    RenderScene();
#endif




#if REFLECTION_EFFECT
    RenderReflectiveObjects();
#endif

//    drawAxis(20, glm::vec3(0,0,0));

#if SMOKE_EFFECT
//    RenderSmoke(true, true, Matrices, gbuffer.get_depth_texture());
    RenderSmoke(true, true, Matrices, depthTexture);
#endif


#endif
}


void ExplosionGenerator::deferredShadingGeometryPass()
{
    /// 2nd Render pass of shadowMapping: camera's point of view



    m_pipeline.matrixMode(MODEL_MATRIX);
 //   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_pipeline.pushMatrix();

    r_deferredShadingRenderTechnique.enableShader(RENDER_PASS1);

        gbuffer.bindForWriting();

   //     glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
   //     glDisable(GL_BLEND);

        r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();


        m_pipeline.pushMatrix();
            wall_negative_z->draw();
            wall_positive_x->draw();
        m_pipeline.popMatrix();


#if SPHERE_EFFECT
        r_Technique = &r_deferredShadingRenderTechnique;
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

        {
            m_pipeline.pushMatrix();
        //        m_pipeline.translate(glm::vec3(5.0f, 2.0f, 0.0f));
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
            //    m_pipeline.Rotate(myThirdPOV_camera);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS1);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

   //     glDepthMask(GL_FALSE);

   //     glDisable(GL_DEPTH_TEST);
        r_deferredShadingRenderTechnique.disableShader(RENDER_PASS1);
    m_pipeline.popMatrix();
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

    /// Upper Right
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);      /// 2
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    /// Lower Right
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);    /// 3
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

/*
        /// Lower Right
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);    /// 3
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_DEPTH_BUFFER_BIT, GL_LINEAR);
*/
}



void ExplosionGenerator::deferredShadingLightPass()
{

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

 //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gbuffer.bindForReadingLightPass();

    m_pipeline.matrixMode(MODEL_MATRIX);
    GetLightPos_ModelView_Matrix();

    /// 2nd Render pass of shadowMapping: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);
 //   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    m_pipeline.pushMatrix();



    r_deferredShadingRenderTechnique.enableShader(RENDER_PASS2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);


        glUniformMatrix4fv(r_deferredShadingRenderTechnique.lightPosition_ModelViewMatrix_UniLoc_,1,GL_FALSE,&LightPos_modelViewMatrix[0][0]);
		glUniform3f(r_deferredShadingRenderTechnique.lightPosition_ObjectSpace_UniLoc_,
                        lightPosition.x,
                        lightPosition.y,
                        lightPosition.z);

        glUniform2f(r_deferredShadingRenderTechnique.screenSize_UniLoc_, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

		glUniform1i(r_deferredShadingRenderTechnique.positionMap_UniLoc_,0);
		glUniform1i(r_deferredShadingRenderTechnique.colorMap_UniLoc_,1);
		glUniform1i(r_deferredShadingRenderTechnique.normalMap_UniLoc_,2);

		glUniform1i(r_deferredShadingRenderTechnique.shadowMap_UniLoc_,3);


        r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS2);
        ground->draw();


        m_pipeline.pushMatrix();
            wall_negative_z->draw();
            wall_positive_x->draw();
        m_pipeline.popMatrix();


#if SPHERE_EFFECT
        r_Technique = &r_deferredShadingRenderTechnique;
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS2, sphere);
#endif
#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), cube);
#endif

        for(int i=0; i<10; i++)
        {
            m_pipeline.pushMatrix();
                m_pipeline.translate(-(float)(i*2), 12.5f, -25.0f);
                r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS2);
                light->draw();
            m_pipeline.popMatrix();
        }


       // if(!isFirstPersonCamera)
        {
            m_pipeline.pushMatrix();
        //        m_pipeline.translate(glm::vec3(5.0f, 2.0f, 0.0f));
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
            //    m_pipeline.Rotate(myThirdPOV_camera);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS2);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.DrawMyHgridFrames();
#endif

        r_deferredShadingRenderTechnique.disableShader(RENDER_PASS2);
    m_pipeline.popMatrix();
}





void ExplosionGenerator::deferredShadingGeometryPass36()
{
    /// 2nd Render pass of shadowMapping: camera's point of view
    r_deferredShadingRenderTechnique.enableShader(RENDER_PASS1);

    gbuffer.bindForWriting();


    m_pipeline.matrixMode(MODEL_MATRIX);
 //   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_pipeline.pushMatrix();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();


        m_pipeline.pushMatrix();
            wall_negative_z->draw();
            wall_positive_x->draw();
        m_pipeline.popMatrix();


#if SPHERE_EFFECT
        r_Technique = &r_deferredShadingRenderTechnique;
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

        {
            m_pipeline.pushMatrix();
        //        m_pipeline.translate(glm::vec3(5.0f, 2.0f, 0.0f));
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
            //    m_pipeline.Rotate(myThirdPOV_camera);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS1);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        r_deferredShadingRenderTechnique.disableShader(RENDER_PASS1);
    m_pipeline.popMatrix();
}

void ExplosionGenerator::beginDeferredShadingLightPass()
{

    glEnable(GL_BLEND);
    /// the GPU will simply add the source and the destination
    glBlendEquation(GL_FUNC_ADD);
    /// we want true addition, so we set it to GL_ONE, which means (1*src + 1*dst)
    glBlendFunc(GL_ONE, GL_ONE);

    // you start rendering to the screen and turn all the relevant textures active
    gbuffer.bindForReading36();
    glClear(GL_COLOR_BUFFER_BIT);

}

void ExplosionGenerator::deferredShadingPointLightPass()
{
    r_deferredShadingPointLightPass.enableShader(RENDER_PASS1);
#if 1
        r_deferredShadingPointLightPass.SetEyeWorldPos(  glm::vec3(thirdPersonPovCamera.m_eye.x,
                                                            thirdPersonPovCamera.m_eye.y,
                                                            thirdPersonPovCamera.m_eye.z));

        for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(pointLights); i++)
        {
            m_pipeline.pushMatrix();
        //    cout << "I'm drawing Point Light" << endl;
            r_deferredShadingPointLightPass.setPointLight(pointLights[i]);
            glm::vec3 transMatrix = glm::vec3(pointLights[i].position.x,
                                              pointLights[i].position.y,
                                              pointLights[i].position.z);
            m_pipeline.translate(transMatrix);
            m_pipeline.scale(r_deferredShadingPointLightPass.computePointLightBoundingSphere(pointLights[i]));
//            m_pipeline.scale(2);
            r_deferredShadingPointLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
            pointLightSphere->draw();

            m_pipeline.popMatrix();
            /*
            p.WorldPos(pointLights[i].Position);
            float BSphereScale = CalcPointLightBSphere(pointLights[i]);
            p.Scale(BSphereScale, BSphereScale, BSphereScale);
            m_DSPointLightPassTech.SetWVP(p.GetWVPTrans());
            m_bsphere->draw(PointLightPass_Shader->getProgramId());
            */
        }
#endif
    r_deferredShadingPointLightPass.disableShader(RENDER_PASS1);

}



void ExplosionGenerator::deferredShadingDirectionalLightPass()
{
  //  glDisable(GL_DEPTH_TEST);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    gbuffer.bindForReadingLightPass();

    m_pipeline.matrixMode(MODEL_MATRIX);
    GetLightPos_ModelView_Matrix();

    /// 2nd Render pass of shadowMapping: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);

    m_pipeline.pushMatrix();


    r_deferredShadingDirectionalLightPass.enableShader(RENDER_PASS1);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);


        glUniformMatrix4fv(r_deferredShadingDirectionalLightPass.lightPosition_ModelViewMatrix_UniLoc_,1,GL_FALSE,&LightPos_modelViewMatrix[0][0]);
		glUniform3f(r_deferredShadingDirectionalLightPass.lightPosition_ObjectSpace_UniLoc_,
                        lightPosition.x,
                        lightPosition.y,
                        lightPosition.z);

		glUniform1i(r_deferredShadingDirectionalLightPass.shadowMap_UniLoc_,3);

        r_deferredShadingDirectionalLightPass.SetEyeWorldPos(  glm::vec3(thirdPersonPovCamera.m_eye.x,
                                                            thirdPersonPovCamera.m_eye.y,
                                                            thirdPersonPovCamera.m_eye.z));
        r_deferredShadingDirectionalLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();


        m_pipeline.pushMatrix();
            wall_negative_z->draw();
            wall_positive_x->draw();
        m_pipeline.popMatrix();


#if SPHERE_EFFECT
        r_Technique = &r_deferredShadingDirectionalLightPass;
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif
#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), cube);
#endif

        for(int i=0; i<10; i++)
        {
            m_pipeline.pushMatrix();
                m_pipeline.translate(-(float)(i*2), 12.5f, -25.0f);
                r_deferredShadingDirectionalLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
                light->draw();
            m_pipeline.popMatrix();
        }



        {
            m_pipeline.pushMatrix();
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_deferredShadingDirectionalLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.DrawMyHgridFrames();
#endif

        r_deferredShadingDirectionalLightPass.disableShader(RENDER_PASS1);
    m_pipeline.popMatrix();
}









/*

void ExplosionGenerator::deferredShadingPointLightPass36()
{
    r_deferredShadingPointLightPass.enableShader(RENDER_PASS1);
#if 1
        r_deferredShadingPointLightPass.SetEyeWorldPos(  glm::vec3(thirdPersonPovCamera.m_eye.x,
                                                            thirdPersonPovCamera.m_eye.y,
                                                            thirdPersonPovCamera.m_eye.z));

        for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(allLights.getPointLights()); i++)
        {
            r_deferredShadingPointLightPass.setPointLight(allLights.getPointLights(i));
            glm::vec3 transMatrix = glm::vec3(allLights.getPointLights(i).position.x,
                                              allLights.getPointLights(i).position.y,
                                              allLights.getPointLights(i).position.z);
            m_pipeline.translate(transMatrix);
            m_pipeline.scale(2);
            r_deferredShadingPointLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
            pointLightSphere->draw();
        }
#endif
    r_deferredShadingPointLightPass.disableShader(RENDER_PASS1);

}
*/


void ExplosionGenerator::deferredShadingDirectionalLightPass36()
{
#if 0
    glDisable(GL_DEPTH_TEST);
  //  glDisable(GL_CULL_FACE);
    r_deferredShadingDirectionalLightPass.enableShader(RENDER_PASS1);
 //   m_pipeline.pushMatrix();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	m_pipeline.loadIdentity();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

/*
        m_pipeline.matrixMode(PROJECTION_MATRIX);
        m_pipeline.loadIdentity();
        m_pipeline.matrixMode(VIEW_MATRIX);
        m_pipeline.loadIdentity();
        m_pipeline.matrixMode(MODEL_MATRIX);
        m_pipeline.loadIdentity();
*/
        r_deferredShadingDirectionalLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
    //    deferredShadingQuad->draw();
        quad->draw();
//    m_pipeline.popMatrix();
    r_deferredShadingDirectionalLightPass.disableShader(RENDER_PASS1);
    glEnable(GL_DEPTH_TEST);
#endif
//     RenderTexture2(gbuffer.m_textures[2]);
     RenderTexture2();
}









void ExplosionGenerator::deferredShadingGeometryPass37()
{
    /// 2nd Render pass of shadowMapping: camera's point of view
    r_deferredShadingRenderTechnique.enableShader(RENDER_PASS1);

    gbuffer.BindForGeomPass37();

//    glDisable(GL_CULL_FACE);

    m_pipeline.matrixMode(MODEL_MATRIX);
    m_pipeline.pushMatrix();


        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);


        r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();


        m_pipeline.pushMatrix();
            wall_negative_z->draw();
            wall_positive_x->draw();
        m_pipeline.popMatrix();


#if SPHERE_EFFECT
        r_Technique = &r_deferredShadingRenderTechnique;
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

        {
            m_pipeline.pushMatrix();
        //        m_pipeline.translate(glm::vec3(5.0f, 2.0f, 0.0f));
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
            //    m_pipeline.Rotate(myThirdPOV_camera);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS1);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }






/*
        for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boxPositions) ; i++)
        {
            m_pipeline.pushMatrix();
                m_pipeline.translate(m_boxPositions[i]);
                r_deferredShadingRenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS1);
                m_box->draw();
            m_pipeline.popMatrix();
        }
*/





        glDepthMask(GL_FALSE);
        r_deferredShadingRenderTechnique.disableShader(RENDER_PASS1);
    m_pipeline.popMatrix();
}


void ExplosionGenerator::deferredShadingStencilPass37(int index)
{
    r_deferredShadingStencilPass.enableShader(RENDER_PASS1);

        // Disable color/depth write and enable stencil
        gbuffer.BindForStencilPass37();
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
        glm::vec3 transMatrix = glm::vec3(pointLights[index].position.x,
                                          pointLights[index].position.y,
                                          pointLights[index].position.z);
        m_pipeline.translate(transMatrix);
        m_pipeline.scale(r_deferredShadingPointLightPass.computePointLightBoundingSphere(pointLights[index]));
//        m_pipeline.scale(2);
        r_deferredShadingStencilPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        pointLightSphere->draw();
        m_pipeline.popMatrix();

    r_deferredShadingStencilPass.disableShader(RENDER_PASS1);
}



void ExplosionGenerator::deferredShadingPointLightPass37(int index)
{
    gbuffer.BindForLightPass37();

    r_deferredShadingPointLightPass.enableShader(RENDER_PASS1);

        r_deferredShadingPointLightPass.SetEyeWorldPos(  glm::vec3(thirdPersonPovCamera.m_eye.x,
                                                            thirdPersonPovCamera.m_eye.y,
                                                            thirdPersonPovCamera.m_eye.z));
//        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

      //  glFrontFace(GL_CW);


        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);


        m_pipeline.pushMatrix();
        //    cout << "I'm drawing Point Light" << endl;

        glm::vec3 transMatrix = glm::vec3(pointLights[index].position.x,
                                          pointLights[index].position.y,
                                          pointLights[index].position.z);
        m_pipeline.translate(transMatrix);
        m_pipeline.scale(r_deferredShadingPointLightPass.computePointLightBoundingSphere(pointLights[index]));
//        m_pipeline.scale(2);
        r_deferredShadingPointLightPass.setPointLight(pointLights[index]);
        r_deferredShadingPointLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);

        pointLightSphere->draw();



    //    glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glDisable(GL_BLEND);

        m_pipeline.popMatrix();
    r_deferredShadingPointLightPass.disableShader(RENDER_PASS1);
}

void ExplosionGenerator::deferredShadingDirectionalLightPass37()
{
    r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->useShader();
        gbuffer.BindForLightPass37();

        m_pipeline.loadIdentity();

        m_pipeline.matrixMode(PROJECTION_MATRIX);
        m_pipeline.pushMatrix();
        m_pipeline.ortho(-1,1,-1,1,-1,1);



        r_deferredShadingDirectionalLightPass.SetEyeWorldPos(  glm::vec3(thirdPersonPovCamera.m_eye.x,
                                                        thirdPersonPovCamera.m_eye.y,
                                                        thirdPersonPovCamera.m_eye.z));

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        r_deferredShadingDirectionalLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);

        quad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());
        m_pipeline.popMatrix();


        glDisable(GL_BLEND);

	r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->delShader();

}


void ExplosionGenerator::deferredShadingFinalPass37()
{
    gbuffer.BindForFinalPass37();
    glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                      0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}




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


///************************Helper Functions**************************///
void ExplosionGenerator::RenderTexture(GLuint TextureId)
{

    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	m_pipeline.loadIdentity();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	quadRenderShader->useShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,TextureId);
	glUniform1i(glGetUniformLocation(quadRenderShader->getProgramId(),"texture"),0);
	glUniform2f(glGetUniformLocation(quadRenderShader->getProgramId(),"pixelSize"),1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT);
	m_pipeline.updateMatrices(quadRenderShader->getProgramId());
	quad->draw(quadRenderShader->getProgramId());
//    deferredShadingQuad->draw(quadRenderShader->getProgramId());
	quadRenderShader->delShader();

    glEnable(GL_DEPTH_TEST);
}



// void ExplosionGenerator::RenderTexture2(GLuint TextureId)
void ExplosionGenerator::RenderTexture2()
{

  //  m_pipeline.pushMatrix();
//    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //render texture to screen
	m_pipeline.loadIdentity();

    m_pipeline.matrixMode(PROJECTION_MATRIX);
	m_pipeline.pushMatrix();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
	r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->useShader();
    gbuffer.bindForReadingLightPass();
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D,TextureId);
//	glUniform1i(glGetUniformLocation(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId(),"texture1"),0);

    r_deferredShadingDirectionalLightPass.SetEyeWorldPos(  glm::vec3(thirdPersonPovCamera.m_eye.x,
                                                    thirdPersonPovCamera.m_eye.y,
                                                    thirdPersonPovCamera.m_eye.z));


//	m_pipeline.updateMatrices(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());
    r_deferredShadingDirectionalLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);

	quad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());
    m_pipeline.popMatrix();


    /// this deferredShading Quad is motherFxxking from the OpenGL UK, so it's reversed.. so you have to have
//    glDisable(GL_CULL_FACE);
//    deferredShadingQuad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());


	r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->delShader();


 //   glEnable(GL_DEPTH_TEST);
 //   m_pipeline.popMatrix();
}



void ExplosionGenerator::setupCamera()
{
  //  cam = t_camera(lightPosition, lightDirection.x, lightDirection.y);

    firstPersonPovCamera = EG_FirstPersonPovCamera(camPosition);
}


void ExplosionGenerator::setupColor_Texture()
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



void ExplosionGenerator::RenderReflectiveObjects()
{

    r_Technique = &r_Reflection_Render;
    m_pipeline.pushMatrix();
    r_Technique->enableShader(RENDER_PASS1);
        m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);
      //  glUniform3f( r_Reflection_Render.CameraPosition_UniformLocation, cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);



        if(isFirstPersonCamera)
            glUniform3f( r_Reflection_Render.CameraPosition_UniformLocation, firstPersonPovCamera.getEyePoint().x,
                                                                             firstPersonPovCamera.getEyePoint().y,
                                                                             firstPersonPovCamera.getEyePoint().z);
        else
        {
            glUniform3f( r_Reflection_Render.CameraPosition_UniformLocation, thirdPersonPovCamera.m_eye.x,
                                                                             thirdPersonPovCamera.m_eye.y,
                                                                             thirdPersonPovCamera.m_eye.z);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.Dynamic_CubeMap_ColorTextureID);
        glUniform1i(r_Reflection_Render.CubeMap_UniformLocation,0);

     //   m_pipeline.updateMatrices(ReflectionShader->getProgramId());
        r_Reflection_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);
        smoothSphere->draw();

    r_Technique->disableShader(RENDER_PASS1);
    m_pipeline.popMatrix();

}



void ExplosionGenerator::RenderSmoke(bool pass1, bool pass2, Matrices_t& Mat, unsigned int depthTextureId)
{
    if(pass1)
    {
        /// First pass of the RayCasting
        Matrices.View = m_pipeline.getViewMatrix();
     //   Matrices.View = glm::mat4(1.0);

        m_pipeline.pushMatrix();

        #if USING_INVERSE_MATRIX
            g_meshOrientation = myThirdPOV_camera.m_orientation;
            g_meshOrientation = glm::inverse(g_meshOrientation);
            glm::mat4 m = glm::toMat4(g_meshOrientation);
            m_pipeline.LoadMatrix(m);
        #endif

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
        glBindTexture(GL_TEXTURE_3D, smoke.f_Slab.Density.Ping.ColorTexture);

        /// the volume RayCasting part
        r_TwoPass_Render.Render_TwoPass_RayCasting_2(Mat, depthTextureId);
    }

}

void ExplosionGenerator::RenderScene()
{
    m_pipeline.matrixMode(MODEL_MATRIX);
    GetLightPos_ModelView_Matrix();

    /// 2nd Render pass of shadowMapping: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);
 //   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    m_pipeline.pushMatrix();

#if USING_INVERSE_MATRIX
    g_meshOrientation = myThirdPOV_camera.m_orientation;
    g_meshOrientation = glm::inverse(g_meshOrientation);
    glm::mat4 m = glm::toMat4(g_meshOrientation);
    m_pipeline.LoadMatrix(m);
#endif

    r_Shadow_Render.enableShader(RENDER_PASS2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);

		glUniformMatrix4fv(r_Shadow_Render.LightMVPmatrix_UniLoc,1,GL_FALSE,&shadowMatrix[0][0]);
		glUniform1i(r_Shadow_Render.shadowMap_UniLoc,0);

        glUniformMatrix4fv(r_Shadow_Render.LightPosition_MVmatrix_UniLoc,1,GL_FALSE,&LightPos_modelViewMatrix[0][0]);
		glUniform3f(r_Shadow_Render.LightPosition_UniLoc,
                        lightPosition.x,
                        lightPosition.y,
                        lightPosition.z);
		glUniform3f(r_Shadow_Render.CameraPosition_UniLoc,firstPersonPovCamera.getLocation().x,firstPersonPovCamera.getLocation().y,firstPersonPovCamera.getLocation().z);

//        r_Shadow_Render.loadSpecialUniformLocation(m_pipeline, RENDER_PASS2);

    //    m_pipeline.pushMatrix();
      //  glm::vec3 EulerAngles(45, 45, 0);
      //  glm::quat rotation_quaternion = glm::quat(EulerAngles);
      //  m_pipeline.Rotate(rotation_quaternion);

//        m_pipeline.Rotate(22.5, 1, 1, 0);

        r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS2);
        ground->draw();
      //  m_pipeline.popMatrix();
//        m_pipeline.pushMatrix();
//            m_pipeline.rotateX(45);
//            m_pipeline.rotateY(45);
//            r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS2);
//            ground->draw();
//        m_pipeline.popMatrix();

#if SPHERE_EFFECT
        r_Technique = &r_Shadow_Render;
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS2, sphere);
#endif
#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), cube);
#endif

       // if(!isFirstPersonCamera)
        {
            m_pipeline.pushMatrix();
        //        m_pipeline.translate(glm::vec3(5.0f, 2.0f, 0.0f));
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
            //    m_pipeline.Rotate(myThirdPOV_camera);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS2);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.DrawMyHgridFrames();
#endif

        r_Shadow_Render.disableShader(RENDER_PASS2);
    m_pipeline.popMatrix();
}






enum CubeMapOrientation
{   POSITIVE_X=0, NEGATIVE_X, POSITIVE_Y,
    NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z};


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


    switch (face)
    {
        case POSITIVE_X:
            m_pipeline.rotateZ(180);
            m_pipeline.rotateY(-90);
            break;

        case NEGATIVE_X:
            m_pipeline.rotateZ(180);
            m_pipeline.rotateY(90);
            break;

        case POSITIVE_Y:
            m_pipeline.rotateX(90);
            break;

        case NEGATIVE_Y:
            m_pipeline.rotateX(-90);
            break;

        case POSITIVE_Z:
            m_pipeline.rotateZ(180);
            m_pipeline.rotateY(180);
            break;

        case NEGATIVE_Z:
            m_pipeline.rotateZ(180);
            break;
        default:
            break;
    };


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




void ExplosionGenerator::drawAxis(float fScale, glm::vec3 translate)
{
    glPushAttrib( GL_ENABLE_BIT );

 //   glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    glPushMatrix();
        glTranslatef( translate.x, translate.y, translate.z );
        glScalef( fScale, fScale, fScale );
        glColor3f( 0.0f, 0.0f, 1.0f );

        glBegin( GL_LINES );
        {
            // x axis: red
            glColor3f( 1.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 0.0 );
            glVertex3f( 1.0f, 0.0f, 0.0f );

            // y axis: green
            glColor3f( 0.0f, 1.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 1.0f, 0.0f );

            // zaxis: blue
            glColor3f( 0.0f, 0.0f, 1.0f );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 1.0f );
        }
        glEnd();

    glPopMatrix();
    glPopAttrib();

}


void ExplosionGenerator::drawAxis(float fScale, pipeline& m_pipeline, glm::vec3 translate)
{
 //   glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    m_pipeline.pushMatrix();
    glPushMatrix();

        glTranslatef( translate.x, translate.y, translate.z );
        glScalef( fScale, fScale, fScale );
        glColor3f( 0.0f, 0.0f, 1.0f );

        m_pipeline.matrixMode(MODEL_MATRIX);
        m_pipeline.translate(translate.x, translate.y, translate.z);
        m_pipeline.scale(fScale);

        glBegin( GL_LINES );
        {
            // x axis: red
            glColor3f( 1.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 0.0 );
            glVertex3f( 1.0f, 0.0f, 0.0f );

            // y axis: green
            glColor3f( 0.0f, 1.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 1.0f, 0.0f );

            // zaxis: blue
            glColor3f( 0.0f, 0.0f, 1.0f );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 1.0f );
        }
        glEnd();

    glPopMatrix();
    m_pipeline.popMatrix();
    glEnable(GL_LIGHTING);
//   glPopAttrib();
}


void ExplosionGenerator::drawCubeFrame(float size, int offset = 0)
{
    glPushMatrix();
        glDisable( GL_LIGHTING );
        glBegin(GL_LINES);
            // front face
            glColor3f(1.0,0.0,1.0);
            glVertex3f(size/2, size/2 +offset, size/2);
            glVertex3f(-size/2, size/2 +offset, size/2);

            glVertex3f(-size/2, -size/2 +offset, size/2);
            glVertex3f(size/2, -size/2 +offset, size/2);

            glVertex3f(-size/2, size/2 +offset, size/2);
            glVertex3f(-size/2, -size/2 +offset, size/2);

            glVertex3f(size/2, size/2 +offset, size/2);
            glVertex3f(size/2, -size/2 +offset, size/2);


            // back face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, size/2 +offset, -size/2);
            glVertex3f(-size/2, size/2+offset, -size/2);

            glVertex3f(-size/2, -size/2+offset, -size/2);
            glVertex3f(size/2, -size/2+offset, -size/2);

            glVertex3f(size/2, size/2 +offset, -size/2);
            glVertex3f(size/2, -size/2+offset, -size/2);

            glVertex3f(-size/2, size/2+offset, -size/2);
            glVertex3f(-size/2, -size/2+offset, -size/2);

            // left face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(-size/2, size/2+offset, size/2);
            glVertex3f(-size/2, size/2+offset, -size/2);
            glVertex3f(-size/2, -size/2+offset, -size/2);
            glVertex3f(-size/2, -size/2+offset, size/2);

            // right face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, size/2+offset, -size/2);
            glVertex3f(size/2, size/2+offset, size/2);
            glVertex3f(size/2, -size/2+offset, size/2);
            glVertex3f(size/2, -size/2+offset, -size/2);

        glEnd();
        glEnable( GL_LIGHTING );
    glPopMatrix();
}


void ExplosionGenerator::drawCube(float size)
{
    glBegin(GL_QUADS);
        // OpenGL use normal vectors to calculate light
        // if we don't ENABLE(COLOR_MATERIAL)

        // we can manually create a material color here
//       glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, difamb);

        // front face
        glNormal3f(0.0,0.0,1.0);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, size/2);
        glVertex3f(-size/2, -size/2, size/2);
        glVertex3f(size/2, -size/2, size/2);

        // back face
        glNormal3f(0.0,0.0,-1.0);
        glColor3f(0.0,0.0,1.0);
        glVertex3f(size/2, size/2, -size/2);
        glVertex3f(-size/2, size/2, -size/2);
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(size/2, -size/2, -size/2);

        // left face
        glNormal3f(-1.0,0.0,0.0);
        glColor3f(0.0,1.0,0.0);
        glVertex3f(-size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, -size/2);
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(-size/2, -size/2, size/2);

        // right face
        glNormal3f(1.0,0.0,0.0);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(size/2, size/2, -size/2);
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(size/2, -size/2, size/2);
        glVertex3f(size/2, -size/2, -size/2);

        // top face
        glNormal3f(0.0,1.0,0.0);
        glColor3f(1.0,0.0,1.0);
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, -size/2);
        glVertex3f(size/2, size/2, -size/2);

        // bottom face
        glNormal3f(0.0,-1.0,0.0);
        glVertex3f(size/2, -size/2, size/2);
        glVertex3f(-size/2, -size/2, size/2);
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(size/2, -size/2, -size/2);
    glEnd();
}

void ExplosionGenerator::drawGround(float size, unsigned int textureId)
{
    glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDisable( GL_LIGHTING );
    glBegin(GL_QUADS);

        glNormal3f(0.0,1.0,0.0);
        glColor3f(1.0,1.0,1.0);
        glTexCoord2f(0.0,2.0);
        glVertex3f(size/2, 0, size/2);

        glTexCoord2f(0.0,0.0);
        glVertex3f(-size/2, 0, size/2);

        glTexCoord2f(2.0,0.0);
        glVertex3f(-size/2, 0, -size/2);

        glTexCoord2f(2.0,2.0);
        glVertex3f(size/2, 0, -size/2);
    glEnd();
    glEnable( GL_LIGHTING );
    glBindTexture(GL_TEXTURE_2D,0);
    glEnable(GL_CULL_FACE);
}

