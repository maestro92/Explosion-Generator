#ifndef EG_CAMERA_H
#define EG_CAMERA_H

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include "pipeline.h"
#include "define.h"
#include "EG_WorldObject.h"
#include "EG_Utility.h"
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

const float CAMERA_FOVX = 90.0f;
const float CAMERA_ZFAR = 100.0f;
const float CAMERA_ZNEAR = 0.1f;
const float CAMERA_ZOOM_MAX = 5.0f;
const float CAMERA_ZOOM_MIN = 1.5f;


const float DEFAULT_MOUSE_ROTATION_SPEED = 0.3f;
const float DEFAULT_ZNEAR = 0.1f;
const float DEFAULT_ZFAR = 1000.0f;
const float DEFAULT_ORBIT_MIN_ZOOM = DEFAULT_ZNEAR + 1.0f;
const float DEFAULT_ORBIT_MAX_ZOOM = DEFAULT_ZFAR * 0.5f;

const float DEFAULT_ORBIT_OFFSET_DISTANCE = DEFAULT_ORBIT_MIN_ZOOM + (DEFAULT_ORBIT_MAX_ZOOM - DEFAULT_ORBIT_MIN_ZOOM) * 0.25f;

/*
const glm::vec3 WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const glm::vec3 WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_ZAXIS(0.0f, 0.0f, 1.0f);
*/

class EG_Camera
{
    enum CAMERA_MODE
    {
        FIRST_PERSON_POV_CAMERA_MODE,
        SPECTATOR_CAMERA_MODE,
        FLIGHT_CAMERA_MODE,
        ORBIT_CAMERA_MODE,
        THIRD_PERSON_POV_CAMERA_MODE
    };

    public:

        EG_Camera();
        ~EG_Camera();

        WorldObject m_cameraObj;

        void init();

    //    void lookAt(glm::vec3& target);
        void lookAt(pipeline& m_pipeline);
        void lookAt(pipeline& m_pipeline, glm::vec3 target);
        void lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up);

        void mouseIn(bool b);

        void Control(pipeline& m_pipeline);
        void Control(pipeline& m_pipeline, int mid_x, int mid_y);

        void move(float dx, float dy, float dz);
        void move(glm::vec3& direction, glm::vec3 amount);

        void rotate(float yawDegree, float pitchDegree, float rollDegree);
        void rotateFirstPersonPOV(float yawDegree, float pitchDegree);



        void RotateOrbit(pipeline& m_pipeline);
        void updateViewMatrix(pipeline& m_pipeline);
 //   glm::
        bool mouse_in;
        float mousevel;


        int m_cameraMode;
        bool m_mouseIn;


        glm::vec3 m_eye;
        glm::vec3 m_target;
        glm::vec3 m_targetYAxis;
        glm::vec3 m_xAxis;
        glm::vec3 m_yAxis;
        glm::vec3 m_zAxis;
        glm::vec3 m_viewDir;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_RotateViewMatrix;
        pipeline m_skyboxRotate;



  //      glm::vec3 newEye;
  //      glm::vec3 newTarget;

        glm::vec3 newEye;
        glm::vec3 newTarget;

        float m_mouseRotationSpeed;


        float m_pitchDegrees;
        float m_yawDegrees;


        float m_PitchDegrees;
        float m_YawDegrees;
        float m_orbitMinZoom;
        float m_orbitMaxZoom;
        float m_orbitOffsetDistance;

        glm::quat m_orientation;


        glm::mat4 skybox_m_viewMatrix;
        glm::quat skybox_m_orientation;
        glm::quat m_savedOrientation;
        glm::quat ROT;
        glm::mat4 ROT_matrix;
};



#endif
