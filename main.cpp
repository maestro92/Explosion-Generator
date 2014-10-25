#include "main.h"

#include "gl/gl.h"
#include "gl/glu.h"

#include "SDL\SDL.h"

using namespace std;




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



PivotCamera g_Camera;


glm::vec2 g_MouseCurrent = glm::vec2(0);
glm::vec2 g_MousePrevious = glm::vec2(0);
glm::vec2 g_MouseDelta = glm::vec2(0);


//glm::vec3 g_DefaultCameraTranslate( 0, 0, 400 );
glm::vec3 g_DefaultCameraTranslate( 0, 0, 0 );
// so positive y axis faces downwards
glm::vec3 g_DefaultCameraRotate( 0, 0, 0 );
glm::vec3 g_DefaultCameraPivot( 0, 0, 0 );






// Textures
GLuint shadowMapTexture;



int shadowMapWidth = SCREEN_WIDTH * 2;
int shadowMapheight = SCREEN_HEIGHT * 2;

bool space_bar = false;


//vector3d lightPosition(-11.0865, 13.8124, 21.3599);
vector3d lightDirection(-29, 330.6);

vector3d lightPosition(-19.1004, 28.881, 40.5246);
//vector3d lightDirection(-37, 335.4);

ExplosionGenerator::ExplosionGenerator()
{
    field_length = 20;
    angle = 0;
    running = true;
    dvel = false;
    addSmoke = false;

    g_bLeftMouseDown = false;
    g_bRightMouseDown = false;


    init_SDL_GLEW();
    init_OpenGL();

    init_Shader();
    init_Models();
    init_Lighting();

    init_Texture_and_FrameBuffer();



    setupCamera();
    setupColor_Texture();



    l_CubeEffect.InitParticleCube();


    SDL_WM_SetCaption("Template", NULL);




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
      /* Problem: glewInit failed, something is seriously wrong. */
      // fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
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


    shadow_depthTexture = createTexture(shadowMapWidth, shadowMapheight, true);
    glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);


 //   renderTexture=createTexture(SCREEN_WIDTH,SCREEN_HEIGHT);
//	depthTexture=createTexture(SCREEN_WIDTH,SCREEN_HEIGHT,true);
    glGenFramebuffers(1,&FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);
	//GL_COLOR_ATTACHMENT0
	//GL_DEPTH_ATTACHMENT
//	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture,0);

//	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture,0);

    int i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not OK, status=" << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		//1.
		tmp.position.change(-1.0,1.0,0.0);
		tmp.U=0;
		tmp.V=1;
		vertices.push_back(tmp);
		//2.
		tmp.position.change(-1.0,-1.0,0.0);
		tmp.U=0;
		tmp.V=0;
		vertices.push_back(tmp);
		//3.
		tmp.position.change(1.0,-1.0,0.0);
		tmp.U=1;
		tmp.V=0;
		vertices.push_back(tmp);
		//4.
		tmp.position.change(1.0,1.0,0.0);
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
}






#if 1
void ExplosionGenerator::init_OpenGL()
{

    //Initialize clear color
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    m_pipeline.perspective(45, SCREEN_WIDTH/SCREEN_HEIGHT, 1,1000.0);
    m_pipeline.matrixMode(MODEL_MATRIX);


#if 1
    //Initialize Projection Mode    (We initialize the camera lens)
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // creates a pyramid
    gluPerspective( 45,             // the camera angle
                    SCREEN_WIDTH/SCREEN_HEIGHT,    // width to height ratio
                    1.0,            // the near z clippFing coordinate
                    1000.0);         // the far z clipping coordinate
                                    // clipping coordinate: the cutoff range


    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
 //   glLoadIdentity();
#endif

    glEnable(GL_DEPTH_TEST);

}
#endif


void ExplosionGenerator::init_Shader()
{
//    ObjShader = new shader("m_obj_new.vs", "m_obj_new.fs");
    ObjShader = new shader("m_obj2_Spot.vs", "m_obj2_Spot.fs");
	quadRenderShader=new shader("quadRender.vs","quadRender.frag"); // rendering texture as a quad
    shadow_FirstRender = new shader("shadow_FirstRender.vs", "shadow_FirstRender.fs");
    shadow_SecondRender = new shader("shadow_SecondRender2.vs", "shadow_SecondRender2.fs");
  //  shadow_SecondRender = new shader("shadow_SecondRender.vs", "shadow_SecondRender.fs");
}


