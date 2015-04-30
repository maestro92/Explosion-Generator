
#include "EG_Camera.h"

static bool first = true;


const float CAMERA_ROTATION_SPEED = 0.2f;
const glm::vec3 CAMERA_FORWARD_SPEED = glm::vec3(2.0f, 2.0f, 2.0f);

EG_Camera::EG_Camera()
{
    m_cameraMode = FIRST_PERSON_POV_CAMERA_MODE;

   /*
    m_PitchDegrees = 0.0f;
    m_YawDegrees = 0.0f;

    m_mouseRotationSpeed = DEFAULT_MOUSE_ROTATION_SPEED;
    m_orbitMinZoom = DEFAULT_ORBIT_MIN_ZOOM;
    m_orbitMaxZoom = DEFAULT_ORBIT_MAX_ZOOM;
 //   m_orbitOffsetDistance = DEFAULT_ORBIT_OFFSET_DISTANCE;
    m_orbitOffsetDistance = 30.0f;

    mousevel = 1.0f;
*/
    mouse_in = false;
//	m_target = glm::vec3(0.0f, 0.0f, 0.0f);
  //  m_targetYAxis = glm::vec3(0.0f, 1.0f, 0.0f);

    m_mouseIn = false;

	m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    m_viewMatrix = glm::mat4(1.0);
    m_RotateViewMatrix = glm::mat4(1.0);

//    m_PitchDegrees = 0.0f;
    m_pitchDegrees = 0.0f;
    m_yawDegrees = 0.0f;

}



EG_Camera::~EG_Camera()
{


}

void EG_Camera::init()
{
    m_eye.x = 0.0f;
    m_eye.y = 2.0f;
    m_eye.z = 0.0f;

 //   m_orbitOffsetDistance = CAMERA_ZOOM_MIN + (CAMERA_ZOOM_MAX - CAMERA_ZOOM_MIN) * 0.3f;

    m_targetYAxis = m_yAxis;
    glm::vec3 newEye = m_eye + m_zAxis * m_orbitOffsetDistance;
    glm::vec3 newTarget = m_eye;

//    newEye = m_eye + m_zAxis * m_orbitOffsetDistance;
  //  newTarget = m_eye;

 //   lookAt(m_pipeline, newEye, newTarget, m_targetYAxis);

}

void EG_Camera::mouseIn(bool b)
{
    mouse_in = b;
}


void EG_Camera::lookAt(pipeline& m_pipeline, glm::vec3 target)
{
    lookAt(m_pipeline, m_eye, target, m_yAxis);
}


void EG_Camera::lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up)
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

/*
    m_viewMatrix[0][0] = m_xAxis.x;
    m_viewMatrix[0][1] = m_xAxis.y;
    m_viewMatrix[0][2] = m_xAxis.z;
    m_viewMatrix[3][0] = eye.x;

    m_viewMatrix[1][0] = m_yAxis.x;
    m_viewMatrix[1][1] = m_yAxis.y;
    m_viewMatrix[1][2] = m_yAxis.z;
    m_viewMatrix[3][1] = eye.y;

    m_viewMatrix[2][0] = m_zAxis.x;
    m_viewMatrix[2][1] = m_zAxis.y;
    m_viewMatrix[2][2] = m_zAxis.z;
    m_viewMatrix[3][2] = eye.z;
*/
    m_viewMatrix = glm::inverse(m_viewMatrix);
	updateViewMatrix(m_pipeline);
}



void EG_Camera::move(float dx, float dy, float dz)
{
    /*
    glm::vec3 eye = m_eye;
    glm::vec3 forward;

    forward = glm::cross(WORLD_YAXIS, m_xAxis);
    forward = glm::normalize(forward);

    eye += EG_Utility(dx, m_xAxis);
    eye += EG_Utility(dy, WORLD_YAXIS);
    eye += EG_Utility(dz, forward);

    m_eye = eye;
*/
}


void EG_Camera::move(glm::vec3& direction, glm::vec3 amount)
{
    m_eye.x += amount.x * direction.x;
    m_eye.y += amount.y * direction.y;
    m_eye.z += amount.z * direction.z;

//    updateViewMatrix();
}




void EG_Camera::RotateOrbit(pipeline& m_pipeline)
{
    glm::quat rot;
 //   rot = glm::toQuat(glm::mat4(1.0));
    ROT = glm::toQuat(glm::mat4(1.0));

    if(m_YawDegrees != 0.0f)
    {
    //    rot = glm::angleAxis(-m_YawDegrees,     m_yAxis);
        rot = glm::angleAxis(-m_YawDegrees,m_targetYAxis);
 //       rot = glm::angleAxis(-m_YawDegrees, glm::vec3(0.0f,1.0f,1.0f));

        ROT = rot * ROT;
        m_orientation = rot * m_orientation;
    }

    if(m_PitchDegrees != 0.0f)
    {
 //       rot = glm::angleAxis(-m_PitchDegrees, WORLD_XAXIS);
   //     rot = glm::rotate(-m_PitchDegrees, 1.0f,0.0f,0.0f);

        ROT = rot * ROT;
        m_orientation = rot * m_orientation;


    }

    updateViewMatrix(m_pipeline);

}




