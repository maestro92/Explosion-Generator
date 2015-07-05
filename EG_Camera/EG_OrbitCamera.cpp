
#include "EG_OrbitCamera.h"


EG_OrbitCamera::EG_OrbitCamera()
{
    m_pivotOffset = glm::vec3(0.0f, 3.0f, 0.0f);
    m_leftMouseDown = false;
    f = true;
}

EG_OrbitCamera::~EG_OrbitCamera()
{

}


float EG_OrbitCamera::maxf(float f1, float f2)
{
    if(f1>f2)
        return f1;
    else
        return f2;
}


float EG_OrbitCamera::minf(float f1, float f2)
{
    if(f1<f2)
        return f1;
    else
        return f2;
}


void EG_OrbitCamera::Control(pipeline& m_pipeline, EG_SkyBox& skybox)
{
    float pitchChange = 0.0f;
    float yawChange = 0.0f;
    float forwardSpeed = 0.0f;

    int tmpx,tmpy;
	SDL_GetMouseState(&tmpx,&tmpy);

    m_mouseCurrent = glm::vec2( tmpx, tmpy );

    m_mouseDelta = ( m_mousePrevious - m_mouseCurrent );
    m_enableSpringSystem = false;



    if(tmpx < 200)
    {
        m_mouseDelta = glm::vec2(0,0);
    }

//	if(mouse_in)
	if(m_leftMouseDown)
	{
		Uint8* state=SDL_GetKeyState(NULL);
        pitchChange = m_mouseDelta.y;
        yawChange = m_mouseDelta.x;

  //      pitchChange=1;
  //      yawChange = 1;
//
/*
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
*/



    }



 //   if(f)
 //       yawChange = 30;
//    f = false;
   // cout << "m_yawDegree is " << m_yawDegree<< endl;

   // cout << "m_pitchChange is " << pitchChange<< endl;

/*
    if( m_pitchDegree < 85)
    {
        m_pitchDegree += pitchChange;
    }
    else
        pitchChange = minf(-5, pitchChange);


    if (m_pitchDegree > -85)
    {
        m_pitchDegree += pitchChange;
    }
    else
        pitchChange = maxf(5, pitchChange);
*/
/*
    cout << "cur m_pitchDegree is " << m_pitchDegree<< endl;

	if(m_pitchDegree>=50)
    {
		m_pitchDegree=50;
        pitchChange = -1;
    }
	if(m_pitchDegree<=-60)
    {
        m_pitchDegree=-60;
        pitchChange = 1;
    }

    m_pitchDegree += pitchChange;
    cout << "m_pitchChange is " << pitchChange<< endl;
 //   else
   //     pitchChange = 0;
*/



    m_pitchDegree = asinf(m_viewMatrix[1][2]) * EG_Utility::RADIAN_TO_DEGREE;
 //   cout << "Here m_pitchDegree is " << m_pitchDegree << endl;

    if( (m_pitchDegree + pitchChange) > 89 || (m_pitchDegree + pitchChange) < -89 )
        pitchChange = 0;


    m_yawDegree += yawChange;
    if(m_yawDegree > 360)
        m_yawDegree -= 360;

    if(m_yawDegree < -360)
        m_yawDegree += 360;

  //  EG_Utility::printGlm("m_pivotOffset", m_pivotOffset);
//    update(m_pipeline, 0.031f, pitchChange, (forwardSpeed >= 0.0f) ? yawChange : -yawChange, skybox);
    update(m_pipeline, 0.031f, pitchChange, -yawChange, skybox);
    m_mousePrevious = m_mouseCurrent;
}