void ExplosionGenerator::init_Models()
{

    // most for shadow mapping
    scene = new meshLoader("shadow.obj");
    ground = new meshLoader("ground.obj");
    ground1 = new meshLoader("quad12.obj");
    sphere = new meshLoader("sphere20.obj");
    monkeyMesh = new meshLoader("monkey1.obj");
  //  sphere = new meshLoader("phoenix_ugv.md2");
/*
    if (!m_pLightingEffect->Init()) {
        printf("Error initializing the lighting technique\n");
        return false;
    }
*/
}





void ExplosionGenerator::start()
{
    printf("nice");

    Uint32 startTime = SDL_GetTicks();
    Uint32 next_game_tick = 0;
    Uint32 delay_time = 0;
    while(running)
    {
        startTime = SDL_GetTicks();
        while(SDL_PollEvent(&event))
		{
            int tmpx,tmpy;
            switch(event.type)
			{
                case SDL_QUIT:
                    running = false;
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
                    cam.mouseIn(true);
                    cam.lookAt(lightDirection.x, lightDirection.y);

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
                            break;

                        case SDL_BUTTON_WHEELDOWN:
                            break;
                    }
                    break;



                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_x:   addSmoke = false;    break;
//
                    }
                    break;


                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:   running = false;    break;
                        case SDLK_z:
                            // e_CubeEffect.Reset();
                            l_CubeEffect.Reset();
                            space_bar = false;
                            break;
                        case SDLK_SPACE:
                            space_bar = true;
                            break;
                        case SDLK_v:
                            dvel = !dvel;
                            break;
                        case SDLK_x:
                            cout << "here" << endl;
                            addSmoke = true;

                            l_CubeEffect.myHgrid.RemoveParticleFromHGrid(&l_CubeEffect.e_ParticleBuffer[3]);

                            break;
                    }
                    break;
			}
        }
            update();
      //      reshape();
          //  PointLight_show();
          //  SpotLight_show();
            show();
          //  shadow_show();


            SDL_GL_SwapBuffers();

          //  next_game_tick += INTERVAL;
         //   delay_time = next_game_tick - SDL_GetTicks();


            if (next_game_tick > SDL_GetTicks())
                SDL_Delay(next_game_tick - SDL_GetTicks());
            next_game_tick = SDL_GetTicks() + INTERVAL;

    }

}



//GLuint nice1;

void ExplosionGenerator::init_Lighting()
{
    Specular_Intensity = 0.0f;
    Specular_Power = 0.0f;

    // set any directional light you have
    m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.00f;
    m_directionalLight.DiffuseIntensity = 0.00f;
    m_directionalLight.Direction = glm::vec3(1.0f, -0.5, 0.0);

    // set any point light you have
    pl[0].DiffuseIntensity = 0.5f;
    pl[0].Color = glm::vec3(1.0f, 0.5f, 0.0f);
    pl[0].Position = glm::vec3(3.0f, 1.0f, 0.0f);
    pl[0].Attenuation.Linear = 0.1f;



    pl[1].DiffuseIntensity = 0.5f;
    pl[1].Color = glm::vec3(0.0f, 0.5f, 1.0f);
    pl[1].Position = glm::vec3(7.0f, 1.0f, field_length * (sinf(angle) + 1.0f) / 2.0f);
    pl[1].Attenuation.Linear = 0.1f;
    // set any spot light you have


  //  sl[0].AmbientIntensity = 0.1f;
    sl[0].DiffuseIntensity = 0.9f;
    sl[0].Color = glm::vec3(1.0f, 1.0f, 1.0f);
    sl[0].Position  = glm::vec3(1.0, 3.0, 0.0f);
    sl[0].Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    sl[0].Attenuation.Linear = 0.1f;
    sl[0].Cutoff = 20.0f;


    m_LightingEffect = new Lighting_Technique();
    m_LightingEffect->Init(ObjShader->getProgramId());


/*
    m_spotLight.AmbientIntensity = 0.1f;
    m_spotLight.DiffuseIntensity = 0.9f;
    m_spotLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_spotLight.Attenuation.Linear = 0.01f;
    m_spotLight.Position = glm::vec3(-20.0, 20.0, 1.0f);
    m_spotLight.Direction = glm::vec3(1.0f, -1.0f, 0.0f);
    m_spotLight.Cutoff =  20.0f;
*/
}