// https://www.opengl.org/archives/resources/faq/technical/viewing.htm
void EG_Camera::Control(pipeline& m_pipeline)
{
	if(mouse_in)
	{
		SDL_ShowCursor(SDL_DISABLE);

		Uint8* state=SDL_GetKeyState(NULL);

        if(state[SDLK_w])
		{
            m_PitchDegrees += mousevel;
		}
		else if(state[SDLK_s])
		{
            m_PitchDegrees -= mousevel;
		}


        if(state[SDLK_a])
		{
            m_YawDegrees += mousevel;
		}
		else if(state[SDLK_d])
		{
            m_YawDegrees -= mousevel;
		}

    m_pipeline.rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f);
    m_pipeline.rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f);

//        RotateOrbit(m_pipeline);
//	m_pipeline.rotateX(camPitch);
//	m_pipeline.rotateY(camYaw);

    }
}

// https://www.opengl.org/archives/resources/faq/technical/viewing.htm
void EG_Camera::Control(pipeline& m_pipeline, int mid_x, int mid_y)
{
    /*
    if(first)
    {
        lookAt(m_pipeline, newEye, newTarget, m_targetYAxis);
        first = false;
    }
*/
	if(mouse_in)
	{

		int MidX=mid_x;
		int MidY=mid_y;
		SDL_ShowCursor(SDL_DISABLE);

		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);

		/// rotate orbit
		m_YawDegrees += mousevel*(MidX-tmpx);
        m_PitchDegrees += mousevel*(MidY-tmpy);

 //       RotateOrbit(m_pipeline);

		SDL_WarpMouse(MidX,MidY);
 //       RotateOrbit(m_pipeline);

        m_pipeline.rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f);
        m_pipeline.rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f);

        m_skyboxRotate.rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f);
        m_skyboxRotate.rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f);
    //
   //     m_skyboxRotate = m_pipeline;
   //     m_skyboxRotate.translate(-m_eye.x, -m_eye.y, -m_eye.z);
//        m_RotateViewMatrix = glm::translate(-m_eye.x, -m_eye.y, -m_eye.z) * glm::rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f) * glm::rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f);
//        m_RotateViewMatrix = glm::rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f) * glm::rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f) * m_viewMatrix;

   //     m_RotateViewMatrix = m_viewMatrix;


/*
		SDL_ShowCursor(SDL_DISABLE);

		Uint8* state=SDL_GetKeyState(NULL);

        if(state[SDLK_w])
		{
            m_PitchDegrees += mousevel;
		}
		else if(state[SDLK_s])
		{
            m_PitchDegrees -= mousevel;
		}


        if(state[SDLK_a])
		{
            m_YawDegrees += mousevel;
		}
		else if(state[SDLK_d])
		{
            m_YawDegrees -= mousevel;
		}
        RotateOrbit(m_pipeline);
*/

		/*
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
*/
    }
}


void EG_Camera::updateViewMatrix(pipeline& m_pipeline)
{
    m_viewMatrix = glm::toMat4(m_orientation);


    m_xAxis = glm::vec3(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
    m_yAxis = glm::vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
    m_zAxis = glm::vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);
    m_viewDir = -m_zAxis;

    /// m_zAxis is the opposite of View_Direciton, so we offset it that way
    m_eye = m_target + m_zAxis * m_orbitOffsetDistance;


 /*
     m_RotateViewMatrix = m_viewMatrix;

    m_RotateViewMatrix[0][0] = m_xAxis.x;
    m_RotateViewMatrix[1][0] = m_xAxis.y;
    m_RotateViewMatrix[2][0] = m_xAxis.z;
 //   m_RotateViewMatrix[3][0] = -glm::dot(m_xAxis, eye);

    m_RotateViewMatrix[0][1] = m_yAxis.x;
    m_RotateViewMatrix[1][1] = m_yAxis.y;
    m_RotateViewMatrix[2][1] = m_yAxis.z;
 //   m_RotateViewMatrix[3][1] = -glm::dot(m_yAxis, eye);

    m_RotateViewMatrix[0][2] = m_zAxis.x;
    m_RotateViewMatrix[1][2] = m_zAxis.y;
    m_RotateViewMatrix[2][2] = m_zAxis.z;
 //   m_RotateViewMatrix[3][2] = -glm::dot(m_zAxis, eye);
*/


    m_skyboxRotate = m_pipeline;
    m_skyboxRotate.matrixMode(VIEW_MATRIX);
    m_skyboxRotate.loadMatrix(m_viewMatrix);

 //   ROT_matrix = glm::toMat4(ROT);
 //   m_RotateViewMatrix = glm::toMat4(ROT);
    m_RotateViewMatrix = glm::translate(m_eye.x, m_eye.y, m_eye.z) * m_viewMatrix;

    m_viewMatrix[3][0] = -glm::dot(m_xAxis, m_eye);
    m_viewMatrix[3][1] = -glm::dot(m_yAxis, m_eye);
    m_viewMatrix[3][2] = -glm::dot(m_zAxis, m_eye);



/*
    m_RotateViewMatrix[3][0] = glm::dot(m_xAxis, m_eye);
    m_RotateViewMatrix[3][1] = glm::dot(m_yAxis, m_eye);
    m_RotateViewMatrix[3][2] = glm::dot(m_zAxis, m_eye);
*/

    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadMatrix(m_viewMatrix);
}



