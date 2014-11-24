#include "ThirdPersonPOV_camera.h"

Third_Person_POV_Camera::Third_Person_POV_Camera()
{
    m_longitudeDegrees = 0.0f;
    m_latitudeDegrees = 0.0f;

    eye_point = glm::vec3(0.0f, 0.0f, 0.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    offset = glm::vec3(0.0f, 0.0f, 0.0f);

    m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

    /// this loads the identity matrix
    m_viewMatrix = glm::mat4(1.0);
    glm::vec3 velocity;
    view_Direction = glm::vec3(0.0f,0.0f,-1.0f);
    glm::quat orientation;
}


/*

    First move the ball

    Then move the camera based on where the ball has moved to
    When the ball is moving backwards rotations are inverted to match the direction
    of travel. Consequently the camera's rotation needs to be inverted as well.
*/



void Third_Person_POV_Camera::Control(pipeline& m_pipeline, int mid_x, int mid_y)
{

    float heading = 0.0f;
    float forwardSpeed = 0.0f;

    if(mouse_in)
	{
		int MidX=mid_x;
		int MidY=mid_y;
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;

		/*
		SDL_GetMouseState(&tmpx,&tmpy);
		camYaw+=mousevel*(MidX-tmpx);
		camPitch+=mousevel*(MidY-tmpy);
		lockCamera();
		SDL_WarpMouse(MidX,MidY);
		*/

		Uint8* state=SDL_GetKeyState(NULL);
		ismoved=false;
		if(state[SDLK_w])
		{
		    forwardSpeed = BALL_FORWARD_SPEED;
			ismoved=true;
		}

		else if(state[SDLK_s])
		{
		    forwardSpeed = -BALL_FORWARD_SPEED;
			ismoved=true;
		}


		if(state[SDLK_a])
		{
            heading = -BALL_HEADING_SPEED;
			ismoved=true;
		}
		else if(state[SDLK_d])
		{
		    heading = BALL_HEADING_SPEED;
			ismoved=true;
		}
	}


}