// bool isDepth: true if it's depth texture
unsigned int ExplosionGenerator::createTexture(int w, int h, bool isDepth)
{
    unsigned int textureID;

    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexImage2D(GL_TEXTURE_2D,0,(!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT),w,h,0,isDepth ? GL_DEPTH_COMPONENT : GL_RGBA,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    int i;
    i = glGetError();
    if(i!=0)
    {
        std::cout << "Error happened while loading the texture: " << i << std::endl;
    }
    // unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}



// we first create a depthTexture: shadow_depthTexture
// then we render to it with glEnable(GL_DEPTH_TEST)


void ExplosionGenerator::getDepthTexture_FromLightPosion()
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadIdentity();

    m_pipeline.rotateX(lightDirection.x);
    m_pipeline.rotateY(lightDirection.y);
    m_pipeline.translate(lightPosition.x,lightPosition.y, lightPosition.z);


    Light_ModelMatrix = m_pipeline.getModelMatrix();
    Light_ViewMatrix = m_pipeline.getViewMatrix();
    Light_ProjectionMatrix = m_pipeline.getProjectionMatrix();
    Light_ModelViewProjectionMatrix = Light_ProjectionMatrix * Light_ViewMatrix * Light_ModelMatrix;

    m_pipeline.updateLightMatrix(Light_ModelMatrix, Light_ViewMatrix, Light_ProjectionMatrix);

    Light_BiasMatrix[0][0]=0.5;Light_BiasMatrix[0][1]=0.0;Light_BiasMatrix[0][2]=0.0;Light_BiasMatrix[0][3]=0.0;
	Light_BiasMatrix[1][0]=0.0;Light_BiasMatrix[1][1]=0.5;Light_BiasMatrix[1][2]=0.0;Light_BiasMatrix[1][3]=0.0;
	Light_BiasMatrix[2][0]=0.0;Light_BiasMatrix[2][1]=0.0;Light_BiasMatrix[2][2]=0.5;Light_BiasMatrix[2][3]=0.0;
	Light_BiasMatrix[3][0]=0.5;Light_BiasMatrix[3][1]=0.5;Light_BiasMatrix[3][2]=0.5;Light_BiasMatrix[3][3]=1.0;

    shadowMatrix = Light_BiasMatrix * Light_ModelViewProjectionMatrix;


    glViewport(0,0,shadowMapWidth, shadowMapheight);
	m_pipeline.matrixMode(MODEL_MATRIX);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);       // we don't render the front, the moisser pattern doesn't appear in the front
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_depthTexture, 0);

    glEnable(GL_DEPTH_TEST);
    shadow_FirstRender->useShader();
        glClear(GL_DEPTH_BUFFER_BIT);
        m_pipeline.updateMatrices(shadow_FirstRender->getProgramId());
        ground->draw(shadow_FirstRender->getProgramId());

        l_CubeEffect.show(m_pipeline, shadow_FirstRender->getProgramId(), sphere);
    shadow_FirstRender->delShader();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_CULL_FACE);
    glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
}


void ExplosionGenerator::updateShadowMatrix(unsigned int shaderId)
{
    Light_ModelMatrix = m_pipeline.getModelMatrix();
    Light_ModelViewProjectionMatrix = Light_ProjectionMatrix * Light_ViewMatrix * Light_ModelMatrix;
    shadowMatrix = Light_BiasMatrix * Light_ModelViewProjectionMatrix;

		glUniformMatrix4fv(glGetUniformLocation(shaderId,"lightModelViewProjectionMatrix"),1,GL_FALSE,&shadowMatrix[0][0]);

}









void ExplosionGenerator::PointLight_show()
{

    glClearColor(0,0,0.5,1);

    m_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT,1.0, 1000.0);
    glEnable(GL_DEPTH_TEST);



///First render pass: light's point of view
    getDepthTexture_FromLightPosion();


///camera motion
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	m_pipeline.matrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();

    cam.Control();
    cam.UpdateCamera();
    cam.UpdateCamera(m_pipeline);


  //  cout << "cam.x " << cam.getLocation().x << " cam.y " << cam.getLocation().y << " cam.z " << cam.getLocation().z << endl;
  //  cout << "cam pitch " << cam.getPitch() << " cam yaw " << cam.getYaw() << endl;

    // get modelViewMatrix for LightPosition, you essentially just treat the light pos like a regular object
    // exactly the same of how you get the ModelView Matrix of a sphere.
    // push -> translate -> m_pipeline.update   which includes updating the modelView Matrix of that sphere
    // here is exactly the same

    m_pipeline.matrixMode(MODEL_MATRIX);
    glm::mat4 LightPos_viewMatrix = m_pipeline.getViewMatrix();
    m_pipeline.pushMatrix();
        m_pipeline.translate(lightPosition.x, lightPosition.y, lightPosition.z);
        glm::mat4 LightPos_modelMatrix = m_pipeline.getModelMatrix();
    m_pipeline.popMatrix();
    glm::mat4 LightPos_modelViewMatrix = LightPos_viewMatrix * LightPos_modelMatrix;


    m_LightingEffect->LoadingModeViewMatrix_ForPointLights(m_pipeline, pl, LightPos_viewMatrix, 0);
    m_LightingEffect->LoadingModeViewMatrix_ForPointLights(m_pipeline, pl, LightPos_viewMatrix, 1);




