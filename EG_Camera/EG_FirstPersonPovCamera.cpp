
#include "EG_FirstPersonPovCamera.h"

const float CAMERA_ROTATION_SPEED = 0.2;
const float CAMERA_FORWARD_SPEED = 0.2;

EG_FirstPersonPovCamera::EG_FirstPersonPovCamera() : EG_FirstPersonPovCamera(glm::vec3(0.0,0.0,0.0), 0, 0)
{
}

EG_FirstPersonPovCamera::EG_FirstPersonPovCamera(glm::vec3 eye) : EG_FirstPersonPovCamera(eye, 0, 0)
{
}

EG_FirstPersonPovCamera::EG_FirstPersonPovCamera(glm::vec3 eye, float yaw, float pitch)
{
	m_pitchDegrees = 0;
	m_yawDegrees = 0;
	m_mouseIn=ismoved=false;

	m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    m_viewMatrix = glm::mat4(1.0);
}

/*
void EG_FirstPersonPovCamera::lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up)
{
    m_eye = eye;

    m_zAxis = eye - target;
    m_zAxis = glm::normalize(m_zAxis);

    m_viewDir = -m_zAxis;

    m_xAxis = glm::cross(up, m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_viewMatrix[0][0] = m_xAxis.x;     m_viewMatrix[1][0] = m_yAxis.x;     m_viewMatrix[2][0] = m_zAxis.x;     m_viewMatrix[3][0] = eye.x;
    m_viewMatrix[0][1] = m_xAxis.y;     m_viewMatrix[1][1] = m_yAxis.y;     m_viewMatrix[2][1] = m_zAxis.y;     m_viewMatrix[3][1] = eye.y;
    m_viewMatrix[0][2] = m_xAxis.z;     m_viewMatrix[1][2] = m_yAxis.z;     m_viewMatrix[2][2] = m_zAxis.z;     m_viewMatrix[3][2] = eye.z;

    m_viewMatrix = glm::inverse(m_viewMatrix);
	updateViewMatrix(m_pipeline);
}



void EG_FirstPersonPovCamera::move(float dx, float dy, float dz)
{
    glm::vec3 eye = m_eye;
    glm::vec3 forward;

    forward = glm::cross(WORLD_YAXIS, m_xAxis);
    forward = glm::normalize(forward);

    eye += EG_Utility::scaleGlmVec3(m_xAxis, dx);
    eye += EG_Utility::scaleGlmVec3(WORLD_YAXIS, dy);
    eye += EG_Utility::scaleGlmVec3(forward, dz);

    m_eye = eye;

}


void EG_FirstPersonPovCamera::move(const glm::vec3& direction, const glm::vec3& amount)
{
    m_eye.x += amount.x * direction.x;
    m_eye.y += amount.y * direction.y;
    m_eye.z += amount.z * direction.z;
}

void EG_FirstPersonPovCamera::updateViewMatrix(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadMatrix(m_viewMatrix);
}
*/




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
	m_eye.x-=sin(rad)*CAMERA_FORWARD_SPEED;
	m_eye.z-=cos(rad)*CAMERA_FORWARD_SPEED;
}

void EG_FirstPersonPovCamera::moveCameraUp(float dir)
{
	float rad=(m_pitchDegrees+dir)*M_PI/180.0;
	m_eye.y+=sin(rad)*CAMERA_FORWARD_SPEED;
}


void EG_FirstPersonPovCamera::Control(int mid_x, int mid_y)
{
	if(m_mouseIn)
	{
		int MidX=mid_x;
		int MidY=mid_y;
		SDL_ShowCursor(SDL_DISABLE);

		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);

		m_yawDegrees += CAMERA_ROTATION_SPEED*(MidX-tmpx);
		m_pitchDegrees += CAMERA_ROTATION_SPEED*(MidY-tmpy);


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




/*
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




    /// the camera can't move, so move everything else
 //   glRotatef(-m_pitchDegrees, 1.0, 0.0, 0.0);
 //   glRotatef(-m_yawDegrees, 0.0, 1.0, 0.0);
//    glTranslatef(-m_eye.x, -m_eye.y, -m_eye.z);

}
*/











void EG_FirstPersonPovCamera::UpdateCamera()
{
    // the camera can't move, so move everything else
    glRotatef(-m_pitchDegrees, 1.0, 0.0, 0.0);
    glRotatef(-m_yawDegrees, 0.0, 1.0, 0.0);
    glTranslatef(-m_eye.x, -m_eye.y, -m_eye.z);
}


void EG_FirstPersonPovCamera::UpdateCameraTranslation(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.translate(m_eye.x, m_eye.y, m_eye.z);
}

void EG_FirstPersonPovCamera::UpdateCameraRotation(pipeline& m_pipeline)
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

/*
glm::mat4 EG_FirstPersonPovCamera::UpdateCamera_NoRotate(pipeline& m_pipeline)
{
    return m_pipeline.translateNoRotate(m_eye.x, m_eye.y, m_eye.z);
}
*/

/*
//change the spherical coordinate system to cartesian
glm::vec3 EG_FirstPersonPovCamera::getVector()
{
	//Google->spherical to cartesian
 	return (glm::vec3(-cos(m_pitchDegrees*M_PI/180.0)*sin(m_yawDegrees*M_PI/180.0),sin(m_pitchDegrees*M_PI/180.0),-cos(m_pitchDegrees*M_PI/180.0)*cos(m_yawDegrees*M_PI/180.0)));
}
*/

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
	return CAMERA_FORWARD_SPEED;
}

float EG_FirstPersonPovCamera::getMousevel()
{
	return CAMERA_ROTATION_SPEED;
}

bool EG_FirstPersonPovCamera::isMouseIn()
{
	return m_mouseIn;
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

/*
void EG_FirstPersonPovCamera::mouseIn(bool b)
{
	m_mouseIn=b;
}
*/
void EG_FirstPersonPovCamera::setMouseInFlag(bool f)
{
	m_mouseIn = f;
}


/*
void EG_FirstPersonPovCamera::setSpeed(float mv,float mov)
{
	CAMERA_FORWARD_SPEED=mv;
	CAMERA_ROTATION_SPEED=mov;
}
*/
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
