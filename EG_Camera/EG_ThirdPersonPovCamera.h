#ifndef THIRD_PERSON_POV_CAMERA_H
#define THIRD_PERSON_POV_CAMERA_H

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "EG_Camera.h"
#include "EG_Skybox.h"
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
#include "EG_Renderer.h"
#include "EG_WorldAnimatedObject.h"
#include "EG_Model.h"
#include "EG_ImportedAnimatedModel.h"
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




const float BALL_FORWARD_SPEED = 0.5f;
const float BALL_HEADING_SPEED = 3.0f;
const float BALL_ROLLING_SPEED = 10.0f;
const float MAX_CAMERA_OFFSET = 200.0f;
const float MIN_CAMERA_OFFSET = 20.0f;

class EG_ThirdPersonPovCamera
{
    public:

    enum CameraMode
    {
        FIRST_PERSON_POV_CAMERA_MODE,
        THIRD_PERSON_POV_CAMERA_MODE,
        ORBIT_CAMERA_MODE
    };





    static const float DEFAULT_SPRING_CONSTANT;
    static const float DEFAULT_DAMPING_CONSTANT;

/*
reason why we need to separate the two is because
using spring system, we will have different view matrix
*/
    public:

        EG_WorldAnimatedObject m_characterObject;
        int cameraMode;

        EG_ThirdPersonPovCamera();
        ~EG_ThirdPersonPovCamera();

        void init(pipeline& m_pipeline);
        void initFirst(pipeline& m_pipeline);


        void lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up);

        void mouseIn(bool b);

        void switchCameraMode();

//        /// from GLThirdPersonCamera1
//        void update(pipeline& m_pipeline, float pitchChange, float yawChange);

        /// from GLThirdPersonCamera2
   //     void update2(pipeline& m_pipeline, float pitchChange, float yawChange);
   //     void update2(pipeline& m_pipeline, float elapsedTimeSec, float pitchChange, float yawChange);
        void update(pipeline& m_pipeline, float elapsedTimeSec, float pitchChange, float yawChange, EG_SkyBox& skybox);


        void setTarget(glm::vec3& target);
        void setTarget(glm::vec3 target);

 //       void Control(pipeline& m_pipeline);
        virtual void Control(pipeline& m_pipeline, EG_SkyBox& skybox);
        void updateViewMatrix(pipeline& m_pipeline);

        void enableSpringSystem(bool enableSpringSystem);
        void setSpringConstant(float springConstant);

        void updateCharacterObject(float pitchChange, float yawChange, float rollChange);


        glm::vec3 getEyePoint();
        void render(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID);
        void render1(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID, EG_Model* m);

        void computeLocalAxisAndEyePosFromMatrix( glm::mat4& mat, int mat_type,
                                                  glm::vec3& xAxis, glm::vec3& yAxis,
                                                  glm::vec3& zAxis, glm::vec3& eye_pos);

        float setPitchDegree(float pitch);
        float setYawDegree(float yaw);
        float getPitchDegree();
        float getYawDegree();
        float pitchDegree;
        float yawDegree;

        /// Get methods
        void increaseOffsetDistance();
        void decreaseOffsetDistance();

        float m_pitchDegree;
        float m_yawDegree;

        float getDampingConstant() const;
        float getOffsetDistance() const;
        const glm::quat &getOrientation() const;
        const glm::vec3 &getPosition() const;
        const glm::mat4 &getProjectionMatrix() const;
        float getSpringConstant() const;
        const glm::vec3 &getTargetYAxis() const;
        const glm::vec3 &getViewDirection() const;
        const glm::mat4 &getViewMatrix() const;
        const glm::vec3 &getXAxis() const;
        const glm::vec3 &getYAxis() const;
        const glm::vec3 &getZAxis() const;
        bool springSystemIsEnabled() const;



 //   glm::
        bool mouse_in;
        float mousevel;

   // private:
        glm::vec3 getCharacterPosition();
        void setCharacterPosition(glm::vec3 c_pos);
        void setCharacterPosition(float x, float y, float z);

        EG_ImportedAnimatedModel m_mainAvatar;

//        protected:

        public:
            glm::vec3 m_eye;
            glm::vec3 m_target;
            glm::vec3 m_offset;
            glm::vec3 m_xAxis;
            glm::vec3 m_yAxis;
            glm::vec3 m_zAxis;
            glm::vec3 m_viewDir;

            glm::mat4 m_viewMatrix;
            glm::mat4 m_idealViewMatrix;

        /// everything related to the character
        meshLoader* m_character;


        float m_characterHeight;




        glm::vec3 c_right;
        glm::vec3 c_up;
        glm::vec3 c_forward;


        float m_springConstant;
        float m_dampingConstant;
        bool  m_enableSpringSystem;

        const float CAMERA_ZOOM_DEGREE = 3.0f;
        glm::vec3 m_velocity;
        float m_offsetDistance;

        void setViewMatrixRotation(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);
        void setViewMatrixPosition(glm::vec3 eye_pos);
};

/*
inline float EG_ThirdPersonPovCamera::getDampingConstant() const
{ return m_dampingConstant; }

inline float EG_ThirdPersonPovCamera::getOffsetDistance() const
{ return m_offsetDistance; }

inline const glm::quat &EG_ThirdPersonPovCamera::getOrientation() const
{ return m_orientation; }

inline const glm::vec3 &EG_ThirdPersonPovCamera::getPosition() const
{ return m_eye; }

inline const glm::mat4 &EG_ThirdPersonPovCamera::getProjectionMatrix() const
{ return m_projMatrix; }

inline float EG_ThirdPersonPovCamera::getSpringConstant() const
{ return m_springConstant; }

inline const glm::vec3 &EG_ThirdPersonPovCamera::getTargetYAxis() const
{ return m_targetYAxis; }

inline const glm::vec3 &EG_ThirdPersonPovCamera::getViewDirection() const
{ return m_viewDir; }

inline const glm::mat4 &EG_ThirdPersonPovCamera::getViewMatrix() const
{ return m_viewMatrix; }

inline const glm::vec3 &EG_ThirdPersonPovCamera::getXAxis() const
{ return m_xAxis; }

inline const glm::vec3 &EG_ThirdPersonPovCamera::getYAxis() const
{ return m_yAxis; }

inline const glm::vec3 &EG_ThirdPersonPovCamera::getZAxis() const
{ return m_zAxis; }

inline bool EG_ThirdPersonPovCamera::springSystemIsEnabled() const
{ return m_enableSpringSystem; }
*/

#endif