///2nd Render pass: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_pipeline.pushMatrix();
    shadow_SecondRender->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);
		glUniformMatrix4fv(glGetUniformLocation(shadow_SecondRender->getProgramId(),"lightModelViewProjectionMatrix"),1,GL_FALSE,&shadowMatrix[0][0]);
		glUniform1i(glGetUniformLocation(shadow_SecondRender->getProgramId(),"shadowMap"),0);

        glUniformMatrix4fv(glGetUniformLocation(shadow_SecondRender->getProgramId(),"LightPosition_ModelViewMatrix"),1,GL_FALSE,&LightPos_modelViewMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shadow_SecondRender->getProgramId(),"LightPosition"),lightPosition.x,lightPosition.y,lightPosition.z);
		glUniform3f(glGetUniformLocation(shadow_SecondRender->getProgramId(),"cameraPosition"),cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);


      //  m_pipeline.updateMatrices(shadow_SecondRender->getProgramId());
        ground->draw(shadow_SecondRender->getProgramId());

      //  l_CubeEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), sphere);
    shadow_SecondRender->delShader();
    m_pipeline.popMatrix();

    /// remember to turn off shader when draw using fixed pipeline functionality
    DrawAxis( 20.0f, m_pipeline);










    m_pipeline.pushMatrix();
    ObjShader->useShader();

		glUniform3f(glGetUniformLocation(ObjShader->getProgramId(),"cameraPosition"),cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);

        m_pipeline.updateMatrices(ObjShader->getProgramId());
        m_LightingEffect->SetDirectionalLight(m_directionalLight);
        m_LightingEffect->SetMatSpecularIntensity(Specular_Intensity);
        m_LightingEffect->SetMatSpecularPower(Specular_Power);

        m_LightingEffect->SetPointLights(2, pl);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        ground1->draw(ObjShader->getProgramId());

    ObjShader->delShader();
    glBindTexture(GL_TEXTURE_2D,0);
    m_pipeline.popMatrix();

#if 0
    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	m_pipeline.loadIdentity();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	quadRenderShader->useShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,renderTexture);
	glUniform1i(glGetUniformLocation(quadRenderShader->getProgramId(),"texture"),0);
	glUniform2f(glGetUniformLocation(quadRenderShader->getProgramId(),"pixelSize"),1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT);
	m_pipeline.updateMatrices(quadRenderShader->getProgramId());
	quad->draw(quadRenderShader->getProgramId());
	quadRenderShader->delShader();

    glEnable(GL_DEPTH_TEST)
#endif

}




void ExplosionGenerator::SpotLight_show()
{

    glClearColor(0,0,0.5,1);

    m_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT,1.0, 1000.0);
    glEnable(GL_DEPTH_TEST);



///First render pass: light's point of view
    getDepthTexture_FromLightPosion();


///camera motion
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	m_pipeline.matrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();

    cam.Control();
    cam.UpdateCamera();
    cam.UpdateCamera(m_pipeline);


  //  cout << "cam.x " << cam.getLocation().x << " cam.y " << cam.getLocation().y << " cam.z " << cam.getLocation().z << endl;
  //  cout << "cam pitch " << cam.getPitch() << " cam yaw " << cam.getYaw() << endl;

    // get modelViewMatrix for LightPosition, you essentially just treat the light pos like a regular object
    // exactly the same of how you get the ModelView Matrix of a sphere.
    // push -> translate -> m_pipeline.update   which includes updating the modelView Matrix of that sphere
    // here is exactly the same

    m_pipeline.matrixMode(MODEL_MATRIX);
    glm::mat4 LightPos_viewMatrix = m_pipeline.getViewMatrix();
    m_pipeline.pushMatrix();
        m_pipeline.translate(lightPosition.x, lightPosition.y, lightPosition.z);
        glm::mat4 LightPos_modelMatrix = m_pipeline.getModelMatrix();
    m_pipeline.popMatrix();
    glm::mat4 LightPos_modelViewMatrix = LightPos_viewMatrix * LightPos_modelMatrix;


    m_LightingEffect->LoadingModeViewMatrix_ForPointLights(m_pipeline, pl, LightPos_viewMatrix, 0);
    m_LightingEffect->LoadingModeViewMatrix_ForPointLights(m_pipeline, pl, LightPos_viewMatrix, 1);

    m_LightingEffect->LoadingModeViewMatrix_ForSpotLights(m_pipeline, sl, LightPos_viewMatrix, 0);



