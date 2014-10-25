


#include "main.h"

#include "gl/gl.h"
#include "gl/glu.h"

#include "PivotCamera.h"



#include "SDL\SDL.h"
using namespace std;




//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
// const int SCREEN_WIDTH = 600;
// const int SCREEN_HEIGHT = 480;

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


glm::vec3 g_DefaultCameraTranslate( 0, 0, 400 );
// so positive y axis faces downwards
glm::vec3 g_DefaultCameraRotate( 0, 0, 0 );
glm::vec3 g_DefaultCameraPivot( 0, 0, 0 );


bool space_bar = false;



ExplosionGenerator::ExplosionGenerator()
{
    running = true;
    dvel = false;
    addSmoke = false;

    g_bLeftMouseDown = false;
    g_bRightMouseDown = false;


    initSDL_GLEW();
    initOpenGL();


    setupCamera();
    setupColor_Texture();
    setupParticleEmitter();

 //   e_CubeEffect.InitParticleCube();
    l_CubeEffect.InitParticleCube();


    SDL_WM_SetCaption("Template", NULL);

    init_Lighting();

    setup_CollisionDetection_HGrid();



    // l_CubeEffect.ExamineParticleAttribute();

}



void ExplosionGenerator::initSDL_GLEW()
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


void ExplosionGenerator::initOpenGL()
{

    //Initialize clear color
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  //  glClearDepth(1.0);

    //Initialize Projection Mode
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

    glEnable(GL_DEPTH_TEST);
}




void ExplosionGenerator::setupParticleEmitter()
{

}


void ExplosionGenerator::setup_CollisionDetection_HGrid()
{


}


void ExplosionGenerator::start()
{
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
            show();


            SDL_GL_SwapBuffers();

          //  next_game_tick += INTERVAL;
         //   delay_time = next_game_tick - SDL_GetTicks();
            if (next_game_tick > SDL_GetTicks())
                SDL_Delay(next_game_tick - SDL_GetTicks());
            next_game_tick = SDL_GetTicks() + INTERVAL;

    }

}





void ExplosionGenerator::init_Lighting()
{
    GLfloat light_ambient[] = {  (0.2),  (0.2),  (0.2),  (1.0) };
	GLfloat light_diffuse[] = {  (1.0),  (1.0),  (1.0),  (1.0) };
	GLfloat light_specular[] = {  (1.0),  (1.0),  (1.0),  (1.0 )};
	/*	light_position is NOT default value	*/
	GLfloat light_position0[] = {  (1.0),  (10.0),  (1.0),  (0.0 )};
	GLfloat light_position1[] = {  (-1.0),  (-10.0),  (-1.0),  (0.0) };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);


	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}




void ExplosionGenerator::show()
{
    // it's stored in your own comptuer not in the video card
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

    // update your camera
    g_Camera.ApplyViewTransform();
    drawGround(200, textureID);
  //  smoke.show(dvel);
    DrawAxis( 20.0f);//, g_Camera.GetPivot());


    l_CubeEffect.show();
    //e_CubeEffect.DrawParticleCube();
   // e_CubeEffect.show();

    int r = 20;

    drawCubeFrame(r, r/2);
 //   myCD_Hgrid.Draw();

}

ExplosionGenerator::~ExplosionGenerator()
{

}




void ExplosionGenerator::update()
{
    MotionGL();

    static ElapsedTime elapsedTime;
    float fDeltaTime = elapsedTime.GetElapsedTime();

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
/*
            // top face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, size/2+offset, size/2);
            glVertex3f(-size/2, size/2+offset, size/2);
            glVertex3f(-size/2, size/2+offset, -size/2);
            glVertex3f(size/2, size/2+offset, -size/2);

            // bottom face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, -size/2+offset, size/2);
            glVertex3f(-size/2, -size/2+offset, size/2);
            glVertex3f(-size/2, -size/2+offset, -size/2);
            glVertex3f(size/2, -size/2+offset, -size/2);
            */
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
}


