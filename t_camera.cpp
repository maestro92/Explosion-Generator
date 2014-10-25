#include "t_camera.h"

void t_camera::lockCamera()
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

void t_camera::moveCamera(float dir)
{
	float rad=(camYaw+dir)*M_PI/180.0;
	loc.x-=sin(rad)*movevel;
	loc.z-=cos(rad)*movevel;
}

void t_camera::moveCameraUp(float dir)
{
	float rad=(camPitch+dir)*M_PI/180.0;
	loc.y+=sin(rad)*movevel;
}

t_camera::t_camera()
{
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;
}
t_camera::t_camera(vector3d l)
{
	loc.change(l);
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;
}

t_camera::t_camera(vector3d l,float yaw,float pitch)
{
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;
}

t_camera::t_camera(vector3d l,float yaw,float pitch,float mv,float mov)
{
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	movevel=mv;
	mousevel=mov;
	mi=false;
}

void t_camera::Control()
{
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
			ismoved=true;
			if(camPitch!=90 && camPitch!=-90)
				moveCamera(0.0);
			moveCameraUp(0.0);
		}else if(state[SDLK_s])
		{
			ismoved=true;
			if(camPitch!=90 && camPitch!=-90)
				moveCamera(180.0);
			moveCameraUp(180.0);
		}
		if(state[SDLK_a])
		{
			ismoved=true;
			moveCamera(90.0);
		}
		else if(state[SDLK_d])
		{
			ismoved=true;
			moveCamera(270);
		}
	}

}


void t_camera::Control(pipeline& m_pipeline)
{
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
			ismoved=true;
			if(camPitch!=90 && camPitch!=-90)
				moveCamera(0.0);
			moveCameraUp(0.0);
		}else if(state[SDLK_s])
		{
			ismoved=true;
			if(camPitch!=90 && camPitch!=-90)
				moveCamera(180.0);
			moveCameraUp(180.0);
		}
		if(state[SDLK_a])
		{
			ismoved=true;
			moveCamera(90.0);
		}
		else if(state[SDLK_d])
		{
			ismoved=true;
			moveCamera(270);
		}
	}
//	m_pipeline.rotateX(camPitch);
//	m_pipeline.rotateY(camYaw);
}




void t_camera::UpdateCamera()
{
    // the camera can't move, so move everything else
    glRotatef(-camPitch, 1.0, 0.0, 0.0);
    glRotatef(-camYaw, 0.0, 1.0, 0.0);
    glTranslatef(-loc.x, -loc.y, -loc.z);
}

void t_camera::UpdateCamera(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.rotateX(camPitch);
    m_pipeline.rotateY(camYaw);
    m_pipeline.translate(loc.x, loc.y, loc.z);
}






//change the spherical coordinate system to cartesian
vector3d t_camera::getVector()
{
	//Google->spherical to cartesian
 	return (vector3d(-cos(camPitch*M_PI/180.0)*sin(camYaw*M_PI/180.0),sin(camPitch*M_PI/180.0),-cos(camPitch*M_PI/180.0)*cos(camYaw*M_PI/180.0)));
}
vector3d t_camera::getLocation()
{
	return loc;
}

float t_camera::getPitch()
{
	return camPitch;
}

float t_camera::getYaw()
{
	return camYaw;
}
float t_camera::getMovevel()
{
	return movevel;
}
float t_camera::getMousevel()
{
	return mousevel;
}

bool t_camera::isMouseIn()
{
	return mi;
}

void t_camera::setLocation(vector3d vec)
{
	loc.change(vec);
}

void t_camera::lookAt(float pitch,float yaw)
{
	camPitch=pitch;
	camYaw=yaw;
}

void t_camera::mouseIn(bool b)
{
	mi=b;
}

void t_camera::setSpeed(float mv,float mov)
{
	movevel=mv;
	mousevel=mov;
}

bool t_camera::isMoved()
{
	return ismoved;
}