///2nd Render pass: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_pipeline.pushMatrix();
    shadow_SecondRender->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);
		glUniformMatrix4fv(glGetUniformLocation(shadow_SecondRender->getProgramId(),"lightModelViewProjectionMatrix"),1,GL_FALSE,&shadowMatrix[0][0]);
		glUniform1i(glGetUniformLocation(shadow_SecondRender->getProgramId(),"shadowMap"),0);

        glUniformMatrix4fv(glGetUniformLocation(shadow_SecondRender->getProgramId(),"LightPosition_ModelViewMatrix"),1,GL_FALSE,&LightPos_modelViewMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shadow_SecondRender->getProgramId(),"LightPosition"),lightPosition.x,lightPosition.y,lightPosition.z);
		glUniform3f(glGetUniformLocation(shadow_SecondRender->getProgramId(),"cameraPosition"),cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);


        m_pipeline.updateMatrices(shadow_SecondRender->getProgramId());
    //    ground->draw(shadow_SecondRender->getProgramId());

    //    l_CubeEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), sphere);
    shadow_SecondRender->delShader();
    m_pipeline.popMatrix();

    /// remember to turn off shader when draw using fixed pipeline functionality
    DrawAxis( 20.0f, m_pipeline);





    m_pipeline.pushMatrix();
    ObjShader->useShader();

		glUniform3f(glGetUniformLocation(ObjShader->getProgramId(),"cameraPosition"),cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);

        m_pipeline.updateMatrices(ObjShader->getProgramId());
        m_LightingEffect->SetDirectionalLight(m_directionalLight);
        m_LightingEffect->SetMatSpecularIntensity(Specular_Intensity);
        m_LightingEffect->SetMatSpecularPower(Specular_Power);

        m_LightingEffect->SetPointLights(2, pl);
        m_LightingEffect->SetSpotLights(1, sl);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        ground1->draw(ObjShader->getProgramId());

    ObjShader->delShader();
    glBindTexture(GL_TEXTURE_2D,0);
    m_pipeline.popMatrix();

}











void ExplosionGenerator::show()
{

    glClearColor(0,0,0.5,1);

    m_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT,1.0, 1000.0);
    glEnable(GL_DEPTH_TEST);



///First render pass: light's point of view
    getDepthTexture_FromLightPosion();


///camera motion
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	m_pipeline.matrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();

    cam.Control();
    cam.UpdateCamera();
    cam.UpdateCamera(m_pipeline);


  //  cout << "cam.x " << cam.getLocation().x << " cam.y " << cam.getLocation().y << " cam.z " << cam.getLocation().z << endl;
  //  cout << "cam pitch " << cam.getPitch() << " cam yaw " << cam.getYaw() << endl;

    // get modelViewMatrix for LightPosition, you essentially just treat the light pos like a regular object
    // exactly the same of how you get the ModelView Matrix of a sphere.
    // push -> translate -> m_pipeline.update   which includes updating the modelView Matrix of that sphere
    // here is exactly the same

    m_pipeline.matrixMode(MODEL_MATRIX);
    glm::mat4 LightPos_viewMatrix = m_pipeline.getViewMatrix();
    m_pipeline.pushMatrix();
        m_pipeline.translate(lightPosition.x, lightPosition.y, lightPosition.z);
        glm::mat4 LightPos_modelMatrix = m_pipeline.getModelMatrix();
    m_pipeline.popMatrix();
    glm::mat4 LightPos_modelViewMatrix = LightPos_viewMatrix * LightPos_modelMatrix;


    m_LightingEffect->LoadingModeViewMatrix_ForPointLights(m_pipeline, pl, LightPos_viewMatrix, 0);
    m_LightingEffect->LoadingModeViewMatrix_ForPointLights(m_pipeline, pl, LightPos_viewMatrix, 1);




