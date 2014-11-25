#include "FirstPersonPOV_QuaternionCamera.h"


/*
FirstPersonPOV_QuaternionCamera::FirstPersonPOV_QuaternionCamera()
{
    m_PitchDegrees = 0.0f;
    m_YawDegrees = 0.0f;

    m_mouseRotationSpeed = DEFAULT_MOUSE_ROTATION_SPEED;
    m_orbitMinZoom = DEFAULT_ORBIT_MIN_ZOOM;
    m_orbitMaxZoom = DEFAULT_ORBIT_MAX_ZOOM;
 //   m_orbitOffsetDistance = DEFAULT_ORBIT_OFFSET_DISTANCE;
    m_orbitOffsetDistance = 30.0f;

    mousevel = 1.0f;
    mouse_in = false;


	m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

//    m_orientation = glm::toQuat(mat4(1.0));
    m_viewMatrix = glm::mat4(1.0);
}


void FirstPersonPOV_QuaternionCamera::init(pipeline& m_pipeline)
{
	m_eye = glm::vec3(0.0f, 1.0f, 0.0f);
	updateViewMatrix(m_pipeline);
}


void FirstPersonPOV_QuaternionCamera::lookAt(pipeline& m_pipeline, glm::vec3 target)
{

}


void FirstPersonPOV_QuaternionCamera::lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up)
{

}


void FirstPersonPOV_QuaternionCamera::Rotate(pipeline& m_pipeline)
{

    updateViewMatrix(m_pipeline);
}

void FirstPersonPOV_QuaternionCamera::Control(pipeline& m_pipeline, int mid_x, int mid_y, glm::vec3& direction)
{
    if(mouse_in)
	{
        direction = glm::vec3(0.0f,0.0f,0.0f);

		int MidX=mid_x;
		int MidY=mid_y;
		SDL_ShowCursor(SDL_DISABLE);

		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);

		/// rotate orbit
		m_YawDegrees += mousevel*(MidX-tmpx);
        m_PitchDegrees += mousevel*(MidY-tmpy);
		SDL_WarpMouse(MidX,MidY);
 //       RotateOrbit(m_pipeline);


        SDL_ShowCursor(SDL_DISABLE);

		Uint8* state=SDL_GetKeyState(NULL);

        if(state[SDLK_w])
		{
            direction.z -= 1.0f;
		}
		else if(state[SDLK_s])
		{
            direction.z += 1.0f;
		}


        if(state[SDLK_a])
		{
            direction.x -= 1.0f;
		}
		else if(state[SDLK_d])
		{
            direction.x += 1.0f;
		}


	}
}
*/
void FirstPersonPOV_QuaternionCamera::updateViewMatrix(pipeline& m_pipeline)
{
    /*
    m_viewMatrix = glm::toMat4(m_orientation);

    m_xAxis = glm::vec3(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
    m_yAxis = glm::vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
    m_zAxis = glm::vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);
    m_viewDir = -m_zAxis;

    m_viewMatrix[3][0] = -glm::dot(m_xAxis, m_eye);
    m_viewMatrix[3][1] = -glm::dot(m_yAxis, m_eye);
    m_viewMatrix[3][2] = -glm::dot(m_zAxis, m_eye);

    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.LoadMatrix(m_viewMatrix);
*/
}



