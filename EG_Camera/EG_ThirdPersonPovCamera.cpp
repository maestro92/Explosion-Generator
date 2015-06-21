
#include "EG_ThirdPersonPovCamera.h"
#define USING_INVERSE_VIEW_MATRIX 1

static bool first = true;

const float EG_ThirdPersonPovCamera::DEFAULT_SPRING_CONSTANT = 16.0f;
const float EG_ThirdPersonPovCamera::DEFAULT_DAMPING_CONSTANT = 8.0f;

EG_ThirdPersonPovCamera::EG_ThirdPersonPovCamera()
{
    cameraMode = CameraMode::THIRD_PERSON_POV_CAMERA_MODE;

    m_springConstant  = DEFAULT_SPRING_CONSTANT;
    m_dampingConstant = DEFAULT_DAMPING_CONSTANT;
    m_enableSpringSystem = true;

    m_pitchDegree = 0.0f;
    m_yawDegree = 0.0f;

    mousevel = 1.0f;
    mouse_in = false;



	m_eye       = glm::vec3(0.0f, 0.0f, 0.0f);
	m_target    = glm::vec3(0.0f, 0.0f, 0.0f);
    m_offset    = glm::vec3(0.0f, 0.0f, 0.0f);
    m_offsetDistance = glm::length(m_offset);

	m_xAxis     = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis     = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis     = glm::vec3(0.0f, 0.0f, 1.0f);
	m_velocity  = glm::vec3(0.0f, 0.0f, 0.0f);
	m_viewDir   = glm::vec3(0.0f, 0.0f, -1.0f);

    m_viewMatrix = glm::mat4(1.0);

}















EG_ThirdPersonPovCamera::~EG_ThirdPersonPovCamera()
{

}


void EG_ThirdPersonPovCamera::switchCameraMode()
{
    if(cameraMode == THIRD_PERSON_POV_CAMERA_MODE)
        cameraMode = FIRST_PERSON_POV_CAMERA_MODE;
    else
        cameraMode = THIRD_PERSON_POV_CAMERA_MODE;
}


void EG_ThirdPersonPovCamera::init(pipeline& m_pipeline)
{
    m_character = new meshLoader("./Characters/LEGO_Man.obj");
    m_characterHeight = 3.0f;
 //   m_pivotOffset.y = m_characterHeight;


    m_mainAvatar.loadModel("./models/Characters/Walking Lego.md5mesh");

    m_characterObject.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_characterObject.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    c_right.x = 1.0f;   c_right.y = 0.0f;   c_right.z = 0.0f;
    c_up.x = 0.0f;      c_up.y = 1.0f;      c_up.z = 0.0f;
    c_forward.x = 0.0f; c_forward.y = 0.0f; c_forward.z = -1.0f;

    glm::vec3 eye_p     = glm::vec3(0.0f, 30.0f, 80.0f);
    glm::vec3 target_p  = glm::vec3(0.0f, 4.0f, 0.0f);
    glm::vec3 up_p      = glm::vec3(0.0f, 1.0f, 0.0f);

    lookAt(m_pipeline, eye_p, target_p, up_p);

  //  m_pitchDegree = -abs( atan2(9.0f, 30.0f)) * 180 / M_PI;
//    cout << "m_pitchDegree is " << m_pitchDegree << endl;

  //  m_pitchDegree = asinf(m_viewMatrix[1][2]) * 180 / M_PI;

}



void EG_ThirdPersonPovCamera::initFirst(pipeline& m_pipeline)
{


}







void EG_ThirdPersonPovCamera::computeLocalAxisAndEyePosFromMatrix(glm::mat4& mat, int mat_type,
                                                                  glm::vec3& xAxis, glm::vec3& yAxis,
                                                                  glm::vec3& zAxis, glm::vec3& eye_pos)
{
    if(mat_type == (int)VIEW_MATRIX)
    {
        xAxis = glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
        yAxis = glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
        zAxis = glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
    }
    else if(mat_type == (int)MODEL_MATRIX)
    {
        xAxis = glm::vec3(mat[0][0], mat[0][1], mat[0][2]);
        yAxis = glm::vec3(mat[1][0], mat[1][1], mat[1][2]);
        zAxis = glm::vec3(-mat[2][0], -mat[2][1], -mat[2][2]);
    }

    xAxis = glm::normalize(xAxis);
    yAxis = glm::normalize(yAxis);
    zAxis = glm::normalize(zAxis);
}


