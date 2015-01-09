
#include "EG_ThirdPersonPovCamera.h"
#define USING_INVERSE_VIEW_MATRIX 1

static bool first = true;


const float EG_ThirdPersonPovCamera::DEFAULT_SPRING_CONSTANT = 16.0f;
const float EG_ThirdPersonPovCamera::DEFAULT_DAMPING_CONSTANT = 8.0f;


EG_ThirdPersonPovCamera::EG_ThirdPersonPovCamera()
{
    m_springConstant  = DEFAULT_SPRING_CONSTANT;
    m_dampingConstant = DEFAULT_DAMPING_CONSTANT;
    m_enableSpringSystem = true;

    m_pitchDegrees = 0.0f;
    m_yawDegrees = 0.0f;

    mousevel = 1.0f;
    mouse_in = false;

	m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	m_target = glm::vec3(0.0f, 0.0f, 0.0f);
    m_offset = glm::vec3(0.0f, 0.0f, 0.0f);
    m_offsetDistance = glm::length(m_offset);

	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    m_viewMatrix = glm::mat4(1.0);

    m_pitchDegrees = 0.0f;
    m_pitchDegrees = 90.0f;

}



EG_ThirdPersonPovCamera::~EG_ThirdPersonPovCamera()
{


}

void EG_ThirdPersonPovCamera::init(pipeline& m_pipeline)
{
    m_character = new meshLoader("./Characters/LEGO_Man.obj");
    c_position = glm::vec3(0.0f, 0.0f, 0.0f);
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
    glm::vec3 eye_p = glm::vec3(0.0f, 13.0f, 30.0f);
    glm::vec3 target_p = glm::vec3(0.0f, 4.0f, 0.0f);
    glm::vec3 up_p = glm::vec3(0.0f, 1.0f, 0.0f);

    lookAt(m_pipeline, eye_p, target_p, up_p);
}




void EG_ThirdPersonPovCamera::enableSpringSystem(bool enableSpringSystem)
{
    m_enableSpringSystem = enableSpringSystem;
}

void EG_ThirdPersonPovCamera::setSpringConstant(float springConstant)
{
    // We're using a critically damped spring system where the damping ratio
    // is equal to one.
    //
    // damping ratio = m_dampingConstant / (2.0f * sqrtf(m_springConstant))

    m_springConstant = springConstant;
    m_dampingConstant = 2.0f * sqrtf(springConstant);
}



void EG_ThirdPersonPovCamera::mouseIn(bool b)
{
    mouse_in = b;
}


void EG_ThirdPersonPovCamera::setTarget(glm::vec3& target)
{
    m_target = target;
}

// http://3dgep.com/understanding-the-view-matrix/
void EG_ThirdPersonPovCamera::lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up)
{
    m_eye = eye;
    m_target = target;

//    newEye = m_eye + m_zAxis * m_orbitOffsetDistance;
//    newTarget = m_eye;

    m_offset = m_zAxis = eye - target;
    m_offsetDistance = glm::length(m_offset);
    m_zAxis = glm::normalize(m_zAxis);

    m_viewDir = -m_zAxis;

    m_xAxis = glm::cross(up, m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);

#if USING_INVERSE_VIEW_MATRIX
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

    /// these two are equivalent
    /// see http://blog.hvidtfeldts.net/index.php/2014/01/combining-ray-tracing-and-polygons/
#else
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

    m_viewMatrix = glm::inverse(m_viewMatrix);
#endif
    m_orientation = glm::toQuat(m_viewMatrix);
	updateViewMatrix(m_pipeline);


 //   cout << "m_eye " << m_eye.x << ", " << m_eye.y << ", " << m_eye.z << endl;
}


void EG_ThirdPersonPovCamera::rotate(pipeline& m_pipeline, float pitch, float yaw)
{
    m_pitchDegrees += pitch;
    m_yawDegrees += yaw;
}

