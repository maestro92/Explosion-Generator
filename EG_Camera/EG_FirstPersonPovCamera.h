//http://www.youtube.com/user/thecplusplusguy
#ifndef T_CAMERA_H
#define T_CAMERA_H
#include <cmath>
#include <iostream>
#include "EG_Utility.h"

#include <SDL/SDL.h>
#include "pipeline.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "define.h"




const glm::vec3 WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const glm::vec3 WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_ZAXIS(0.0f, 0.0f, 1.0f);


class EG_FirstPersonPovCamera{
//	glm::vec3 loc;
    private:
    glm::vec3 m_eye;
    glm::quat rotation_quaternion;


    float m_pitchDegrees;
    float m_yawDegrees;

//	float camPitch,camYaw;
//	float movevel;
//	float mousevel;
//	bool mi,

	bool ismoved;
	bool m_mouseIn;

	void lockCamera();
	void moveCamera(float dir);
	void moveCameraUp(float dir);
	public:
		EG_FirstPersonPovCamera();
		EG_FirstPersonPovCamera(glm::vec3 eye);
		EG_FirstPersonPovCamera(glm::vec3 eye, float yaw,float pitch);
		EG_FirstPersonPovCamera(glm::vec3 eye, float yaw,float pitch,float mv,float mov);


        void move(float dx, float dy, float dz);
        void move(const glm::vec3& direction, const glm::vec3& amount);


        void Control(int mid_x, int mid_y);

		void UpdateCamera();
		void UpdateCamera(pipeline& m_pipeline);
		void UpdateCameraTranslation(pipeline& m_pipeline);
        void UpdateCameraRotation(pipeline& m_pipeline);


//		glm::mat4 UpdateCamera_NoRotate(pipeline& m_pipeline);

        void updateViewMatrix(pipeline& m_pipeline);

        void lookAt(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up);
        glm::vec3 m_xAxis;
        glm::vec3 m_yAxis;
        glm::vec3 m_zAxis;
        glm::vec3 m_viewDir;
        glm::mat4 m_viewMatrix;
        float m_accumPitchDegres;
//        void UpdateCamera_MatrixBased(glm::vec3& eye, glm::vec3& target, glm::vec3& up);
//        void UpdateCamera_MatrixBased(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up);

/*
		glm::vec3 getVector();

*/
		float getMovevel();
		float getMousevel();
		bool isMouseIn();

		float getPitch();
		float getYaw();
        void setPitch(float pitch);
        void setYaw(float yaw);


		void setEyePoint(glm::vec3 eye);
        void setEyePoint(float x, float y, float z);
		glm::vec3 getEyePoint();

		void lookAt(float pitch,float yaw);

//		void mouseIn(bool b);
		void setMouseInFlag(bool f);

		void setSpeed(float mv,float mov);

		bool isMoved();
};

#endif