void EG_ThirdPersonPovCamera::lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up)
{
    m_eye = eye;
    m_target = target;

    m_offset = m_zAxis = eye - target;
    m_offsetDistance = glm::length(m_offset);
    m_zAxis = glm::normalize(m_zAxis);

    m_viewDir = -m_zAxis;

    m_xAxis = glm::cross(up, m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);

    m_viewMatrix[0][0] = m_xAxis.x;    m_viewMatrix[1][0] = m_yAxis.x;    m_viewMatrix[2][0] = m_zAxis.x;    m_viewMatrix[3][0] = eye.x;
    m_viewMatrix[0][1] = m_xAxis.y;    m_viewMatrix[1][1] = m_yAxis.y;    m_viewMatrix[2][1] = m_zAxis.y;    m_viewMatrix[3][1] = eye.y;
    m_viewMatrix[0][2] = m_xAxis.z;    m_viewMatrix[1][2] = m_yAxis.z;    m_viewMatrix[2][2] = m_zAxis.z;    m_viewMatrix[3][2] = eye.z;
    m_viewMatrix = glm::inverse(m_viewMatrix);


    m_idealViewMatrix = m_viewMatrix;

	updateViewMatrix(m_pipeline);
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

void EG_ThirdPersonPovCamera::setTarget(glm::vec3 target)
{
    m_target = target;
}








void EG_ThirdPersonPovCamera::update(pipeline& m_pipeline, float elapsedTimeSec, float pitchChange, float yawChange, EG_SkyBox& skybox)
{
    glm::quat rot;

    if (m_enableSpringSystem)
    {
        /// imagine the sequence of Model View Projection Matrix
        /// it will be projectionMatrix * viewMatrix * modelMatrix
        /// in this case, it will be
        /// projectionMatrix * (m_orientation * rot) * modelMatrix
        /// so by selecting
        ///         rot = glm::angleAxis(yawChange, glm::vec3(0,1,0));
        /// we're essentially turning the world first, then rotating our camera, then move our camera along our camera orientation
        if (yawChange != 0.0f)
        {
            glm::mat4 rot_temp = glm::rotate(yawChange, 0.0f, 1.0f, 0.0f);
            m_idealViewMatrix = m_idealViewMatrix * rot_temp;
        }

        if (pitchChange != 0.0f)
        {
            glm::mat4 rot_temp = glm::rotate(pitchChange, 1.0f,0.0f,0.0f);
            m_idealViewMatrix = m_idealViewMatrix * rot_temp ;
        }

        glm::vec3 m_idealXAxis;
        glm::vec3 m_idealYAxis;
        glm::vec3 m_idealZAxis;
        glm::vec3 m_idealEye;

        /// extracting x,y,z axis from viewMatrix
        computeLocalAxisAndEyePosFromMatrix(m_idealViewMatrix, VIEW_MATRIX,
                                              m_idealXAxis, m_idealYAxis,
                                              m_idealZAxis, m_idealEye);


        glm::vec3 idealPosition = m_target + m_idealZAxis * m_offsetDistance;
        glm::vec3 displacement = m_eye - idealPosition;

        /// spring force F = kx
        /// ma = kx - damping * v
        glm::vec3 springAcceleration = (-m_springConstant * displacement) -
                                (m_dampingConstant * m_velocity);

        m_velocity += springAcceleration * elapsedTimeSec;
        m_eye += m_velocity * elapsedTimeSec;
    }
    else
    {

        if (yawChange != 0.0f)
        {
            glm::mat4 rot_temp = glm::rotate(yawChange, 0.0f, 1.0f, 0.0f);
            m_viewMatrix = m_viewMatrix * rot_temp;
        }



        if (pitchChange != 0.0f)
        {
            glm::mat4 rot_temp = glm::rotate(pitchChange, 1.0f,0.0f,0.0f);
            m_viewMatrix = m_viewMatrix * rot_temp ;
//            m_viewMatrix = rot_temp * m_viewMatrix;
        }


        /// extracting x,y,z axis from viewMatrix
        computeLocalAxisAndEyePosFromMatrix(m_viewMatrix, VIEW_MATRIX,
                                              m_xAxis, m_yAxis,
                                              m_zAxis, m_eye);

        /// if not using the spring system
        m_eye = m_target + m_zAxis * m_offsetDistance;
      //  debugVec3(m_eye, "m_eye");
    }


    m_zAxis = m_eye - m_target;
    m_zAxis = glm::normalize(m_zAxis);

    m_xAxis = glm::cross(glm::vec3(0.0f,1.0f,0.0f), m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);

    setViewMatrixRotation(m_xAxis, m_yAxis, m_zAxis);

    m_pipeline.pushMatrix();
        updateViewMatrix(m_pipeline);
        skybox.UpdateRotationOnly_View_Pipeline(m_pipeline);
    m_pipeline.popMatrix();

    setViewMatrixPosition(m_eye);


    m_viewMatrix = m_viewMatrix * glm::translate(0.0f, -m_characterHeight, 0.0f);

 //   m_viewMatrix = m_viewMatrix * glm::translate(-m_pivotOffset.x, -m_pivotOffset.y, -m_pivotOffset.z);


    m_viewDir = -m_zAxis;
	updateViewMatrix(m_pipeline);
  //  m_eye += m_characterHeight;
}

glm::vec3 EG_ThirdPersonPovCamera::getEyePoint()
{
    if (cameraMode == THIRD_PERSON_POV_CAMERA_MODE)
        return glm::vec3(m_eye.x, m_eye.y+m_characterHeight, m_eye.z);
    else
        return m_eye;
}





void EG_ThirdPersonPovCamera::setViewMatrixRotation(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
    m_viewMatrix[0][0] = xAxis.x;
    m_viewMatrix[1][0] = xAxis.y;
    m_viewMatrix[2][0] = xAxis.z;
    m_viewMatrix[3][0] = 0.0f;

    m_viewMatrix[0][1] = yAxis.x;
    m_viewMatrix[1][1] = yAxis.y;
    m_viewMatrix[2][1] = yAxis.z;
    m_viewMatrix[3][1] = 0.0f;

    m_viewMatrix[0][2] = zAxis.x;
    m_viewMatrix[1][2] = zAxis.y;
    m_viewMatrix[2][2] = zAxis.z;
    m_viewMatrix[3][2] = 0.0f;
}


void EG_ThirdPersonPovCamera::setViewMatrixPosition(glm::vec3 eye_pos)
{
    m_viewMatrix[3][0] = -glm::dot(m_xAxis, eye_pos);
    m_viewMatrix[3][1] = -glm::dot(m_yAxis, eye_pos);
    m_viewMatrix[3][2] = -glm::dot(m_zAxis, eye_pos);
}


void EG_ThirdPersonPovCamera::Control(pipeline& m_pipeline, EG_SkyBox& skybox)
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
            pitchChange = -BALL_ROLLING_SPEED;
		}
		if(state[SDLK_s])
		{
            forwardSpeed = -BALL_FORWARD_SPEED;
            pitchChange = BALL_ROLLING_SPEED;
		}

        if(state[SDLK_a])
            yawChange = -BALL_HEADING_SPEED;

		if(state[SDLK_d])
            yawChange = BALL_HEADING_SPEED;





        /// update the character first
        /// When moving backwards invert rotations to match direction of travel.
        /// When we drive backwards, our car actually turn leftwards when we steer rightwards
        m_characterObject.setVelocity(0.0f, 0.0f, forwardSpeed);
        m_characterObject.setAngularVelocity(0.0f, -yawChange, 0.0f);

        updateCharacterObject(0.0f, -yawChange, 0.0f);

        setTarget(m_characterObject.getPosition());
        update(m_pipeline, 0.031f, 0.0f, (forwardSpeed >= 0.0f) ? yawChange : -yawChange, skybox);
    }
}


