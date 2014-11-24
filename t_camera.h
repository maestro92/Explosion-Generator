//http://www.youtube.com/user/thecplusplusguy
#ifndef T_CAMERA_H
#define T_CAMERA_H
#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include "pipeline.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "vector3d.h"
#include "define.h"

class t_camera{
	vector3d loc;

    glm::quat rotation_quaternion;


	float camPitch,camYaw;
	float movevel;
	float mousevel;
	bool mi,ismoved;
	void lockCamera();
	void moveCamera(float dir);
	void moveCameraUp(float dir);
	public:
		t_camera();
		t_camera(vector3d loc);
		t_camera(vector3d loc,float yaw,float pitch);
		t_camera(vector3d loc,float yaw,float pitch,float mv,float mov);


		void Control();
        void Control(int mid_x, int mid_y);
		void Control(pipeline& m_pipeline);
		void Control(pipeline& m_pipeline, int mid_x, int mid_y);
        void Control_MatrixBased(pipeline& m_pipeline, int mid_x, int mid_y);

		void UpdateCamera();
		void UpdateCamera(pipeline& m_pipeline);
		void UpdateCamera_Translation(pipeline& m_pipeline);
        void UpdateCamera_Rotation(pipeline& m_pipeline);


		glm::mat4 UpdateCamera_NoRotate(pipeline& m_pipeline);


        glm::vec3 m_eye;
        glm::vec3 m_xAxis;
        glm::vec3 m_yAxis;
        glm::vec3 m_zAxis;
        glm::vec3 m_viewDir;
        glm::mat4 m_viewMatrix;
        float m_accumPitchDegres;
        void UpdateCamera_MatrixBased(glm::vec3& eye, glm::vec3& target, glm::vec3& up);
        void UpdateCamera_MatrixBased(pipeline& m_pipeline, glm::vec3& eye, glm::vec3& target, glm::vec3& up);

		vector3d getVector();
		vector3d getLocation();
		float getPitch();
		float getYaw();
		float getMovevel();
		float getMousevel();
		bool isMouseIn();

		void setLocation(vector3d vec);
		void lookAt(float pitch,float yaw);
		void mouseIn(bool b);
		void setSpeed(float mv,float mov);

		bool isMoved();
};

#endif