void EG_OrbitCamera::Control(pipeline& m_pipeline, EG_SkyBox& skybox, bool GuiFlag)
{
    float pitchChange = 0.0f;
    float yawChange = 0.0f;
    float forwardSpeed = 0.0f;

    int tmpx,tmpy;
	SDL_GetMouseState(&tmpx,&tmpy);

    m_mouseCurrent = glm::vec2( tmpx, tmpy );

    m_mouseDelta = ( m_mousePrevious - m_mouseCurrent );
    m_enableSpringSystem = false;

/*
    if(tmpx < palette_w)
    {
        m_mouseDelta = glm::vec2(0,0);
    }
*/

    if(GuiFlag)
        m_mouseDelta = glm::vec2(0,0);

//	if(mouse_in)
	if(m_leftMouseDown)
	{
		Uint8* state=SDL_GetKeyState(NULL);
        pitchChange = m_mouseDelta.y;
        yawChange = m_mouseDelta.x;
    }


    m_pitchDegree = asinf(m_viewMatrix[1][2]) * EG_Utility::RADIAN_TO_DEGREE;
 //   cout << "Here m_pitchDegree is " << m_pitchDegree << endl;

    if( (m_pitchDegree + pitchChange) > 89 || (m_pitchDegree + pitchChange) < -89 )
        pitchChange = 0;


    m_yawDegree += yawChange;
    if(m_yawDegree > 360)
        m_yawDegree -= 360;

    if(m_yawDegree < -360)
        m_yawDegree += 360;

    update(m_pipeline, 0.031f, pitchChange, -yawChange, skybox);
    m_mousePrevious = m_mouseCurrent;
}



void EG_OrbitCamera::Control(pipeline& m_pipeline, EG_SkyBox& skybox, int screen_w, int screen_h, int palette_w, int palette_h)
{

}

void EG_OrbitCamera::update(pipeline& m_pipeline, float elapsedTimeSec, float pitchChange, float yawChange, EG_SkyBox& skybox)
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

   //     cout << "#################### Start ##########################" << endl;
        m_pitchDegree = asinf(m_viewMatrix[1][2]) * 180 / M_PI;
     //   cout << "Here m_pitchDegree is " << m_pitchDegree << endl;

        if( m_pitchDegree > 89 && m_pitchDegree < 90)
            int a = 1;

        if (pitchChange != 0.0f)
        {
            glm::mat4 rot_temp = glm::rotate(pitchChange, 1.0f,0.0f,0.0f);
            m_viewMatrix = rot_temp * m_viewMatrix;
        }

 //       EG_Utility::printGlm("m_viewMatrix", m_viewMatrix);


        /// extracting x,y,z axis from viewMatrix
        computeLocalAxisAndEyePosFromMatrix(m_viewMatrix, VIEW_MATRIX,
                                              m_xAxis, m_yAxis,
                                              m_zAxis, m_eye);
/*
        EG_Utility::printGlm("m_xAxis", m_xAxis);
        EG_Utility::printGlm("m_yAxis", m_yAxis);
        EG_Utility::printGlm("m_zAxis", m_zAxis);
*/
        /// if not using the spring system
        m_eye = m_target + m_zAxis * m_offsetDistance;
   //     debugVec3(m_eye, "m_eye");
    }


    m_zAxis = m_eye - m_target;
 //   cout << "target is " << endl;
//    EG_Utility::printGlm("m_target", m_target);
    m_zAxis = glm::normalize(m_zAxis);

 //   EG_Utility::printGlm("m_zAxis", m_zAxis);

    m_xAxis = glm::cross(glm::vec3(0.0f,1.0f,0.0f), m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);
 //   EG_Utility::printGlm("m_xAxis", m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);
 //   EG_Utility::printGlm("m_yAxis", m_yAxis);

    setViewMatrixRotation(m_xAxis, m_yAxis, m_zAxis);
 //   EG_Utility::printGlm("ViewMatrix", m_viewMatrix);

    m_pipeline.pushMatrix();
        updateViewMatrix(m_pipeline);
        skybox.UpdateRotationOnly_View_Pipeline(m_pipeline);
    m_pipeline.popMatrix();

    setViewMatrixPosition(m_eye);
 //   EG_Utility::printGlmMat("ViewMatrix", m_viewMatrix);

   // m_viewMatrix = m_viewMatrix * glm::translate(0.0f, -m_characterHeight, 0.0f);
    m_viewMatrix = m_viewMatrix * glm::translate(-m_pivotOffset.x, -m_pivotOffset.y, -m_pivotOffset.z);



    m_viewDir = -m_zAxis;
	updateViewMatrix(m_pipeline);
  //  m_eye += m_characterHeight;
}
