
#include "EG_FirstPersonPovCamera.h"

EG_FirstPersonPovCamera::EG_FirstPersonPovCamera()
{
    rotation_quaternion = glm::quat(1, 0, 0, 0);
	m_pitchDegrees=0;
	m_yawDegrees=0;
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
EG_FirstPersonPovCamera::EG_FirstPersonPovCamera(glm::vec3 eye)
{
	m_eye = eye;
	rotation_quaternion = glm::quat(1, 0, 0, 0);
	m_pitchDegrees=0;
	m_yawDegrees=0;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;
}

EG_FirstPersonPovCamera::EG_FirstPersonPovCamera(glm::vec3 eye,float yaw,float pitch)
{
	m_eye = eye;
	rotation_quaternion = glm::quat(1, 0, 0, 0);
	m_pitchDegrees=pitch;
	m_yawDegrees=yaw;
	movevel=0.2;
	mousevel=0.2;
	mi=ismoved=false;
}

EG_FirstPersonPovCamera::EG_FirstPersonPovCamera(glm::vec3 eye,float yaw,float pitch,float mv,float mov)
{
	m_eye = eye;
	rotation_quaternion = glm::quat(1, 0, 0, 0);
	m_pitchDegrees=pitch;
	m_yawDegrees=yaw;
	movevel=mv;
	mousevel=mov;
	mi=false;
}



void EG_FirstPersonPovCamera::lockCamera()
{
	if(m_pitchDegrees>90)
		m_pitchDegrees=90;
	if(m_pitchDegrees<-90)
		m_pitchDegrees=-90;
	if(m_yawDegrees<0.0)
		m_yawDegrees+=360.0;
	if(m_yawDegrees>360.0)
		m_yawDegrees-=360;
}

void EG_FirstPersonPovCamera::moveCamera(float dir)
{
	float rad=(m_yawDegrees+dir)*M_PI/180.0;
	m_eye.x-=sin(rad)*movevel;
	m_eye.z-=cos(rad)*movevel;
}

void EG_FirstPersonPovCamera::moveCameraUp(float dir)
{
	float rad=(m_pitchDegrees+dir)*M_PI/180.0;
	m_eye.y+=sin(rad)*movevel;
}


void EG_FirstPersonPovCamera::Control(int mid_x, int mid_y)
{
	if(mi)
	{
		int MidX=mid_x;
		int MidY=mid_y;
		SDL_ShowCursor(SDL_DISABLE);

		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		m_yawDegrees+=mousevel*(MidX-tmpx);
		m_pitchDegrees+=mousevel*(MidY-tmpy);
		lockCamera();
		SDL_WarpMouse(MidX,MidY);
		Uint8* state=SDL_GetKeyState(NULL);
		ismoved=false;
		if(state[SDLK_w])
		{
			ismoved=true;
			if(m_pitchDegrees!=90 && m_pitchDegrees!=-90)
				moveCamera(0.0);
			moveCameraUp(0.0);
		}else if(state[SDLK_s])
		{
			ismoved=true;
			if(m_pitchDegrees!=90 && m_pitchDegrees!=-90)
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





void EG_FirstPersonPovCamera::Control_MatrixBased(pipeline& m_pipeline, int mid_x, int mid_y)
{
	if(mi)
	{
		int MidX=mid_x;
		int MidY=mid_y;

		/// this is the same as rotateFirstPerson in GLCamera1
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		m_yawDegrees+=mousevel*(MidX-tmpx);
		m_pitchDegrees+=mousevel*(MidY-tmpy);
		SDL_WarpMouse(MidX,MidY);
	}
}


void EG_FirstPersonPovCamera::UpdateCamera_MatrixBased(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up)
{
    /// z axis is always defined by eye,target
    /// x axis is cross(z_axis, up)
    /// y axis is cross(x,z)

 //   m_eye = eye;
    m_eye.x = m_eye.x;
    m_eye.y = m_eye.y;
    m_eye.z = m_eye.z;

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
    glRotatef(-m_pitchDegrees, 1.0, 0.0, 0.0);
    glRotatef(-m_yawDegrees, 0.0, 1.0, 0.0);
    glTranslatef(-m_eye.x, -m_eye.y, -m_eye.z);
*/
}












void EG_FirstPersonPovCamera::Control(pipeline& m_pipeline, int mid_x, int mid_y)
{
	if(mi)
	{
		int MidX=mid_x;
		int MidY=mid_y;
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		m_yawDegrees+=mousevel*(MidX-tmpx);
		m_pitchDegrees+=mousevel*(MidY-tmpy);
		lockCamera();
		SDL_WarpMouse(MidX,MidY);
		Uint8* state=SDL_GetKeyState(NULL);
		ismoved=false;
		if(state[SDLK_w])
		{
			ismoved=true;
			if(m_pitchDegrees!=90 && m_pitchDegrees!=-90)
				moveCamera(0.0);
			moveCameraUp(0.0);
		}else if(state[SDLK_s])
		{
			ismoved=true;
			if(m_pitchDegrees!=90 && m_pitchDegrees!=-90)
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
//	m_pipeline.rotateX(m_pitchDegrees);
//	m_pipeline.rotateY(m_yawDegrees);
}






void EG_FirstPersonPovCamera::UpdateCamera()
{
    // the camera can't move, so move everything else
    glRotatef(-m_pitchDegrees, 1.0, 0.0, 0.0);
    glRotatef(-m_yawDegrees, 0.0, 1.0, 0.0);
    glTranslatef(-m_eye.x, -m_eye.y, -m_eye.z);
}


void EG_FirstPersonPovCamera::UpdateCamera_Translation(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.translate(m_eye.x, m_eye.y, m_eye.z);
}

void EG_FirstPersonPovCamera::UpdateCamera_Rotation(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.rotateX(m_pitchDegrees);
    m_pipeline.rotateY(m_yawDegrees);
}


void EG_FirstPersonPovCamera::UpdateCamera(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.rotateX(m_pitchDegrees);
    m_pipeline.rotateY(m_yawDegrees);
    m_pipeline.translate(m_eye.x, m_eye.y, m_eye.z);
}

glm::mat4 EG_FirstPersonPovCamera::UpdateCamera_NoRotate(pipeline& m_pipeline)
{
    return m_pipeline.translateNoRotate(m_eye.x, m_eye.y, m_eye.z);
}




//change the spherical coordinate system to cartesian
glm::vec3 EG_FirstPersonPovCamera::getVector()
{
	//Google->spherical to cartesian
 	return (glm::vec3(-cos(m_pitchDegrees*M_PI/180.0)*sin(m_yawDegrees*M_PI/180.0),sin(m_pitchDegrees*M_PI/180.0),-cos(m_pitchDegrees*M_PI/180.0)*cos(m_yawDegrees*M_PI/180.0)));
}
glm::vec3 EG_FirstPersonPovCamera::getLocation()
{
	return m_eye;
}

float EG_FirstPersonPovCamera::getPitch()
{
	return m_pitchDegrees;
}

float EG_FirstPersonPovCamera::getYaw()
{
	return m_yawDegrees;
}
float EG_FirstPersonPovCamera::getMovevel()
{
	return movevel;
}
float EG_FirstPersonPovCamera::getMousevel()
{
	return mousevel;
}

bool EG_FirstPersonPovCamera::isMouseIn()
{
	return mi;
}

void EG_FirstPersonPovCamera::setEyePoint(glm::vec3 eye)
{
	m_eye = eye;
}

void EG_FirstPersonPovCamera::setEyePoint(float x, float y, float z)
{
	m_eye.x = x; m_eye.y = y; m_eye.z = z;
}



glm::vec3 EG_FirstPersonPovCamera::getEyePoint()
{
	return m_eye;
}

void EG_FirstPersonPovCamera::lookAt(float pitch,float yaw)
{
	m_pitchDegrees=pitch;
	m_yawDegrees=yaw;
}

void EG_FirstPersonPovCamera::mouseIn(bool b)
{
	mi=b;
}

void EG_FirstPersonPovCamera::setSpeed(float mv,float mov)
{
	movevel=mv;
	mousevel=mov;
}

bool EG_FirstPersonPovCamera::isMoved()
{
	return ismoved;
}


void EG_FirstPersonPovCamera::setPitch(float pitch)
{
    m_pitchDegrees = pitch;
}

void EG_FirstPersonPovCamera::setYaw(float yaw)
{
    m_yawDegrees = yaw;
}
