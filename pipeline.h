
#ifndef PIPELINE_H_
#define PIPELINE_H_
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
    MODEL_MATRIX = 0,
    VIEW_MATRIX,
    PROJECTION_MATRIX
};


class pipeline{
	int currentMatrix;
	//members
	std::vector<glm::mat4> modelMatrix;
	std::vector<glm::mat4> viewMatrix;
	std::vector<glm::mat4> projectionMatrix;


	bool matricesReady;
	glm::mat4 modelViewMatrix;						//only to save time for vertexshader
	glm::mat4 modelViewProjectionMatrix;	//only to save time for vertexshader
	glm::mat3 normalMatrix;							  //only to save time for vertexshader

    /// for shadow mapping
    glm::mat4 Light_BiasMatrix;
    glm::mat4 Light_ModelMatrix;
    glm::mat4 Light_ViewMatrix;
    glm::mat4 Light_ProjectionMatrix;
    glm::mat4 Light_ModelViewProjectionMatrix;
    glm::mat4 shadowMatrix;


	public:

		pipeline();

		void loadIdentity();
		bool matrixMode(int m);

		//modelview
		void translate(float x,float y,float z);
		void scale(float x,float y,float z);
		void scale(float v);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);


		//projection
		void ortho(float left,float right,float bottom,float top,float near,float far); //==glOrtho
		void perspective(float angle,float aRatio,float near,float far);	//==gluPerspective

		//getters
		glm::mat4 getModelMatrix();
		glm::mat4 getViewMatrix();
		glm::mat4 getModelViewMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getModelViewProjectionMatrix();

		//GLSL
		void updateMatrices(unsigned int programId);
        void updateLightMatrix(glm::mat4 Light_Model, glm::mat4 Light_View, glm::mat4 Light_Projection);
        void updateShadowMatrix(unsigned int shaderId);
        void updateShadowMatrix(unsigned int shaderId, glm::mat4 Light_ViewMatrix, glm::mat4 Light_ProjectionMatrix);

        void pushMatrix();
		void popMatrix();
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
