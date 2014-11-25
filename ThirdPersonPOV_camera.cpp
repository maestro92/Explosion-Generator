
#include "ThirdPersonPOV_Camera.h"

static bool first = true;
Third_Person_POV_Camera::Third_Person_POV_Camera()
{




    m_PitchDegrees = 0.0f;
    m_YawDegrees = 0.0f;

    mousevel = 1.0f;
    mouse_in = false;

	m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_target = glm::vec3(0.0f, 0.0f, 0.0f);
    m_offset = glm::vec3(0.0f, 0.0f, 0.0f);

	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    m_viewMatrix = glm::mat4(1.0);

    m_PitchDegrees = 0.0f;
    m_PitchDegrees = 90.0f;

}



Third_Person_POV_Camera::~Third_Person_POV_Camera()
{


}

void Third_Person_POV_Camera::init(pipeline& m_pipeline)
{
    m_character = new meshLoader("./Characters/LEGO_Man.obj");
    c_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    c_eulerOrient = glm::vec3(0.0f, 0.0f, 0.0f);
    c_eulerRotate = glm::vec3(0.0f, 0.0f, 0.0f);


    c_right.x = 1.0f;   c_right.y = 0.0f;   c_right.z = 0.0f;
    c_up.x = 0.0f;      c_up.y = 1.0f;      c_up.z = 0.0f;
    c_forward.x = 0.0f; c_forward.y = 0.0f; c_forward.z = -1.0f;

 //   c_Orientation = glm::quat(glm::rotate(180.0f, 0.0f, 1.0f, 0.0f));



/*
    glm::mat4 m = glm::toMat4(c_Orientation);

    c_right.x = m[0][0];    c_right.y = m[0][1];   c_right.z = m[0][2];
    c_right = glm::normalize(c_right);

    c_up.x = m[1][0];    c_up.y = m[1][1];   c_up.z = m[1][2];
    c_up = glm::normalize(c_up);

    c_forward.x = m[0][0];    c_forward.y = m[0][1];   c_forward.z = m[0][2];
    c_forward = glm::normalize(c_forward);
*/

/*
    c_right.x = 1.0f; c_right.y = 0.0f; c_right.z = 0.0f;

    c_up.x = 0.0f; c_up.y = 1.0f; c_up.z = 0.0f;

    c_forward.x = 0.0f; c_forward.y = 0.0f; c_forward.z = -1.0f;
*/

//    glm::vec3 eye_p = glm::vec3(0.0f, 13.0f, 25.0f);
    glm::vec3 eye_p = glm::vec3(0.0f, 13.0f, 25.0f);
    glm::vec3 target_p = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up_p = glm::vec3(0.0f, 1.0f, 0.0f);

    lookAt(m_pipeline, eye_p, target_p, up_p);
}

void Third_Person_POV_Camera::mouseIn(bool b)
{
    mouse_in = b;
}



void Third_Person_POV_Camera::lookAt(glm::vec3& target)
{
    m_target = target;
}

/*
void Third_Person_POV_Camera::lookAt(pipeline& m_pipeline, glm::vec3 target)
{
    lookAt(m_pipeline, m_eye, target, m_yAxis);
}
*/

void Third_Person_POV_Camera::lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up)
{
    m_eye = eye;
    m_target = target;

//    newEye = m_eye + m_zAxis * m_orbitOffsetDistance;
//    newTarget = m_eye;

    m_offset = m_zAxis = eye - target;
    m_zAxis = glm::normalize(m_zAxis);

    m_viewDir = -m_zAxis;

    m_xAxis = glm::cross(up, m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);

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

    m_orientation = glm::toQuat(m_viewMatrix);
	updateViewMatrix(m_pipeline);

    cout << "m_eye " << m_eye.x << ", " << m_eye.y << ", " << m_eye.z << endl;
}

/*
void Third_Person_POV_Camera::RotateOrbit(pipeline& m_pipeline)
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
        rot = glm::angleAxis(-m_PitchDegrees, WORLD_XAXIS);
   //     rot = glm::rotate(-m_PitchDegrees, 1.0f,0.0f,0.0f);

        ROT = rot * ROT;
        m_orientation = rot * m_orientation;


    }

    updateViewMatrix(m_pipeline);

}
*/

void Third_Person_POV_Camera::Rotate(pipeline& m_pipeline, float pitch, float yaw)
{
    m_PitchDegrees = pitch;
    m_YawDegrees = yaw;
}