///2nd Render pass: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_pipeline.pushMatrix();
    shadow_SecondRender->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);
		glUniformMatrix4fv(glGetUniformLocation(shadow_SecondRender->getProgramId(),"lightModelViewProjectionMatrix"),1,GL_FALSE,&shadowMatrix[0][0]);
		glUniform1i(glGetUniformLocation(shadow_SecondRender->getProgramId(),"shadowMap"),0);

        glUniformMatrix4fv(glGetUniformLocation(shadow_SecondRender->getProgramId(),"LightPosition_ModelViewMatrix"),1,GL_FALSE,&LightPos_modelViewMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shadow_SecondRender->getProgramId(),"LightPosition"),lightPosition.x,lightPosition.y,lightPosition.z);
		glUniform3f(glGetUniformLocation(shadow_SecondRender->getProgramId(),"cameraPosition"),cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);


        m_pipeline.updateMatrices(shadow_SecondRender->getProgramId());
        ground->draw(shadow_SecondRender->getProgramId());

        l_CubeEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), sphere);
    shadow_SecondRender->delShader();
    m_pipeline.popMatrix();

    /// remember to turn off shader when draw using fixed pipeline functionality
    DrawAxis( 20.0f, m_pipeline);









/*
    m_pipeline.pushMatrix();
    ObjShader->useShader();

		glUniform3f(glGetUniformLocation(ObjShader->getProgramId(),"cameraPosition"),cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);

        m_pipeline.updateMatrices(ObjShader->getProgramId());
        m_LightingEffect->SetDirectionalLight(m_directionalLight);
        m_LightingEffect->SetMatSpecularIntensity(Specular_Intensity);
        m_LightingEffect->SetMatSpecularPower(Specular_Power);

        m_LightingEffect->SetPointLights(2, pl);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        ground1->draw(ObjShader->getProgramId());

    ObjShader->delShader();
    glBindTexture(GL_TEXTURE_2D,0);
    m_pipeline.popMatrix();
*/
#if 0
    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	m_pipeline.loadIdentity();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	quadRenderShader->useShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,renderTexture);
	glUniform1i(glGetUniformLocation(quadRenderShader->getProgramId(),"texture"),0);
	glUniform2f(glGetUniformLocation(quadRenderShader->getProgramId(),"pixelSize"),1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT);
	m_pipeline.updateMatrices(quadRenderShader->getProgramId());
	quad->draw(quadRenderShader->getProgramId());
	quadRenderShader->delShader();

    glEnable(GL_DEPTH_TEST)
#endif

}













ExplosionGenerator::~ExplosionGenerator()
{

}




