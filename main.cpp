
#include <cstdlib>
#include <iostream>
#include "main.h"


/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and string streams

#include <stdio.h>
#include <string>
#include <sstream>



// #include "gl/glew.h"
#include "gl/gl.h"
#include "gl/glu.h"

#include "PivotCamera.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
// const int SCREEN_WIDTH = 600;
// const int SCREEN_HEIGHT = 480;





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
    g_bLeftMouseDown = false;
    g_bRightMouseDown = false;


    initSDL_GLEW();
    initOpenGL();
    setupCamera();
    setupColor_Texture();
    setupParticleEmitter();

    e_CubeEmitter.InitParticleCube();

    SDL_WM_SetCaption("Template", NULL);

    init_Lighting();
}



void ExplosionGenerator::update()
{
    MotionGL();

    static ElapsedTime elapsedTime;
    float fDeltaTime = elapsedTime.GetElapsedTime();


    if (space_bar)
    {
        e_CubeEmitter.UpdateParticleCube(fDeltaTime);
    }

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

    DrawAxis( 20.0f);//, g_Camera.GetPivot());
  //  drawCube(20);
    e_CubeEmitter.DrawParticleCube();

}







void ExplosionGenerator::start()
{
    Uint32 startTime = SDL_GetTicks();
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
               //         case SDLK_SPACE:   space_bar = false;    break;
                    }
                    break;


                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:   running = false;    break;
                        case SDLK_z:
                            e_CubeEmitter.InitParticleCube();
                            space_bar = false;
                            break;
                        case SDLK_SPACE:
                            space_bar = true;

                            break;
                    }
                    break;
			}
        }
            update();
            show();


            SDL_GL_SwapBuffers();


            if(1000/30> (SDL_GetTicks()-startTime))
            {
                SDL_Delay(1000/30 - (SDL_GetTicks() - startTime));
            }
    }

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
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
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



void ExplosionGenerator::setupCamera()
{

    g_Camera.SetTranslate( g_DefaultCameraTranslate );
    g_Camera.SetRotate( g_DefaultCameraRotate );
    g_Camera.SetPivot( g_DefaultCameraPivot );

}

void ExplosionGenerator::setupColor_Texture()
{
    glShadeModel( GL_SMOOTH );

}


void ExplosionGenerator::setupParticleEmitter()
{

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




void display()
{

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);


        glBegin(GL_TRIANGLES);
                glVertex3f(0.0,2.0,-2.0);
                glVertex3f(-2.0,-2.0,-2.0);
                glVertex3f(2.0,-2.0,-2.0);
        glEnd();


}


