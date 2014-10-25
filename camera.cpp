#include "camera.h"
#include <algorithm>

#include <iostream>
using namespace std;


void Camera::Update( float fDeltaTime )
{

}

void Camera::SetViewport( int x, int y, int width, int height )
{
    m_ViewportX = x;
    m_ViewportY = y;
    m_ViewportWidth = width;
    m_ViewportHeight = std::max( height, 1 );
}


void Camera::SetProjection( float fovy, float aspect, float zNear, float zFar )
{
    m_fVFOV = fovy;
    m_fAspect = aspect;
    m_fNear = zNear;
    m_fFar = zFar;
}



// x axis
void Camera::AddPitch( float fPitch )
{
    m_Rotate.x += fPitch;
}



// y axis (same in OpenGL because it is OpenGL XD)
void Camera::AddYaw( float fYaw )
{
    m_Rotate.y += fYaw;
}



// z axis
void Camera::AddRoll( float fRoll )
{
    m_Rotate.z += fRoll;
}

/*
void Camera::AddRotation( const glm::quat& rot )
{
    glm::quat finalRotate = rot * glm::quat( m_Rotate );
    m_Rotate = glm::eularAngles( finalRotate );
}
*/



void Camera::TranslateX( float x )
{
    m_Translate.x += x;
}

void Camera::TranslateY( float y )
{
    m_Translate.y += y;
}

void Camera::TranslateZ( float z )
{
    m_Translate.z += z;
}



void Camera::SetTranslate( const glm::vec3& translate )
{
    m_Translate = translate;
}

void Camera::SetRotate( float pitch, float yaw, float roll )
{
    m_Rotate = glm::vec3( pitch, yaw, roll );
}


void Camera::SetRotate( const glm::vec3& rotate )
{
    m_Rotate = rotate;
}


glm::vec3 Camera::GetTranslation() const
{
    return m_Translate;
}

glm::vec3 Camera::GetRotation() const
{
    return m_Rotate;
}



void Camera::ApplyViewport()
{
    glViewport( m_ViewportX, m_ViewportY, m_ViewportWidth, m_ViewportHeight );
}



void Camera::ApplyProjectionTransform()
{
    glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    gluPerspective( m_fVFOV, m_fAspect, m_fNear, m_fFar );
}


void Camera::ApplyViewTransform()
{
    glMatrixMode( GL_MODELVIEW );
    glTranslatef( m_Translate.x, m_Translate.y, m_Translate.z );
    glRotatef( m_Rotate.x, 1.0f, 0.0f, 0.0f );
    glRotatef( m_Rotate.y, 0.0f, 1.0f, 0.0f );
    glRotatef( m_Rotate.z, 0.0f, 0.0f, 1.0f );
}


/*
void camera::lockCamera()
{
	if(camPitch>90)
		camPitch=90;
	if(camPitch<-90)
		camPitch=-90;
	if(camYaw<0.0)
		camYaw+=360.0;
	if(camYaw>360.0)
		camYaw-=360;
}

void camera::moveCamera(float dir)
{
	float rad=(camYaw+dir)*M_PI/180.0;
	loc.x-=sin(rad)*movevel;
	loc.z-=cos(rad)*movevel;
}

void camera::moveCameraUp(float dir)
{
	float rad=(camPitch+dir)*M_PI/180.0;
	loc.y+=sin(rad)*movevel;
}

camera::camera()
{
	camPitch=0;
	camYaw=0;
	camRoll=0;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;
}
camera::camera(vector3d l)
{
	loc.change(l);
	camPitch=0;
	camYaw=0;
	camRoll=0;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;
}

camera::camera(vector3d l,float yaw,float pitch)
{
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	camRoll=0;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;
}

camera::camera(vector3d l,float yaw,float pitch,float mv,float mov)
{
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	camRoll=0;
	movevel=mv;
	mousevel=mov;
	mi=false;
}

void camera::Control()
{
 //   cout << "in camera control" << endl;
	if(mi)
	{
		int MidX=320;
		int MidY=240;
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		camYaw+=mousevel*(MidX-tmpx);
		camPitch+=mousevel*(MidY-tmpy);
		lockCamera();
		SDL_WarpMouse(MidX,MidY);
		Uint8* state=SDL_GetKeyState(NULL);
		ismoved=false;
		if(state[SDLK_w])
		{
   //         cout << "pressing w" << endl;
			ismoved=true;
			if(camPitch!=90 && camPitch!=-90)
				moveCamera(0.0);
			moveCameraUp(0.0);
		}else if(state[SDLK_s])
		{
     //       cout << "pressing s" << endl;
			ismoved=true;
			if(camPitch!=90 && camPitch!=-90)
				moveCamera(180.0);
			moveCameraUp(180.0);
		}
		if(state[SDLK_a])
		{
       //     cout << "pressing a" << endl;
			ismoved=true;
			moveCamera(90.0);
		}
		else if(state[SDLK_d])
		{
		 //   cout << "pressing d" << endl;
			ismoved=true;
			moveCamera(270);
		}
	}
	glRotatef(-camPitch,1.0,0.0,0.0);
	glRotatef(-camYaw,0.0,1.0,0.0);
}

void camera::UpdateCamera()
{

	glTranslatef(-loc.x,-loc.y,-loc.z);
}

//change the spherical coordinate system to cartesian
vector3d camera::getVector()
{
	//Google->spherical to cartesian
 	return (vector3d(-cos(camPitch*M_PI/180.0)*sin(camYaw*M_PI/180.0),sin(camPitch*M_PI/180.0),-cos(camPitch*M_PI/180.0)*cos(camYaw*M_PI/180.0)));
}
vector3d camera::getLocation()
{
	return loc;
}

float camera::getPitch()
{
	return camPitch;
}

float camera::getYaw()
{
	return camYaw;
}

float camera::getRoll()
{
	return camRoll;
}


float camera::getMovevel()
{
	return movevel;
}
float camera::getMousevel()
{
	return mousevel;
}

bool camera::isMouseIn()
{
	return mi;
}

void camera::setLocation(vector3d vec)
{
	loc.change(vec);
}

void camera::lookAt(float pitch,float yaw)
{
	camPitch=pitch;
	camYaw=yaw;
}

void camera::mouseIn(bool b)
{
	mi=b;
}

void camera::setSpeed(float mv,float mov)
{
	movevel=mv;
	mousevel=mov;
}

bool camera::isMoved()
{
	return ismoved;
}
*/