void ExplosionGenerator::update()
{
 //   MotionGL();

    static ElapsedTime elapsedTime;
    float fDeltaTime = elapsedTime.GetElapsedTime();
    angle+=0.05f;

    pl[0].Position = glm::vec3(3.0f, 1.0f, -field_length/2 + field_length * (cosf(angle) + 1.0f) / 2.0f);
    pl[1].Position = glm::vec3(-3.0f, 1.0f, -field_length/2 + field_length * (sinf(angle) + 1.0f) / 2.0f);

 //   sl[0].Position = glm::vec3(0.0f, 1.0f, -field_length/2 + field_length * (sinf(angle) + 1.0f) / 2.0f);

    if (space_bar)
    {
        // e_CubeEffect.UpdateParticleCube(fDeltaTime);
        // e_CubeEffect.update();
        l_CubeEffect.update();

/*
        for(int i = 0; i < l_CubeEffect.e_ParticleBuffer.size(); i++)
        {
//            l_CubeEffect.e_ParticleBuffer[i].m_fAge += dt;
            myCD_Hgrid.rehash( &l_CubeEffect.e_ParticleBuffer[i] );
        }
*/

    }

 //   if (addSmoke)
  //      smoke.addDensitySource_FromUI(5,5,5);

   // smoke.update(addSmoke);
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



void ExplosionGenerator::MotionGL()
{

    int tmpx,tmpy;
    SDL_GetMouseState(&tmpx,&tmpy);

    g_MouseCurrent = glm::vec2( tmpx, tmpy );
    g_MouseDelta = ( g_MousePrevious - g_MouseCurrent );

    // Update the camera
    if ( g_bLeftMouseDown && g_bRightMouseDown )
    {
        g_Camera.TranslateZ( g_MouseDelta.y );
    }

	// this changes the camera around the origin, no shifting at all
    else if ( g_bLeftMouseDown )
    {
		// if you have a picture of a burning fire
		// addYaw will make it into a spinning burning fire
		// if you add AddRoll, it will spin your camera around
//		g_Camera.AddRoll(-g_MouseDelta.x);
        g_Camera.AddPitch( -g_MouseDelta.y );
        g_Camera.AddYaw( -g_MouseDelta.x );
    }
    else if ( g_bRightMouseDown )
    {
        g_Camera.TranslatePivotX( g_MouseDelta.x );
        g_Camera.TranslatePivotY( -g_MouseDelta.y );
    }

    g_MousePrevious = g_MouseCurrent;

}


void ExplosionGenerator::setupCamera()
{
    cam = t_camera(lightPosition, lightDirection.x, lightDirection.y);

    g_Camera.SetTranslate( g_DefaultCameraTranslate );
    g_Camera.SetRotate( g_DefaultCameraRotate );
    g_Camera.SetPivot( g_DefaultCameraPivot );

}



void ExplosionGenerator::setupColor_Texture()
{
    glShadeModel( GL_SMOOTH );

    glEnable(GL_TEXTURE_2D);
    textureID = loadTexture("red clay.jpg");
    if(textureID == false)
    {
        cout << "textureID is NULL" << glGetError << endl;
        exit(1);
    }

}

unsigned int ExplosionGenerator::loadTexture(string filename, int background, bool generate)
{

    std::ifstream in(filename.c_str());

    if(!in.is_open())
    {
        std::cout << "Nor oepened1" << std::endl;
        return -1;
    }

    string path = filename.substr(0,(filename.find_last_of('/') != string::npos ?
            filename.find_last_of('/')+1 : 0));

    unsigned int num;
    glGenTextures(1,&num);
    SDL_Surface* img = IMG_Load(filename.c_str());
    if(img == NULL)
    {
        cout << "Loadtexture img failed" << endl;
        return -1;
    }

    SDL_PixelFormat pixel_format = {NULL,
                                    32,             // converting to 32 bit pixel
                                    4,              // number of bytes
                                    0,0,0,0,        // byte lost
                                    0,0,0,0,        // byte shift
                                    0xff000000,     // red mask
                                    0x00ff0000,     // green mask
                                    0x0000ff00,     // blue mask
                                    0x000000ff,     // alpha mask
                                    0,              // color code
                                    255};           // alpha code

    // SDL_SWSURFACE means it's a software surface so we don't store it
    // in the video card
    // we're converting im2 to into this new SDL_PixelFormat format
    SDL_Surface* img2 = SDL_ConvertSurface(img, &pixel_format ,SDL_SWSURFACE);
    if(img2 == NULL)
    {
        cout << "img2 was not loaded" << std :: endl;
        return -1;
    }


    // tell OpenGL we want to use this texture
    glBindTexture(GL_TEXTURE_2D,num);       //and use the texture, we have just generated

    //    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,img->w,img->h,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);
    // no else
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);        //we make the actual texture
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_BGR,GL_UNSIGNED_BYTE,img->pixels);        //we make the actual texture


    if(generate)
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); //if the texture is smaller, than the image, we get the avarege of the pixels next to it
    else
        // no need to Magnify the MAG_FILTER
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    // if you comment these two lines out, you will see the edges of the cube
    if(background)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);      //we do it for vertically and horizontally (previous line)
    }

    // openGL 3.0
    if(generate)
        glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(img);   //we delete the image, we don't need it anymore
    SDL_FreeSurface(img2);
    return num;
}




void ExplosionGenerator::DrawAxis(float fScale, glm::vec3 translate)
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




void ExplosionGenerator::DrawAxis(float fScale, pipeline& m_pipeline, glm::vec3 translate)
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
//    float difamb[] ={1.0, 0.5, 0.3, 1.0};
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








void ExplosionGenerator::show1()
{
//rendering to texture...
    glClearColor(0,0,0.5,1);

    m_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT,1.0, 1000.0);
    glEnable(GL_DEPTH_TEST);
	m_pipeline.matrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();
    cam.Control();
    cam.UpdateCamera();
    cam.UpdateCamera(m_pipeline);

	m_pipeline.matrixMode(MODEL_MATRIX);
	glUniform3f(glGetUniformLocation(ObjShader->getProgramId(),"lightPos"),0,1,2);
	glUniform3f(glGetUniformLocation(ObjShader->getProgramId(),"cameraPosition"),cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);
	ObjShader->useShader();
	m_pipeline.updateMatrices(ObjShader->getProgramId());