void Third_Person_POV_Camera::update(pipeline& m_pipeline)
{
  //  float total_PitchDegrees = 0.031f * m_PitchDegrees;
  //  float total_YawDegrees = 0.031f * m_YawDegrees;

    float total_PitchDegrees = (3.14f / 180.0f) * m_PitchDegrees;
    float total_YawDegrees = (3.14f / 180.0f) * m_YawDegrees;

    cout << "total_YawDegrees " << total_YawDegrees << endl;

    /// quaternion rotation of the OffsetVector
    glm::quat rotation(glm::vec3(total_PitchDegrees, total_YawDegrees, 0.0f));
    /// offset is the difference between eye and target,
    /// so this rotation transformation is relative to the target point already
    glm::quat offsetVector(0.0f, m_offset.x, m_offset.y, m_offset.z);
    glm::quat result = glm::conjugate(rotation) * offsetVector * rotation;

    /// transformedOffsetVector
    glm::vec3 transformedOffsetVector = glm::vec3(result.x, result.y, result.z);
    glm::vec3 newCameraPosition = transformedOffsetVector + m_target;

    glm::vec3 world_yaxis = glm::vec3(0.0f, 1.0f, 0.0f);
    ///rebuild the viewMatrix
    lookAt(m_pipeline, newCameraPosition, m_target, world_yaxis);
}

// https://www.opengl.org/archives/resources/faq/technical/viewing.htm
void Third_Person_POV_Camera::Control(pipeline& m_pipeline)
{
    float pitch = 0.0f;
    float heading = 0.0f;
    float forwardSpeed = 0.0f;

	if(mouse_in)
	{
		SDL_ShowCursor(SDL_DISABLE);

		Uint8* state=SDL_GetKeyState(NULL);

        if(state[SDLK_w])
		{
            forwardSpeed = BALL_FORWARD_SPEED;
      //      c_Position.z -= BALL_FORWARD_SPEED;
            pitch = -BALL_ROLLING_SPEED;
		}
		if(state[SDLK_s])
		{
            forwardSpeed = -BALL_FORWARD_SPEED;
      //      c_Position.z += BALL_FORWARD_SPEED;
            pitch = BALL_ROLLING_SPEED;
		}


        if(state[SDLK_a])
		{
            heading = -BALL_HEADING_SPEED;

		}
		if(state[SDLK_d])
		{
            heading = BALL_HEADING_SPEED;
		}

    /// update the character first
    c_Velocity.x = 0.0f;
    c_Velocity.y = 0.0f;
    c_Velocity.z = forwardSpeed;

//    CharacterOrient(0.0f, -heading, 0.0f);

    CharacterOrient(0.0f, -heading, 0.0f);

 //   CharacterRotate(pitch, 0.0f, 0.0f);
    CharacterUpdate();
 //   CharacterUpdate(0.0f, heading, 0.0f);



    Rotate(m_pipeline,  0.0f, (forwardSpeed >= 0.0f) ? heading : -heading);
  //  glm::vec3 newTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    lookAt(c_Position);
    update(m_pipeline);

//        RotateOrbit(m_pipeline);
//	m_pipeline.rotateX(camPitch);
//	m_pipeline.rotateY(camYaw);

    }
}


#if 0
// https://www.opengl.org/archives/resources/faq/technical/viewing.htm
void Third_Person_POV_Camera::Control(pipeline& m_pipeline, int mid_x, int mid_y)
{
    /*
    if(first)
    {
        lookAt(m_pipeline, newEye, newTarget, m_targetYAxis);
        first = false;
    }
*/

    cout << "Pitch " << m_PitchDegrees << endl;
    cout << "Yaw " << m_YawDegrees << endl;

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

        m_pipeline.Rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f);
        m_pipeline.Rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f);

        m_skyboxRotate.Rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f);
        m_skyboxRotate.Rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f);

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
  //      RotateOrbit(m_pipeline);

  //      moveCamera();
  //      moveCameraUp();


        m_pipeline.Rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f);
        m_pipeline.Rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f);

        m_skyboxRotate.Rotate(-m_PitchDegrees, 1.0f, 0.0f, 0.0f);
        m_skyboxRotate.Rotate(-m_YawDegrees, 0.0f, 1.0f, 0.0f);
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
#endif

void Third_Person_POV_Camera::updateViewMatrix(pipeline& m_pipeline)
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
*/

    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.LoadMatrix(m_viewMatrix);
}


