#ifndef THIRD_PERSON_POV_CAMERA_H
#define THIRD_PERSON_POV_CAMERA_H

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "EG_Camera.h"
#include "sceneLoader.h"
#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include "pipeline.h"
#include "define.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"


/*

// Orbit mode rotates the camera around a target position. This mode can be
// used to simulate a third person camera. Orbit mode supports 2 modes of
// operation: orbiting about the target's Y axis, and free orbiting. The former
// mode only allows pitch and yaw. All yaw changes are relative to the target's
// local Y axis. This means that camera yaw changes won't be affected by any
// rolling. The latter mode allows the camera to freely orbit the target. The
// camera is free to pitch, yaw, and roll. All yaw changes are relative to the
// camera's orientation (in space orbiting the target).

*/




class Third_Person_POV_Camera
{
/*
const float BALL_FORWARD_SPEED = 60.0f;
const float BALL_HEADING_SPEED = 60.0f;
const float BALL_ROLLING_SPEED = 140.0f;
*/
const float BALL_FORWARD_SPEED = 1.0f;
const float BALL_HEADING_SPEED = 2.0f;
const float BALL_ROLLING_SPEED = 10.0f;

    public:

        Third_Person_POV_Camera();
        ~Third_Person_POV_Camera();

        void init(pipeline& m_pipeline);

        void lookAt(glm::vec3& target);
        void lookAt(pipeline& m_pipeline, glm::vec3 target);
        void lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up);

        void mouseIn(bool b);

        void Rotate(pipeline& m_pipeline, float pitch, float yaw);
        void update(pipeline& m_pipeline);





      //  void RotateOrbit(pipeline& m_pipeline);
        void Control(pipeline& m_pipeline);
        void Control(pipeline& m_pipeline, int mid_x, int mid_y);
        void updateViewMatrix(pipeline& m_pipeline);


        void CharacterOrient(float pitch, float yaw, float roll);
        void CharacterRotate(float pitch, float yaw, float roll);
        void CharacterUpdate();
        void CharacterUpdate(float pitch, float yaw, float roll);

        glm::quat eulerToQuaternion(glm::mat4 m, float pitch, float yaw, float roll);


 //   glm::
        bool mouse_in;
        float mousevel;


        glm::vec3 m_eye;
        glm::vec3 m_target;
        glm::vec3 m_offset;

        glm::vec3 m_xAxis;
        glm::vec3 m_yAxis;
        glm::vec3 m_zAxis;
        glm::vec3 m_viewDir;
        glm::mat4 m_viewMatrix;
        glm::quat m_orientation;

        float m_PitchDegrees;
        float m_YawDegrees;

        /// everything related to the character
        meshLoader* m_character;
        glm::vec3 c_Position;
        glm::vec3 c_Velocity;

        glm::vec3 c_eulerOrient;
        glm::vec3 c_eulerRotate;

        glm::vec3 c_right;
        glm::vec3 c_up;
        glm::vec3 c_forward;

        glm::quat c_Orientation;
        glm::quat c_Rotation;

        glm::mat4 c_WorldMatrix;
};



#endif