//	glBindFramebuffer(GL_FRAMEBUFFER,FBO);  // this will render to a texture if we bind FBO
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		scene->draw(ObjShader->getProgramId());
//	glBindFramebuffer(GL_FRAMEBUFFER,0);
	ObjShader->delShader();




#if 0
    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	m_pipeline.loadIdentity();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	quadRenderShader->useShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,renderTexture);
	glUniform1i(glGetUniformLocation(quadRenderShader->getProgramId(),"texture"),0);
	glUniform2f(glGetUniformLocation(quadRenderShader->getProgramId(),"pixelSize"),1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT);
	m_pipeline.updateMatrices(quadRenderShader->getProgramId());
	quad->draw(quadRenderShader->getProgramId());
	quadRenderShader->delShader();

    glEnable(GL_DEPTH_TEST)
#endif





#if 0
    // it's stored in your own comptuer not in the video card
    glClearColor(0.0,0.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


/// set to correct mode
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadIdentity();


///Camera controls
    // MotionGL();
    // g_Camera.ApplyViewTransform();
    // g_Camera.ApplyViewTransform(m_pipeline);
    cam.Control();
    cam.UpdateCamera();
    cam.UpdateCamera(m_pipeline);


/*
    m_pipeline.rotateX(lightDirection.x);
    m_pipeline.rotateY(lightDirection.y);
    m_pipeline.translate(lightPosition.x,lightPosition.y, lightPosition.z);


    glRotatef(-lightDirection.x, 1.0, 0.0, 0.0);
    glRotatef(-lightDirection.y, 0.0, 1.0, 0.0);
    glTranslatef(-lightPosition.x, -lightPosition.y, -lightPosition.z);
*/


///First render pass: light's point of view





  //  getDepthTexture_FromLightPosion();





    m_pipeline.matrixMode(MODEL_MATRIX);
	glEnable(GL_DEPTH_TEST);




    //DrawAxis( 20.0f);//, g_Camera.GetPivot());

/*
  //  glDisable(GL_CULL_FACE);
    GLUquadricObj* quad;
	quad = gluNewQuadric();
    MainShader->useShader();

    glPushMatrix();
    m_pipeline.pushMatrix();
        m_pipeline.updateMatrices(MainShader->getProgramId());
        gluSphere(quad,0.5,20,20);
    glPopMatrix();
    m_pipeline.popMatrix();
    MainShader->delShader();

 //   glEnable(GL_CULL_FACE);
    m_pipeline.pushMatrix();
   // drawGround(200, textureID);
*/







#if 0
    MainShader->useShader();
    m_pipeline.pushMatrix();

    // updating variables
        m_pipeline.updateMatrices(MainShader->getProgramId());

        glUniform3f(glGetUniformLocation(MainShader->getProgramId(),"lightPos"),0.0,0.0,0.0);	//light position (is the same as the player position)

        glUniform3f(glGetUniformLocation(MainShader->getProgramId(),"mambient"),0.2,0.2,0.2);	//setting the material property
        glUniform3f(glGetUniformLocation(MainShader->getProgramId(),"mdiffuse"),0.6,0.6,0.6);
        glUniform3f(glGetUniformLocation(MainShader->getProgramId(),"mspecular"),1.0,1.0,1.0);

        glUniform3f(glGetUniformLocation(MainShader->getProgramId(),"lambient"),0.2,0.2,0.2);	//setting light property
        glUniform3f(glGetUniformLocation(MainShader->getProgramId(),"ldiffuse"),0.6,0.6,0.6);
        glUniform3f(glGetUniformLocation(MainShader->getProgramId(),"lspecular"),1.0,1.0,1.0);

        glUniform1f(glGetUniformLocation(MainShader->getProgramId(),"shininess"),32.0);	//shininess


    // drawing it
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_QUADS, 0, 4);


    m_pipeline.popMatrix();
    MainShader->delShader();






    ObjShader->useShader();
     //   glUniform3f(glGetUniformLocation(ObjShader->getProgramId(),"lightPos"),0,1,2);
    //	glUniform3f(glGetUniformLocation(ObjShader->getProgramId(),"cameraPosition"),2,3,2);
        m_pipeline.updateMatrices(ObjShader->getProgramId());
        scene->draw(ObjShader->getProgramId());
    ObjShader->delShader();


  //  l_CubeEffect.show();
    int r = 20;

    DrawAxis( 20.0f, m_pipeline);//, g_Camera.GetPivot());
#endif



 //   drawCubeFrame(r, r/2);
//    myCD_Hgrid.Draw();

#endif




}