/*
void EG_ThirdPersonPovCamera::render(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID)
{
    m_pipeline.pushMatrix();
        m_pipeline.LoadMatrix(c_worldMatrix);
        m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        m_character->draw();
    m_pipeline.popMatrix();
}
*/

/*
void EG_ThirdPersonPovCamera::render(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID)
{
    m_pipeline.pushMatrix();

//      these two are equivalent
//      m_pipeline.LoadMatrix(c_worldMatrix);

        m_pipeline.translate(characterObject.m_position);
    //    m_pipeline.translate(characterObject.m_scale);
        m_pipeline.loadMatrix(glm::toMat4(characterObject.m_rotation));


        m_pipeline.rotate(180.0f, 0.0f, 1.0f, 0.0f);

        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        m_character->draw();
    m_pipeline.popMatrix();
}
*/


void EG_ThirdPersonPovCamera::render(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID)
{
    m_pipeline.pushMatrix();

//      these two are equivalent
//      m_pipeline.LoadMatrix(c_worldMatrix);

        m_pipeline.translate(m_characterObject.m_position);
    //    m_pipeline.translate(characterObject.m_scale);
        m_pipeline.loadMatrix(glm::toMat4(m_characterObject.m_rotation));


        m_pipeline.rotate(180.0f, 0.0f, 1.0f, 0.0f);

        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        m_character->draw();
    m_pipeline.popMatrix();
}