void Third_Person_POV_Camera::CharacterUpdate()
{
    glm::vec3 old_pos, yaw, heading;
    glm::quat temp;
/*
    glm::vec3 TotalVelocity = 0.031 * c_Velocity;
    glm::vec3 TotaleulerOrient = 0.031 * c_eulerOrient;
    glm::vec3 TotaleulerRotate = 0.031 * c_eulerRotate;
*/
    glm::vec3 TotalVelocity, TotaleulerOrient, TotaleulerRotate;
/*
    TotalVelocity.x = 0.031 * c_Velocity.x;
    TotalVelocity.y = 0.031 * c_Velocity.y;
    TotalVelocity.z = 0.031 * c_Velocity.z;

    TotaleulerOrient.x = 0.031 * c_eulerOrient.x;
    TotaleulerOrient.y = 0.031 * c_eulerOrient.y;
    TotaleulerOrient.z = 0.031 * c_eulerOrient.z;

    TotaleulerRotate.x = 0.031 * c_eulerRotate.x;
    TotaleulerRotate.y = 0.031 * c_eulerRotate.y;
    TotaleulerRotate.z = 0.031 * c_eulerRotate.z;
*/

    TotalVelocity.x = 2.0f * c_Velocity.x;
    TotalVelocity.y = 2.0f * c_Velocity.y;
    TotalVelocity.z = 2.0f * c_Velocity.z;

    TotaleulerOrient.x = 1.0f * c_eulerOrient.x;
    TotaleulerOrient.y = 1.0f * c_eulerOrient.y;
    TotaleulerOrient.z = 1.0f * c_eulerOrient.z;

    TotaleulerRotate.x = 1.0f * c_eulerRotate.x;
    TotaleulerRotate.y = 1.0f * c_eulerRotate.y;
    TotaleulerRotate.z = 1.0f * c_eulerRotate.z;

    cout << "TotalEulerOrient.y " << TotaleulerOrient.y << endl;

    /// extract the axes
    glm::mat4 m = glm::toMat4(c_Orientation);

    c_right.x = m[0][0];    c_right.y = m[0][1];   c_right.z = m[0][2];
    c_right = glm::normalize(c_right);

    c_up.x = m[1][0];    c_up.y = m[1][1];   c_up.z = m[1][2];
    c_up = glm::normalize(c_up);

    c_forward.x = -m[2][0];    c_forward.y = -m[2][1];   c_forward.z = -m[2][2];
    c_forward = glm::normalize(c_forward);

    /// update the entity's position
    old_pos = c_Position;
    c_Position += c_right * TotalVelocity.x;
    c_Position += c_up * TotalVelocity.y;
    c_Position += c_forward * TotalVelocity.z;


    heading = c_Position - old_pos;
    heading = glm::normalize(heading);



    /// Update the entity's orientation.

    temp = eulerToQuaternion(glm::toMat4(c_Orientation), TotaleulerOrient.x,
            TotaleulerOrient.y, TotaleulerOrient.z);

    /// When moving backwards invert rotations to match direction of travel.
    if (glm::dot(heading, c_forward) < 0.0f)
        temp = glm::inverse(temp);

    c_Orientation = temp * c_Orientation;
    c_Orientation = glm::normalize(c_Orientation);



    temp = glm::normalize(c_Orientation);

    c_WorldMatrix = glm::toMat4(temp);
    c_WorldMatrix[3][0] = c_Position.x;
    c_WorldMatrix[3][1] = c_Position.y;
    c_WorldMatrix[3][2] = c_Position.z;




    c_Velocity.x = 0.0f;    c_Velocity.y = 0.0f;    c_Velocity.z = 0.0f;
    c_eulerOrient.x = 0.0f;    c_eulerOrient.y = 0.0f;    c_eulerOrient.z = 0.0f;
    c_eulerRotate.x = 0.0f;    c_eulerRotate.y = 0.0f;    c_eulerRotate.z = 0.0f;
}






