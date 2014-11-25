#ifndef FIRST_PERSON_POV_QUATERNION_CAMERA_H
#define FIRST_PERSON_POV_QUATERNION_CAMERA_H

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "EG_Camera.h"

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




class FirstPersonPOV_QuaternionCamera
{



    public:

        FirstPersonPOV_QuaternionCamera();
        ~FirstPersonPOV_QuaternionCamera();

        void init(pipeline& m_pipeline);


        void lookAt(pipeline& m_pipeline, glm::vec3 target);
        void lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up);

        void Move(pipeline& m_pipeline, float dx, float dy, float dz);
        void Move(pipeline& m_pipeline, const glm::vec3& direction, glm::vec3& amount);

        void mouseIn(bool b);

        void Rotate(pipeline& m_pipeline);
        void Control(pipeline& m_pipeline, int mid_x, int mid_y, glm::vec3& direction);
        void updateViewMatrix(pipeline& m_pipeline);
 //   glm::
        bool mouse_in;
        float mousevel;


        glm::vec3 m_eye;
        glm::vec3 m_xAxis;
        glm::vec3 m_yAxis;
        glm::vec3 m_zAxis;
        glm::vec3 m_viewDir;
        glm::quat m_orientation;
        glm::mat4 m_viewMatrix;


        float m_mouseRotationSpeed;
        float m_PitchDegrees;
        float m_YawDegrees;
        float m_orbitMinZoom;
        float m_orbitMaxZoom;
        float m_orbitOffsetDistance;




        glm::mat4 skybox_m_viewMatrix;
        glm::quat skybox_m_orientation;
        glm::quat m_savedOrientation;
        glm::quat ROT;
        glm::mat4 ROT_matrix;
};



#endif