/*
void EG_ThirdPersonPovCamera::update(pipeline& m_pipeline)
{
  //  float total_PitchDegrees = 0.031f * m_PitchDegrees;
  //  float total_YawDegrees = 0.031f * m_YawDegrees;

    float totalPitchDegrees = (M_PI / 180.0f) * m_pitchDegrees;
    float totalYawDegrees = (M_PI / 180.0f) * m_yawDegrees;

    cout << "totalPitchDegrees " << totalPitchDegrees << endl;
    cout << "totalYawDegrees " << totalYawDegrees << endl;

    /// quaternion rotation of the OffsetVector
    glm::quat cameraRotation(glm::vec3(totalPitchDegrees, totalYawDegrees, 0.0f));
    /// offset is the difference between eye and target,
    /// so this rotation transformation is relative to the target point already
    glm::quat offsetVector(0.0f, m_offset.x, m_offset.y, m_offset.z);
    glm::quat result = glm::conjugate(cameraRotation) * offsetVector * cameraRotation;
    cout << "m_offset "<< m_offset.x << ", " << m_offset.y << ", " << m_offset.z << endl;

    /// transformedOffsetVector
    glm::vec3 transformedOffsetVector = glm::vec3(result.x, result.y, result.z);
    glm::vec3 newCameraPosition = transformedOffsetVector + m_target;

    glm::vec3 world_yaxis = glm::vec3(0.0f, 1.0f, 0.0f);
    ///rebuild the viewMatrix
    lookAt(m_pipeline, newCameraPosition, m_target, world_yaxis);
}
*/

glm::vec3 EG_ThirdPersonPovCamera::set(float x, float y, float z)
{
    glm::vec3 temp(x,y,z);
    return temp;
}

/// from GLThirdPersonCamera1
void EG_ThirdPersonPovCamera::update(pipeline& m_pipeline, float pitchChange, float yawChange)
{
    float pitchChangeRadians = (M_PI / 180.0f) * pitchChange;
    float yawChangeRadians = (M_PI / 180.0f) * yawChange;

    cout << "pitchChangeRadians " << pitchChangeRadians << endl;
    cout << "yawChangeRadians " << yawChangeRadians << endl;

    /// quaternion rotation of the OffsetVector
    glm::quat cameraRotation(glm::vec3(pitchChangeRadians, yawChangeRadians, 0.0f));
    /// offset is the difference between eye and target,
    /// so this rotation transformation is relative to the target point already
    glm::quat offsetVector(0.0f, m_offset.x, m_offset.y, m_offset.z);
 //   glm::quat result = cameraRotation * offsetVector * glm::conjugate(cameraRotation);
    glm::quat result = glm::conjugate(cameraRotation) * offsetVector * cameraRotation;

    cout << "m_offset "<< m_offset.x << ", " << m_offset.y << ", " << m_offset.z << endl;


    /// transformedOffsetVector
    glm::vec3 transformedOffsetVector = glm::vec3(result.x, result.y, result.z);
    glm::vec3 newCameraPosition = transformedOffsetVector + m_target;

    glm::vec3 world_yaxis = glm::vec3(0.0f, 1.0f, 0.0f);
    ///rebuild the viewMatrix
    lookAt(m_pipeline, newCameraPosition, m_target, world_yaxis);
}