void Third_Person_POV_Camera::CharacterUpdate(float pitch, float yaw, float roll)
{
    glm::vec3 old_pos, heading;
    glm::quat temp;

    glm::vec3 TotalVelocity, TotaleulerOrient, TotaleulerRotate;
/*
    TotalVelocity.x = 0.031 * c_Velocity.x;
    TotalVelocity.y = 0.031 * c_Velocity.y;
    TotalVelocity.z = 0.031 * c_Velocity.z;

    TotaleulerOrient.x = 0.031 * c_eulerOrient.x;
    TotaleulerOrient.y = 0.031 * c_eulerOrient.y;
    TotaleulerOrient.z = 0.031 * c_eulerOrient.z;

    TotaleulerRotate.x = 0.031 * c_eulerRotate.x;
    TotaleulerRotate.y = 0.031 * c_eulerRotate.y;
    TotaleulerRotate.z = 0.031 * c_eulerRotate.z;
*/

    TotaleulerOrient.x = pitch;
    TotaleulerOrient.y = yaw;
    TotaleulerOrient.z = roll;

    cout << "TotalEulerOrient.y " << TotaleulerOrient.y << endl;

    /// extract the axes
    glm::mat4 m = glm::toMat4(c_Orientation);

    c_right.x = m[0][0];    c_right.y = m[0][1];   c_right.z = m[0][2];
    c_right = glm::normalize(c_right);

    c_up.x = m[1][0];    c_up.y = m[1][1];   c_up.z = m[1][2];
    c_up = glm::normalize(c_up);

    c_forward.x = -m[2][0];    c_forward.y = -m[2][1];   c_forward.z = -m[2][2];
    c_forward = glm::normalize(c_forward);

    /// update the entity's position
    old_pos = c_Position;
    c_Position += c_right * TotalVelocity.x;
    c_Position += c_up * TotalVelocity.y;
    c_Position += c_forward * TotalVelocity.z;


    heading = c_Position - old_pos;
    heading = glm::normalize(heading);



    /// Update the entity's orientation.

    temp = eulerToQuaternion(glm::toMat4(c_Orientation), pitch, -yaw, roll);

    /// When moving backwards invert rotations to match direction of travel.
    if (glm::dot(heading, c_forward) < 0.0f)
        temp = glm::inverse(temp);

    c_Orientation = temp * c_Orientation;
    c_Orientation = glm::normalize(c_Orientation);



    temp = glm::normalize(c_Orientation);

    c_WorldMatrix = glm::toMat4(temp);
    c_WorldMatrix[3][0] = c_Position.x;
    c_WorldMatrix[3][1] = c_Position.y;
    c_WorldMatrix[3][2] = c_Position.z;


    c_Velocity.x = 0.0f;    c_Velocity.y = 0.0f;    c_Velocity.z = 0.0f;
    c_eulerOrient.x = 0.0f;    c_eulerOrient.y = 0.0f;    c_eulerOrient.z = 0.0f;
    c_eulerRotate.x = 0.0f;    c_eulerRotate.y = 0.0f;    c_eulerRotate.z = 0.0f;
}



/// changes the way the model is facing
void Third_Person_POV_Camera::CharacterOrient(float pitch, float yaw, float roll)
{
    c_eulerOrient.x += pitch;
    c_eulerOrient.y += yaw;
    c_eulerOrient.z += roll;

    if (c_eulerOrient.x > 360.0f)
        c_eulerOrient.x -= 360.0f;

    if (c_eulerOrient.x < -360.0f)
        c_eulerOrient.x += 360.0f;

    if (c_eulerOrient.y > 360.0f)
        c_eulerOrient.y -= 360.0f;

    if (c_eulerOrient.y < -360.0f)
        c_eulerOrient.y += 360.0f;

    if (c_eulerOrient.z > 360.0f)
        c_eulerOrient.z -= 360.0f;

    if (c_eulerOrient.z < -360.0f)
        c_eulerOrient.z += 360.0f;
}

/// changes the way the model is facing
void Third_Person_POV_Camera::CharacterRotate(float pitch, float yaw, float roll)
{
    c_eulerRotate.x += pitch;
    c_eulerRotate.y += yaw;
    c_eulerRotate.z += roll;

    if (c_eulerRotate.x > 360.0f)
        c_eulerRotate.x -= 360.0f;

    if (c_eulerRotate.x < -360.0f)
        c_eulerRotate.x += 360.0f;

    if (c_eulerRotate.y > 360.0f)
        c_eulerRotate.y -= 360.0f;

    if (c_eulerRotate.y < -360.0f)
        c_eulerRotate.y += 360.0f;

    if (c_eulerRotate.z > 360.0f)
        c_eulerRotate.z -= 360.0f;

    if (c_eulerRotate.z < -360.0f)
        c_eulerRotate.z += 360.0f;
}



glm::quat Third_Person_POV_Camera::eulerToQuaternion(glm::mat4 m, float pitch, float yaw, float roll)
{
    glm::quat result;
    glm::quat rotation;

    glm::vec3 localXAxis(m[0][0], m[0][1], m[0][2]);
    glm::vec3 localYAxis(m[1][0], m[1][1], m[1][2]);
    glm::vec3 localZAxis(m[2][0], m[2][1], m[2][2]);


    if (pitch != 0.0f)
    {
        rotation = glm::angleAxis(pitch, localXAxis);
        result = rotation * result;
    }

    if (yaw != 0.0f)
    {
        rotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        result = rotation * result;
    }


    if (roll != 0.0f)
    {
        rotation = glm::angleAxis(roll, localZAxis);
        result = rotation * result;
    }

    return result;

}