void EG_ThirdPersonPovCamera::render1(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID, EG_Model* m)
{

    /*
    m_pipeline.pushMatrix();

        m_pipeline.translate(m_characterObject.m_position);
    //    m_pipeline.translate(characterObject.m_scale);
        m_pipeline.loadMatrix(glm::toMat4(m_characterObject.m_rotation));


        m_pipeline.rotate(180.0f, 0.0f, 1.0f, 0.0f);

        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        // m_character->draw();
        m_characterObject.renderSingle(m_pipeline, Renderer, RenderPassID, m);

    m_pipeline.popMatrix();
*/

    Renderer->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        m_pipeline.translate(m_characterObject.m_position);
        m_pipeline.loadMatrix(glm::toMat4(m_characterObject.m_rotation));
   //     m_pipeline.scale(m_scale);
        m_pipeline.rotate(180.0f, 0.0f, 1.0f, 0.0f);
        m_pipeline.rotate(-90, 1.0, 0.0, 0.0);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        m->render();
    m_pipeline.popMatrix();
    Renderer->disableShader(RenderPassID);
}



void EG_ThirdPersonPovCamera::updateViewMatrix(pipeline& m_pipeline)
{
    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadMatrix(m_viewMatrix);
}

void EG_ThirdPersonPovCamera::updateCharacterObject(float pitchChange, float yawChange, float rollChange)
{
    glm::vec3 oldPosition, positionChange;

    /// extract the axes, compute the local x,y,z axis
    /// http://www.songho.ca/opengl/gl_anglestoaxes.html    for more information on orientation
    /// c_orientation, we treat it as a model matrix, so we extract local axes this way. (different from our camera)
    glm::mat4 m = glm::toMat4(m_characterObject.m_rotation);

    glm::vec3 temp;
    computeLocalAxisAndEyePosFromMatrix(m, MODEL_MATRIX,
                                        c_right, c_up,
                                        c_forward, temp);


    /// update the entity's position
    oldPosition = m_characterObject.getPosition();

    m_characterObject.updatePosition(c_right, c_up, c_forward);


    /// Update the entity's orientation
    /// When moving backwards invert rotations to match direction of travel.
    /// When we drive backwards, our car actually turn leftwards when we steer rightwards
    positionChange = m_characterObject.getPosition() - oldPosition;
    positionChange = glm::normalize(positionChange);
    if (glm::dot(positionChange, c_forward) < 0.0f)
    {
        m_characterObject.setAngularVelocityY(-yawChange);
        yawChange = -yawChange;
    }

    m_characterObject.updateRotation(c_right, c_up, -c_forward);

    m_characterObject.setVelocity(glm::vec3(0.0f));
    m_characterObject.setAngularVelocity(glm::vec3(0.0f));
}




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




