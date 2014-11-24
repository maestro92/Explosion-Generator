
#ifndef THIRD_PERSON_POV_CAMERA_H_
#define THIRD_PERSON_POV_CAMERA_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include "pipeline.h"
#include "define.h"


const float BALL_FORWARD_SPEED = 60.0f;     /// how fast it travels
const float BALL_HEADING_SPEED = 60.0f;     /// how fast the head turns sideways
const float BALL_ROLLING_SPEED = 140.0f;    /// how fast it rolls
const float BALL_RADIUS = 20.0f;


class Third_Person_POV_Camera
{

    private:

        float m_longitudeDegrees;
        float m_latitudeDegrees;

        float m_springConstant;
        float m_dampingConstant;
        float m_offsetDistance;
        float m_headingDegrees;
        float m_pitchDegrees;

        bool mouse_in;
        bool ismoved;

        glm::vec3 eye_point;
        glm::vec3 target;
        glm::vec3 offset;

        glm::vec3 m_xAxis;
        glm::vec3 m_yAxis;
        glm::vec3 m_zAxis;

        glm::mat4 m_viewMatrix;
        glm::vec3 velocity;
        glm::vec3 view_Direction;
        glm::quat orientation;

    public:
        Third_Person_POV_Camera();
        ~Third_Person_POV_Camera();
        void Control(pipeline& m_pipeline, int mid_x, int mid_y);
};


#endif
