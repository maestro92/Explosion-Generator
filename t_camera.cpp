#include "t_camera.h"



t_camera::t_camera()
{
    rotation_quaternion = glm::quat(1, 0, 0, 0);
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;

	m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    m_viewMatrix = glm::mat4(1.0);

}
t_camera::t_camera(vector3d l)
{
	loc.change(l);
	rotation_quaternion = glm::quat(1, 0, 0, 0);
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;

    m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    m_viewMatrix = glm::mat4(1.0);

}

t_camera::t_camera(vector3d l,float yaw,float pitch)
{
	loc.change(l);
	rotation_quaternion = glm::quat(1, 0, 0, 0);
	camPitch=pitch;
	camYaw=yaw;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;

    m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    m_viewMatrix = glm::mat4(1.0);
}

t_camera::t_camera(vector3d l,float yaw,float pitch,float mv,float mov)
{
	loc.change(l);
	rotation_quaternion = glm::quat(1, 0, 0, 0);
	camPitch=pitch;
	camYaw=yaw;
	movevel=mv;
	mousevel=mov;
	mi=false;


    m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    m_viewMatrix = glm::mat4(1.0);
}








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







void t_camera::Control(int mid_x, int mid_y)
{
	if(mi)
	{
		int MidX=mid_x;
		int MidY=mid_y;
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





void t_camera::Control_MatrixBased(pipeline& m_pipeline, int mid_x, int mid_y)
{
	if(mi)
	{
		int MidX=mid_x;
		int MidY=mid_y;

		/// this is the same as rotateFirstPerson in GLCamera1
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		camYaw+=mousevel*(MidX-tmpx);
		camPitch+=mousevel*(MidY-tmpy);
		SDL_WarpMouse(MidX,MidY);
	}
}


void t_camera::UpdateCamera_MatrixBased(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up)
{
    /// z axis is always defined by eye,target
    /// x axis is cross(z_axis, up)
    /// y axis is cross(x,z)

 //   m_eye = eye;
    m_eye.x = loc.x;
    m_eye.y = loc.y;
    m_eye.z = loc.z;

    m_zAxis = eye - target;     /// target-eye is -z axis
    m_zAxis = glm::normalize(m_zAxis);

    m_viewDir = -m_zAxis;

    m_xAxis = glm::cross(up, m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);
    m_xAxis = glm::normalize(m_yAxis);

    /// These are all unit vectors
    m_viewMatrix[0][0] = m_xAxis.x;
    m_viewMatrix[1][0] = m_xAxis.y;
    m_viewMatrix[2][0] = m_xAxis.z;
    m_viewMatrix[3][0] = -glm::dot(m_xAxis, eye);

    m_viewMatrix[0][1] = m_yAxis.x;
    m_viewMatrix[1][1] = m_yAxis.y;
    m_viewMatrix[2][1] = m_yAxis.z;
    m_viewMatrix[3][1] = -glm::dot(m_yAxis, eye);

    m_viewMatrix[0][2] = m_zAxis.x;
    m_viewMatrix[1][2] = m_zAxis.y;
    m_viewMatrix[2][2] = m_zAxis.z;
    m_viewMatrix[3][2] = -glm::dot(m_zAxis, eye);

    /*


    /// the camera can't move, so move everything else
    glRotatef(-camPitch, 1.0, 0.0, 0.0);
    glRotatef(-camYaw, 0.0, 1.0, 0.0);
    glTranslatef(-loc.x, -loc.y, -loc.z);
*/
}












void t_camera::Control(pipeline& m_pipeline, int mid_x, int mid_y)
{
	if(mi)
	{
		int MidX=mid_x;
		int MidY=mid_y;
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


void t_camera::UpdateCamera_Translation(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.translate(loc.x, loc.y, loc.z);


/*
    glm::vec3 EulerAngles(-camPitch, -camYaw,0);
    rotation_quaternion = glm::quat(EulerAngles);
    m_pipeline.Rotate(rotation_quaternion);
*/
}

void t_camera::UpdateCamera_Rotation(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.rotateX(camPitch);
    m_pipeline.rotateY(camYaw);
 //   m_pipeline.rotateX(camPitch);
 //   m_pipeline.rotateY(camYaw);
/*
    glm::vec3 EulerAngles(-camPitch, -camYaw,0);
    rotation_quaternion = glm::quat(EulerAngles);
    m_pipeline.Rotate(rotation_quaternion);
*/
}


void t_camera::UpdateCamera(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
//      m_pipeline.translate(loc.x, loc.y, loc.z);
    m_pipeline.rotateX(camPitch);
    m_pipeline.rotateY(camYaw);
    m_pipeline.translate(loc.x, loc.y, loc.z);
}

glm::mat4 t_camera::UpdateCamera_NoRotate(pipeline& m_pipeline)
{
    return m_pipeline.translateNoRotate(loc.x, loc.y, loc.z);
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
