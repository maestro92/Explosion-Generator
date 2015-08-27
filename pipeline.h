
#ifndef PIPELINE_H_
#define PIPELINE_H_
#include <iostream>
#include <vector>

#include "define.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#define NO_SDL_GLEXT
#include <GL/glew.h>

using namespace std;


enum matrixModes
{
    MODEL_MATRIX = 0,
    VIEW_MATRIX,
    PROJECTION_MATRIX
};

const static glm::mat4 LIGHT_BIAS_MATRIX = glm::mat4( 0.5, 0.0, 0.0, 0.0,
                                                     0.0, 0.5, 0.0, 0.0,
                                                     0.0, 0.0, 0.5, 0.0,
                                                     0.5, 0.5, 0.5, 1.0);
/*
http://www.geeksforgeeks.org/assignment-operator-overloading-in-c/

if a class doesn't contain pointers, then there is no need to write assignment operator and copy constructor
*/
class pipeline
{



    public:
        int currentMatrix;
        //members
//        std::vector<glm::mat4> m_modelMatrices;
//        std::vector<glm::mat4> m_viewMatrices;
//        std::vector<glm::mat4> m_projectionMatrices;


        std::vector<glm::mat4> modelMatrix;
        std::vector<glm::mat4> viewMatrix;
        std::vector<glm::mat4> projectionMatrix;

        bool matricesReady;
        glm::mat4 modelViewMatrix;						//only to save time for vertexshader
        glm::mat4 modelViewProjectionMatrix;	//only to save time for vertexshader
        glm::mat3 normalMatrix;							  //only to save time for vertexshader

        /// for shadow mapping
        glm::mat4 m_lightModelMatrix;
        glm::mat4 m_lightViewMatrix;
        glm::mat4 m_lightProjectionMatrix;



	public:
		pipeline();

		void loadIdentity();
		bool matrixMode(int m);

		//modelview
		void translate(float x,float y,float z);
        void translate(glm::vec3 trans_matrix);
	//	glm::mat4 translateNoRotate(float x,float y,float z);
		void scale(float x,float y,float z);
		void scale(float v);
		void scale(glm::vec3 v);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
        void rotate(float angle, float x, float y, float z);
        void rotate(glm::quat q_rotation);
        void reset();

        void loadMatrix(glm::mat4 m);

		//projection
		void ortho(float left,float right,float bottom,float top,float near,float far); //==glOrtho
		void perspective(float angle,float aRatio,float near,float far);	//==gluPerspective

		//getters
		glm::mat4 getModelMatrix();
		glm::mat4 getViewMatrix();
		glm::mat4 getModelViewMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getModelViewProjectionMatrix();
        glm::mat4 getModelViewProjectionMatrixForInstancedRendering();

		//GLSL
		void updateMatrices(unsigned int programId);
//		void updateMatrices_TwoPassRayCasting(unsigned int programId);

        void updateLightMatrix(glm::mat4 lightModel, glm::mat4 lightView, glm::mat4 lightProjection);
        void updateShadowMatrix();
        glm::mat4 getShadowMatrix();

        void pushMatrix();
		void popMatrix();


        enum CubeMapOrientation
        {
            POSITIVE_X=0, NEGATIVE_X, POSITIVE_Y,
            NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z
        };

        void RotateForReflection(int face);
		void RotatePositiveX();
		void RotateNegativeX();

		void RotatePositiveY();
		void RotateNegativeY();

		void RotatePositiveZ();
		void RotateNegativeZ();


        glm::mat4 m_shadowMatrix;
};

#endif


#if 0
#ifndef PIPELINE_H
#define PIPELINE_H

#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#define NO_SDL_GLEXT
#include <GL/glew.h>

using namespace std;


enum matrixModes
{
    MODEL = 0,
    VIEW,
    PROJECTION
};

class pipeline
{
    vector<glm::mat4> modelMatrix; // object
    vector<glm::mat4> viewMatrix;  // camera/viewer
    //modelViewMatrix = viewMatrix * modelMatrix

    vector<glm::mat4> projectionMatrix;
    //modelViewProjectionMatrix = ProjectionMatrix * viewMatrix * modelMatrix

    //glMatrixMode
    int currentMatrixMode;

    bool matricesReady; // if the following member variables are ready, we can then use
    glm::mat4 modelViewMatrix;
    glm::mat4 modelViewProjectionMatrix;
    glm::mat3 normalMatrix; // the matrix you multiply to keep the normal vector. (Condition, dot(normal and vertex) = 0, so when
                                                                                    // vertex * ModelViewMatrix, normal needs a normalMatrix to keep the dot product 0
    public:
        pipeline();

        void loadIdentity(); // currentMatrix = glLoadIdentity()
        void matrixMode(int i); //glMatrixMode

        //transformation
        void translate(float x, float y, float z);
        void scale(float x, float y, float z);
        void scale(float x);
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);

        // projection
        void perspective(float angle, float aRatio, float near, float far);
        void ortho(float left, float right, float bottom, float top, float near, float far);  // parallel projection, like a box

        void pushMatrix();
        void popMatrix();

        glm::mat4& getModelMatrix();
        glm::mat4& getModelViewMatrix();
        glm::mat4& getModelViewProjectionMatrix();
        glm::mat4& getProjectionMatrix();
    //    glm::mat4& getNormalMatrix();

        // will upload the matrices to the GLSL shaders
        void updateMatrices(unsigned int programId);

};



#endif // PIPELINE_H
#endif