/// from GLThirdPersonCamera2
void EG_ThirdPersonPovCamera::update2(pipeline& m_pipeline, float pitchChange, float yawChange)
{
    glm::quat rot;

    /// imagine the sequence of Model View Projection Matrix
    /// it will be projectionMatrix * viewMatrix * modelMatrix
    /// in this case, it will be
    /// projectionMatrix * (m_orientation * rot) * modelMatrix
    /// so by selecting
    ///         rot = glm::angleAxis(yawChange, glm::vec3(0,1,0));
    /// we're essentially turning the world first, then rotating our camera, then move our camera along our camera orientation
    if (yawChange != 0.0f)
    {
        rot = glm::angleAxis(yawChange, glm::vec3(0,1,0));
        m_orientation = m_orientation * rot;

/*
      /// local +y direction
        m_viewMatrix = glm::toMat4(m_orientation);
        m_yAxis = set(m_viewMatrix[1][0], m_viewMatrix[1][1], m_viewMatrix[1][2]);

        rot = glm::angleAxis(yawChange, m_yAxis);
        m_orientation = rot * m_orientation;
*/
    }

    if (pitchChange != 0.0f)
    {
        rot = glm::angleAxis(pitchChange, glm::vec3(1,0,0));
        m_orientation = rot * m_orientation;
    }

    /// constructing the my camera-viewMatrix, which is the inverse of the modelMatrix for the camera
    /// therefore my m_viewMatrix and m_orientation always stores my viewMatrix / inverse of (modelMatrix of camera)
    m_viewMatrix = glm::toMat4(m_orientation);

    /// that's why the x,y,z axis are extracted in this pattern
    m_xAxis = set(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
    m_yAxis = set(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
    m_zAxis = set(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);
    m_viewDir = -m_zAxis;

    m_eye = m_target + m_zAxis * glm::length(m_offset);

    m_viewMatrix[3][0] = -glm::dot(m_xAxis, m_eye);
    m_viewMatrix[3][1] = -glm::dot(m_yAxis, m_eye);
    m_viewMatrix[3][2] = -glm::dot(m_zAxis, m_eye);
    updateViewMatrix(m_pipeline);
}



void EG_ThirdPersonPovCamera::update2(pipeline& m_pipeline, float elapsedTimeSec, float pitchChange, float yawChange)
{
    glm::quat rot;

    /// imagine the sequence of Model View Projection Matrix
    /// it will be projectionMatrix * viewMatrix * modelMatrix
    /// in this case, it will be
    /// projectionMatrix * (m_orientation * rot) * modelMatrix
    /// so by selecting
    ///         rot = glm::angleAxis(yawChange, glm::vec3(0,1,0));
    /// we're essentially turning the world first, then rotating our camera, then move our camera along our camera orientation
    if (yawChange != 0.0f)
    {
        rot = glm::angleAxis(yawChange, glm::vec3(0,1,0));
        m_orientation = m_orientation * rot;
    }

    if (pitchChange != 0.0f)
    {
        rot = glm::angleAxis(pitchChange, glm::vec3(1,0,0));
        m_orientation = rot * m_orientation;
    }

    /// constructing the my camera-viewMatrix, which is the inverse of the modelMatrix for the camera
    /// therefore my m_viewMatrix and m_orientation always stores my viewMatrix / inverse of (modelMatrix of camera)
    m_viewMatrix = glm::toMat4(m_orientation);

    /// that's why the x,y,z axis are extracted in this pattern
    m_xAxis = set(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
    m_yAxis = set(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
    m_zAxis = set(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);

//    glm::vec3 idealPosition = m_target + m_zAxis * glm::length(m_offset);
    glm::vec3 idealPosition = m_target + m_zAxis * m_offsetDistance;
    glm::vec3 displacement = m_eye - idealPosition;

    /// spring force F = kx
    /// ma = kx - damping * v
    glm::vec3 springAcceleration = (-m_springConstant * displacement) -
                            (m_dampingConstant * m_velocity);

//    cout << "m_velocity is " << m_velocity.x << ", " << m_velocity.y << ", " << m_velocity.z << endl;

/*
    velocity = accel * dt
    position = velocity * dt
*/
    m_velocity += springAcceleration * elapsedTimeSec;
    m_eye += m_velocity * elapsedTimeSec;

//    cout << "m_velocity is " << m_velocity.x << ", " << m_velocity.y << ", " << m_velocity.z << endl;

/*
    glm::vec3 springAcceleration = (-m_springConstant * displacement) -
                            (m_dampingConstant * c_velocity);

    cout << "c_velocity is " << c_velocity.x << ", " << c_velocity.y << ", " << c_velocity.z << endl;

    c_velocity += springAcceleration * elapsedTimeSec;
    m_eye += c_velocity * elapsedTimeSec;

    cout << "c_velocity is " << c_velocity.x << ", " << c_velocity.y << ", " << c_velocity.z << endl;
*/

    m_zAxis = m_eye - m_target;
    m_zAxis = glm::normalize(m_zAxis);

    m_xAxis = glm::cross(glm::vec3(0.0f,1.0f,0.0f), m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);

    m_viewMatrix[0][0] = m_xAxis.x;
    m_viewMatrix[1][0] = m_xAxis.y;
    m_viewMatrix[2][0] = m_xAxis.z;
    m_viewMatrix[3][0] = -glm::dot(m_xAxis, m_eye);

    m_viewMatrix[0][1] = m_yAxis.x;
    m_viewMatrix[1][1] = m_yAxis.y;
    m_viewMatrix[2][1] = m_yAxis.z;
    m_viewMatrix[3][1] = -glm::dot(m_yAxis, m_eye);

    m_viewMatrix[0][2] = m_zAxis.x;
    m_viewMatrix[1][2] = m_zAxis.y;
    m_viewMatrix[2][2] = m_zAxis.z;
    m_viewMatrix[3][2] = -glm::dot(m_zAxis, m_eye);

    m_viewDir = -m_zAxis;
	updateViewMatrix(m_pipeline);
}





// https://www.opengl.org/archives/resources/faq/technical/viewing.htm
void EG_ThirdPersonPovCamera::Control(pipeline& m_pipeline)
{
    float pitchChange = 0.0f;
    float yawChange = 0.0f;
    float forwardSpeed = 0.0f;

	if(mouse_in)
	{
		SDL_ShowCursor(SDL_DISABLE);

		Uint8* state=SDL_GetKeyState(NULL);

        if(state[SDLK_w])
		{
            forwardSpeed = BALL_FORWARD_SPEED;
      //      c_Position.z -= BALL_FORWARD_SPEED;
            pitchChange = -BALL_ROLLING_SPEED;
		}
		if(state[SDLK_s])
		{
            forwardSpeed = -BALL_FORWARD_SPEED;
      //      c_Position.z += BALL_FORWARD_SPEED;
            pitchChange = BALL_ROLLING_SPEED;
		}

        if(state[SDLK_a])
		{
            yawChange = -BALL_HEADING_SPEED;
		}
		if(state[SDLK_d])
		{
            yawChange = BALL_HEADING_SPEED;
		}


        /// mouse wheel input


        ///


    /// update the character first
    c_velocity.x = 0.0f;
    c_velocity.y = 0.0f;
    c_velocity.z = forwardSpeed;


    updateCharacterOrientation(0.0f, -yawChange, 0.0f);
 //   CharacterRotate(pitch, 0.0f, 0.0f);
    updateCharacter(0.0f, -yawChange, 0.0f);
 //   CharacterUpdate(0.0f, heading, 0.0f);

 //   rotate(m_pipeline,  0.0f, (forwardSpeed >= 0.0f) ? yawChange : -yawChange);
    setTarget(c_position);
//    update(m_pipeline);
//    update2(m_pipeline, 0.0f, (forwardSpeed >= 0.0f) ? yawChange : -yawChange);
    update2(m_pipeline, 0.031f, 0.0f, (forwardSpeed >= 0.0f) ? yawChange : -yawChange);


//        RotateOrbit(m_pipeline);
//	m_pipeline.rotateX(camPitch);
//	m_pipeline.rotateY(camYaw);

    }
}



void EG_ThirdPersonPovCamera::updateViewMatrix(pipeline& m_pipeline)
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


void EG_ThirdPersonPovCamera::updateCharacter(float pitchChange, float yawChange, float rollChange)
{
    glm::vec3 oldPosition, positionChange;
    glm::quat orientationChange;
    glm::vec3 TotalVelocity, TotaleulerRotate;

    TotalVelocity.x = 1.0f * c_velocity.x;
    TotalVelocity.y = 1.0f * c_velocity.y;
    TotalVelocity.z = 1.0f * c_velocity.z;

    TotaleulerRotate.x = 1.0f * c_eulerRotate.x;
    TotaleulerRotate.y = 1.0f * c_eulerRotate.y;
    TotaleulerRotate.z = 1.0f * c_eulerRotate.z;

//    cout << "TotalEulerOrient.y " << TotaleulerOrient.y << endl;

    /// extract the axes, compute the local x,y,z axis
    /// http://www.songho.ca/opengl/gl_anglestoaxes.html    for more information on orientation
    /// c_orientation, we treat it as a model matrix, so we extract local axes this way. (different from our camera)
    glm::mat4 m = glm::toMat4(c_orientation);

    /// +x direction
    c_right.x = m[0][0];    c_right.y = m[0][1];   c_right.z = m[0][2];
    c_right = glm::normalize(c_right);

    /// +y direction
    c_up.x = m[1][0];    c_up.y = m[1][1];   c_up.z = m[1][2];
    c_up = glm::normalize(c_up);

    /// -z direction
    c_forward.x = -m[2][0];    c_forward.y = -m[2][1];   c_forward.z = -m[2][2];
    c_forward = glm::normalize(c_forward);


    /// update the entity's position
    oldPosition = c_position;
    c_position += c_right * TotalVelocity.x;
    c_position += c_up * TotalVelocity.y;
    c_position += c_forward * TotalVelocity.z;

    positionChange = c_position - oldPosition;
    positionChange = glm::normalize(positionChange);


    /// Update the entity's orientation
    orientationChange = computeOrientationChange(c_right,       c_up,       -c_forward,
                                                 pitchChange,   yawChange,  rollChange);

    /// When moving backwards invert rotations to match direction of travel.
    if (glm::dot(positionChange, c_forward) < 0.0f)
    {
        orientationChange = glm::inverse(orientationChange);
        cout << "i'm here inversing " << endl;
    }

    /// c_orientation is the current orientation
    c_orientation = orientationChange * c_orientation;
    c_orientation = glm::normalize(c_orientation);


    /// setting the modelmatrix of the main character
    c_orientation = glm::normalize(c_orientation);
    c_worldMatrix = glm::toMat4(c_orientation);
    c_worldMatrix[3][0] = c_position.x;
    c_worldMatrix[3][1] = c_position.y;
    c_worldMatrix[3][2] = c_position.z;

    c_velocity.x = 0.0f;    c_velocity.y = 0.0f;    c_velocity.z = 0.0f;
//    c_eulerOrient.x = 0.0f;    c_eulerOrient.y = 0.0f;    c_eulerOrient.z = 0.0f;
//    c_eulerRotate.x = 0.0f;    c_eulerRotate.y = 0.0f;    c_eulerRotate.z = 0.0f;
}


glm::quat EG_ThirdPersonPovCamera::computeOrientationChange(glm::vec3 localXAxis, glm::vec3 localYAxis, glm::vec3 localZAxis,
                                                            float pitch, float yaw, float roll)
{
    glm::quat result;
    glm::quat rotation;

    if (pitch != 0.0f)
    {
        rotation = glm::angleAxis(pitch, localXAxis);
        result = rotation * result;
    }

    if (yaw != 0.0f)
    {
//        rotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        rotation = glm::angleAxis(yaw, localYAxis);
        result = rotation * result;
    }

    if (roll != 0.0f)
    {
        rotation = glm::angleAxis(roll, localZAxis);
        result = rotation * result;
    }

    return result;
}


glm::vec3 EG_ThirdPersonPovCamera::getCharacterPosition()
{
    return c_position;
}

void EG_ThirdPersonPovCamera::setCharacterPosition(glm::vec3 c_pos)
{
    c_position = c_pos;
}

void EG_ThirdPersonPovCamera::setCharacterPosition(float x, float y, float z)
{
    c_position.x = x; c_position.y = y; c_position.z = z;
}


float EG_ThirdPersonPovCamera::setPitch(float pitch)
{
    m_pitchDegrees = pitch;
}

float EG_ThirdPersonPovCamera::setYaw(float yaw)
{
    m_yawDegrees = yaw;
}

float EG_ThirdPersonPovCamera::getPitch()
{
    return m_pitchDegrees;
}

float EG_ThirdPersonPovCamera::getYaw()
{
    return m_yawDegrees;
}

/// changes the way the model is facing
void EG_ThirdPersonPovCamera::updateCharacterOrientation(float pitchChange, float yawChange, float rollChange)
{
    c_eulerOrient.x += pitchChange;
    c_eulerOrient.y += yawChange;
    c_eulerOrient.z += rollChange;

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
void EG_ThirdPersonPovCamera::updateCharacterRotation(float pitchChange, float yawChange, float rollChange)
{
    c_eulerRotate.x += pitchChange;
    c_eulerRotate.y += yawChange;
    c_eulerRotate.z += rollChange;

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
/*
void EG_ThirdPersonPovCamera::increaseOffsetDistance()
{
    if( (m_offsetDistance+1.0f) > MAX_CAMERA_OFFSET)
        m_offsetDistance = MAX_CAMERA_OFFSET;
    else
        m_offsetDistance+=(1.0f);

}

void EG_ThirdPersonPovCamera::decreaseOffsetDistance()
{
    if( (m_offsetDistance-1.0f)< MIN_CAMERA_OFFSET)
        m_offsetDistance = MIN_CAMERA_OFFSET;
    else
        m_offsetDistance-=(1.0f);
}
*/

void EG_ThirdPersonPovCamera::increaseOffsetDistance()
{
    if( (m_offsetDistance+CAMERA_ZOOM_DEGREE) > MAX_CAMERA_OFFSET)
        m_offsetDistance = MAX_CAMERA_OFFSET;
    else
        m_offsetDistance+=(CAMERA_ZOOM_DEGREE);
}

void EG_ThirdPersonPovCamera::decreaseOffsetDistance()
{
    if( (m_offsetDistance-CAMERA_ZOOM_DEGREE)< MIN_CAMERA_OFFSET)
        m_offsetDistance = MIN_CAMERA_OFFSET;
    else
        m_offsetDistance-=(CAMERA_ZOOM_DEGREE);
}





