#if 1

#include "pipeline.h"
#include "glm/gtx/quaternion.hpp"

pipeline::pipeline()
{
	modelMatrix.push_back(glm::mat4(1.0));
	viewMatrix.push_back(glm::mat4(1.0));
	projectionMatrix.push_back(glm::mat4(1.0));

	modelViewMatrix=glm::mat4(1.0);
	modelViewProjectionMatrix=glm::mat4(1.0);
	normalMatrix=glm::mat3(1.0);
	matricesReady=true;
	currentMatrix=0;

/*
    Light_BiasMatrix[0][0]=0.5;Light_BiasMatrix[0][1]=0.0;Light_BiasMatrix[0][2]=0.0;Light_BiasMatrix[0][3]=0.0;
	Light_BiasMatrix[1][0]=0.0;Light_BiasMatrix[1][1]=0.5;Light_BiasMatrix[1][2]=0.0;Light_BiasMatrix[1][3]=0.0;
	Light_BiasMatrix[2][0]=0.0;Light_BiasMatrix[2][1]=0.0;Light_BiasMatrix[2][2]=0.5;Light_BiasMatrix[2][3]=0.0;
	Light_BiasMatrix[3][0]=0.5;Light_BiasMatrix[3][1]=0.5;Light_BiasMatrix[3][2]=0.5;Light_BiasMatrix[3][3]=1.0;
*/
}

void pipeline::loadIdentity()
{
	if(currentMatrix==MODEL_MATRIX || currentMatrix==VIEW_MATRIX)
	{
		modelMatrix[modelMatrix.size()-1]=glm::mat4(1.0);
		viewMatrix[viewMatrix.size()-1]=glm::mat4(1.0);
	}else
		projectionMatrix[projectionMatrix.size()-1]=glm::mat4(1.0);
	matricesReady=false;
}


void pipeline::reset()
{
    modelMatrix.clear();
	viewMatrix.clear();
	projectionMatrix.clear();

	modelMatrix.push_back(glm::mat4(1.0));
	viewMatrix.push_back(glm::mat4(1.0));
	projectionMatrix.push_back(glm::mat4(1.0));

	modelViewMatrix=glm::mat4(1.0);
	modelViewProjectionMatrix=glm::mat4(1.0);
	normalMatrix=glm::mat3(1.0);
	matricesReady=true;
	currentMatrix=0;
}


bool pipeline::matrixMode(int m)
{
	if(m==MODEL_MATRIX || m==VIEW_MATRIX || m==PROJECTION_MATRIX)
		currentMatrix=m;
}

		//modelview
void pipeline::translate(float x,float y,float z)
{
	if(currentMatrix==MODEL_MATRIX)
		modelMatrix[modelMatrix.size()-1]*=glm::translate(x,y,z);
	else if(currentMatrix==VIEW_MATRIX)
		viewMatrix[viewMatrix.size()-1]*=glm::translate(-x,-y,-z);
	matricesReady=false;
}


void pipeline::translate(glm::vec3 trans_matrix)
{
	if(currentMatrix==MODEL_MATRIX)
		modelMatrix[modelMatrix.size()-1]*=glm::translate(trans_matrix.x,trans_matrix.y,trans_matrix.z);
	else if(currentMatrix==VIEW_MATRIX)
		viewMatrix[viewMatrix.size()-1]*=glm::translate(-trans_matrix.x,-trans_matrix.y,-trans_matrix.z);
	matricesReady=false;
}


void pipeline::scale(float v)
{
    scale(glm::vec3(v,v,v));
}

void pipeline::scale(float x,float y,float z)
{
    scale(glm::vec3(x,y,z));
}

void pipeline::scale(glm::vec3 v)
{
	if(currentMatrix==MODEL_MATRIX)
		modelMatrix[modelMatrix.size()-1]*=glm::scale(v.x, v.y, v.z);
	else if(currentMatrix==VIEW_MATRIX)
		viewMatrix[viewMatrix.size()-1]*=glm::scale(v.x, v.y, v.z);
	matricesReady=false;
}


void pipeline::rotateX(float angle)
{
    rotate(angle, 1.0f, 0.0f, 0.0f);
}

void pipeline::rotateY(float angle)
{
    rotate(angle, 0.0f, 1.0f, 0.0f);
}

void pipeline::rotateZ(float angle)
{
    rotate(angle, 0.0f, 0.0f, 1.0f);
}


void pipeline::rotate(float angle, float x, float y, float z)
{
	if(currentMatrix==MODEL_MATRIX)
		modelMatrix[modelMatrix.size()-1]*=glm::rotate(angle,x,y,z);
	else if(currentMatrix==VIEW_MATRIX)
		viewMatrix[viewMatrix.size()-1]*=glm::rotate(-angle,x,y,z);
	matricesReady=false;
}


void pipeline::rotate(glm::quat q_rotation)
{
    glm::mat4 RotationMatrix = glm::toMat4(q_rotation);

	if(currentMatrix==MODEL_MATRIX)
		modelMatrix[modelMatrix.size()-1]*=RotationMatrix;
	else if(currentMatrix==VIEW_MATRIX)
		viewMatrix[viewMatrix.size()-1]*=RotationMatrix;
	matricesReady=false;
}


void pipeline::loadMatrix(glm::mat4 m)
{
    if(currentMatrix==MODEL_MATRIX)
		modelMatrix[modelMatrix.size()-1]*=m;
	else if(currentMatrix==VIEW_MATRIX)
		viewMatrix[viewMatrix.size()-1]*=m;
	matricesReady=false;
}


//projection
void pipeline::ortho(float left,float right,float bottom,float top,float near,float far) //==glOrtho
{
	projectionMatrix[projectionMatrix.size()-1]=glm::ortho(left,right,bottom,top,near,far);
	matricesReady=false;
}

void pipeline::perspective(float angle,float aRatio,float near,float far)	//==gluPerspective
{
	projectionMatrix[projectionMatrix.size()-1]=glm::perspective(angle,aRatio,near,far);
	matricesReady=false;
}

		//getters
glm::mat4 pipeline::getModelMatrix()
{
	return modelMatrix[modelMatrix.size()-1];
}

glm::mat4 pipeline::getViewMatrix()
{
	return viewMatrix[viewMatrix.size()-1];
}

glm::mat4 pipeline::getModelViewMatrix()
{
	if(!matricesReady)
		return getViewMatrix()*getModelMatrix();
	else
		return modelViewMatrix;
}

glm::mat4 pipeline::getProjectionMatrix()
{
	return projectionMatrix[projectionMatrix.size()-1];
}

glm::mat4 pipeline::getModelViewProjectionMatrix()
{
	if(!matricesReady)
        modelViewProjectionMatrix = getProjectionMatrix()*getViewMatrix()*getModelMatrix();

    return modelViewProjectionMatrix;
}



glm::mat4 pipeline::getModelViewProjectionMatrixForInstancedRendering()
{
	if(!matricesReady)
		modelViewProjectionMatrix = getProjectionMatrix()*getViewMatrix()*getModelMatrix();
    return modelViewProjectionMatrix;
}




void pipeline::pushMatrix()      // glPushMatrix()
{
    glm::mat4 matrix;

    if ( currentMatrix == MODEL_MATRIX )
    {
        if(modelMatrix.size() < 1)
            return;
        matrix = modelMatrix[modelMatrix.size()-1];
        modelMatrix.push_back(matrix);
    }

    else if ( currentMatrix == VIEW_MATRIX )
    {
        if(viewMatrix.size() < 1)
            return;
        matrix = viewMatrix[modelMatrix.size()-1];
        viewMatrix.push_back(matrix);
    }
    else
    {
        if(projectionMatrix.size() < 1)
            return;
        matrix = projectionMatrix[projectionMatrix.size()-1];
        projectionMatrix.push_back(matrix);
    }

}

void pipeline::popMatrix()       // glPopMatrix()
{
    if ( currentMatrix == MODEL_MATRIX )
    {
        if(modelMatrix.size()>1)
            modelMatrix.pop_back();
    }

    else if ( currentMatrix == VIEW_MATRIX)
    {
        if(viewMatrix.size()>1)
            viewMatrix.pop_back();
    }
    else
    {
        if(projectionMatrix.size()>1)
            projectionMatrix.pop_back();
    }
}




//GLSL
void pipeline::updateMatrices(unsigned int programId)
{
	if(!matricesReady)
	{
		modelViewMatrix=viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		modelViewProjectionMatrix=projectionMatrix[projectionMatrix.size()-1]*viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		normalMatrix=glm::mat3(modelViewMatrix);
	}
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelMatrix"),1,GL_FALSE,&modelMatrix[modelMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"viewMatrix"),1,GL_FALSE,&viewMatrix[viewMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelViewMatrix"),1,GL_FALSE,&modelViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelViewProjectionMatrix"),1,GL_FALSE,&modelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"m_ModelviewProjection"),1,GL_FALSE,&modelViewProjectionMatrix[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(programId,"normalMatrix"),1,GL_FALSE,&normalMatrix[0][0]);
}





/*
//GLSL
void pipeline::updateMatrices_TwoPassRayCasting(unsigned int programId)
{
	if(!matricesReady)
	{
		modelViewMatrix=viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		modelViewProjectionMatrix=projectionMatrix[projectionMatrix.size()-1]*viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		normalMatrix=glm::mat3(modelViewMatrix);
	}
	glUniformMatrix4fv(glGetUniformLocation(programId,"m_Modelview"),1,GL_FALSE,&modelMatrix[modelMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"m_ViewMatrix"),1,GL_FALSE,&viewMatrix[viewMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"m_Modelview"),1,GL_FALSE,&modelViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"m_ModelviewProjection"),1,GL_FALSE,&modelViewProjectionMatrix[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(programId,"m_normalMatrix"),1,GL_FALSE,&normalMatrix[0][0]);
}
*/



void pipeline::updateLightMatrix(glm::mat4 lightModel, glm::mat4 lightView, glm::mat4 lightProjection)
{
    m_lightModelMatrix = lightModel;
    m_lightViewMatrix = lightView;
    m_lightProjectionMatrix = lightProjection;

}

//GLSL

void pipeline::updateShadowMatrix()
{
    m_shadowMatrix = LIGHT_BIAS_MATRIX * m_lightProjectionMatrix * m_lightViewMatrix * getModelMatrix();
}


glm::mat4 pipeline::getShadowMatrix()
{
    updateShadowMatrix();
    return m_shadowMatrix;
}

void pipeline::RotateForReflection(int face)
{
    switch (face)
    {
        case POSITIVE_X:
            RotatePositiveX();
            break;

        case NEGATIVE_X:
            RotateNegativeX();
            break;

        case POSITIVE_Y:
            RotatePositiveY();
            break;

        case NEGATIVE_Y:
            RotateNegativeY();
            break;

        case POSITIVE_Z:
            RotatePositiveZ();
            break;

        case NEGATIVE_Z:
            RotateNegativeZ();
            break;
        default:
            break;
    };
}


void pipeline::RotatePositiveX()
{
    rotateZ(180);
    rotateY(-90);
}

void pipeline::RotateNegativeX()
{
    rotateZ(180);
    rotateY(90);
}

void pipeline::RotatePositiveY()
{
    rotateX(90);
}

void pipeline::RotateNegativeY()
{
    rotateX(-90);
}

void pipeline::RotatePositiveZ()
{
    rotateZ(180);
    rotateY(180);
}

void pipeline::RotateNegativeZ()
{
    rotateZ(180);
}



/*
#include "pipeline.h"



pipeline::pipeline()
{
    // set everything to identity
    modelMatrix.push_back(glm::mat4(1.0)); // identity
    viewMatrix.push_back(glm::mat4(1.0));
    projectionMatrix.push_back(glm::mat4(1.0));
    currentMatrix = MODEL_MATRIX;
    matricesReady = true;
    modelViewMatrix = glm::mat4(1.0);
    modelViewProjectionMatrix = glm::mat4(1.0);
    normalMatrix=glm::mat3(1.0);

}

void pipeline::loadIdentity()    // glLoadIdentity()
{
    if(currentMatrix == MODEL_MATRIX || currentMatrix == VIEW_MATRIX)
    {
        modelMatrix[modelMatrix.size() - 1] = glm::mat4(1.0);
        viewMatrix[viewMatrix.size() - 1] = glm::mat4(1.0);
    }
    else
    {
        projectionMatrix[projectionMatrix.size()-1] = glm::mat4(1.0);
    }

    // set it to false, since we have to recalculate the matrix
    matricesReady = false;
}


void pipeline::matrixMode(int i) // glMatrixMode
{

    if ( i == MODEL_MATRIX)
        currentMatrix = MODEL_MATRIX;
    else if ( i == VIEW_MATRIX)
        currentMatrix = VIEW_MATRIX;
    else if ( i == PROJECTION_MATRIX)
        currentMatrix = PROJECTION_MATRIX;


}


// transformation
void pipeline::translate(float x, float y, float z) // == glTranslatef
{

    // combining transformations = multiplying transformation matrix together
    if ( currentMatrix == MODEL_MATRIX )
		modelMatrix[modelMatrix.size()-1]*=glm::translate(x,y,z);
    else if ( currentMatrix == VIEW_MATRIX)
        viewMatrix[modelMatrix.size()-1]*=glm::translate(-x,-y,-z);
}


void pipeline::scale(float x, float y, float z)      // glScalef(x,y,z)
{
    // combining transformations = multiplying transformation matrix together
    if ( currentMatrix == MODEL_MATRIX )
        modelMatrix[modelMatrix.size()-1]*=glm::scale(x,y,z);
    else if ( currentMatrix == VIEW_MATRIX)
        viewMatrix[modelMatrix.size()-1]*=glm::scale(-x,-y,-z);


}


void pipeline::scale(float x)                        // glScalef(x,x,x)
{
    if ( currentMatrix == MODEL_MATRIX )
        modelMatrix[modelMatrix.size()-1]*=glm::scale(x,x,x);
    else if ( currentMatrix == VIEW_MATRIX)
        viewMatrix[modelMatrix.size()-1]*=glm::scale(-x,-x,-x);
}

void pipeline::rotateX(float angle)  //glRotatef(angle, 1, 0, 0)
{

    if ( currentMatrix == MODEL_MATRIX )
        modelMatrix[modelMatrix.size()-1]*=glm::rotate(angle,1.0f,0.0f,0.0f);
    else if ( currentMatrix == VIEW_MATRIX)
        viewMatrix[modelMatrix.size()-1]*=glm::rotate(-angle,1.0f,0.0f,0.0f);

}


void pipeline::rotateY(float angle)  //glRotatef(angle, 0, 1, 0)
{
    if ( currentMatrix == MODEL_MATRIX )
        modelMatrix[modelMatrix.size()-1]*=glm::rotate(angle,0.0f,1.0f,0.0f);
    else if ( currentMatrix == VIEW_MATRIX)
        viewMatrix[modelMatrix.size()-1]*=glm::rotate(-angle,0.0f,1.0f,0.0f);
}

void pipeline::rotateZ(float angle)  //glRotatef(angle, 0, 0, 1)
{
    if ( currentMatrix == MODEL_MATRIX )
        modelMatrix[modelMatrix.size()-1]*=glm::rotate(angle,0.0f,0.0f,1.0f);
    else if ( currentMatrix == VIEW_MATRIX)
        viewMatrix[modelMatrix.size()-1]*=glm::rotate(-angle,0.0f,0.0f,1.0f);
}




// projection
void pipeline::perspective(float angle, float aRatio, float near, float far) // == gluPerspective
{
    projectionMatrix[projectionMatrix.size()-1] = glm::perspective(angle, aRatio, near, far);
}

void pipeline::ortho(float left, float right, float bottom, float top, float near, float far) // this is like defining a box
{

    projectionMatrix[projectionMatrix.size()-1] = glm::ortho(left, right, bottom, top, near, far);
}

void pipeline::pushMatrix()      // glPushMatrix()
{
    glm::mat4 matrix;

    if ( currentMatrix == MODEL_MATRIX )
    {
        matrix = modelMatrix[modelMatrix.size()-1];
        modelMatrix.push_back(matrix);
    }

    else if ( currentMatrix == VIEW_MATRIX)
    {
        matrix = viewMatrix[modelMatrix.size()-1];
        viewMatrix.push_back(matrix);
    }
    else
    {
        matrix = projectionMatrix[projectionMatrix.size()-1];
        projectionMatrix.push_back(matrix);
    }

}

void pipeline::popMatrix()       // glPopMatrix()
{
    if ( currentMatrix == MODEL_MATRIX )
    {
        if(modelMatrix.size()>1)
            modelMatrix.pop_back();
    }

    else if ( currentMatrix == VIEW_MATRIX)
    {
        if(viewMatrix.size()>1)
            viewMatrix.pop_back();
    }
    else
    {
        if(projectionMatrix.size()>1)
            projectionMatrix.pop_back();
    }
}


glm::mat4 pipeline::getModelMatrix()
{
	return modelMatrix[modelMatrix.size()-1];
}

glm::mat4 pipeline::getModelViewMatrix()
{
    if(!matricesReady)
        return viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
    else
        return modelViewMatrix;
}


glm::mat4 pipeline::getModelViewProjectionMatrix()
{
	if(!matricesReady)
		return viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1]*projectionMatrix[projectionMatrix.size()-1];
	else
		return modelViewProjectionMatrix;
}

glm::mat4 pipeline::getProjectionMatrix()
{
	return projectionMatrix[projectionMatrix.size()-1];
}

glm::mat4 pipeline::getNormalMatrix()
{
	return projectionMatrix[projectionMatrix.size()-1];
}

void pipeline::updateMatrices(unsigned int programId)
{
    if(!matricesReady)
	{
		modelViewMatrix=viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		modelViewProjectionMatrix=projectionMatrix[projectionMatrix.size()-1]*viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		normalMatrix=glm::mat3(modelViewMatrix);
	}
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelMatrix"),1,GL_FALSE,&modelMatrix[modelMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"viewMatrix"),1,GL_FALSE,&viewMatrix[viewMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelViewMatrix"),1,GL_FALSE,&modelViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelViewProjectionMatrix"),1,GL_FALSE,&modelViewProjectionMatrix[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(programId,"normalMatrix"),1,GL_FALSE,&normalMatrix[0][0]);



}
*/

#endif


#if 0
#include "pipeline.h"


pipeline::pipeline()
{
    modelMatrix.push_back(glm::mat4(1.0));
    viewMatrix.push_back(glm::mat4(1.0));
    projectionMatrix.push_back(glm::mat4(1.0));

    modelViewMatrix = glm::mat4(1.0);
    modelViewProjectionMatrix = glm::mat4(1.0);
    normalMatrix = glm::mat3(1.0);
    matricesReady = true;
    currentMatrixMode = MODEL;
}

void pipeline::loadIdentity() // currentMatrix = glLoadIdentity()
{
    if(currentMatrixMode == MODEL || currentMatrixMode == VIEW)
    {
        modelMatrix[modelMatrix.size()-1] = glm::mat4(1.0);
        viewMatrix[viewMatrix.size()-1] = glm::mat4(1.0);
    }
    else
    {
        projectionMatrix[projectionMatrix.size()-1] = glm::mat4(1.0);
    }
    matricesReady = false;
}


void pipeline::matrixMode(int i) //glMatrixMode
{
    if(i == MODEL)
        currentMatrixMode = MODEL;
    else if (i == VIEW)
        currentMatrixMode = VIEW;
    else if (i == PROJECTION)
        currentMatrixMode = PROJECTION;
}


//transformation
void pipeline::translate(float x,float y,float z)
{
	if(currentMatrixMode == MODEL)
		modelMatrix[modelMatrix.size()-1]*=glm::translate(x,y,z);
	else if(currentMatrixMode == VIEW)
		viewMatrix[viewMatrix.size()-1]*=glm::translate(-x,-y,-z);
	matricesReady=false;
}

void pipeline::scale(float x, float y, float z)
{
    if(currentMatrixMode == MODEL)
        modelMatrix[modelMatrix.size()-1]*= glm::translate(x,y,z);
    else if (currentMatrixMode == VIEW)
        viewMatrix[viewMatrix.size()-1]*= glm::translate(-x,-y,-z);
    matricesReady = false;
}

void pipeline::scale(float x)
{
    if(currentMatrixMode == MODEL)
        modelMatrix[modelMatrix.size()-1]*= glm::translate(x,x,x);
    else if (currentMatrixMode == VIEW)
        viewMatrix[viewMatrix.size()-1]*= glm::translate(-x,-x,-x);
    matricesReady = false;
}

void pipeline::rotateX(float angle)
{
    if(currentMatrixMode == MODEL)
        modelMatrix[modelMatrix.size()-1]*= glm::rotate(angle, 1.0f, 0.0f, 0.0f);
    else if (currentMatrixMode == VIEW)
        viewMatrix[viewMatrix.size()-1]*= glm::rotate(angle, 1.0f, 0.0f, 0.0f);
    matricesReady = false;
}

void pipeline::rotateY(float angle)
{
    if(currentMatrixMode == MODEL)
        modelMatrix[modelMatrix.size()-1]*= glm::rotate(angle, 0.0f, 1.0f, 0.0f);
    else if (currentMatrixMode == VIEW)
        viewMatrix[viewMatrix.size()-1]*= glm::rotate(angle, 0.0f, 1.0f, 0.0f);
    matricesReady = false;
}

void pipeline::rotateZ(float angle)
{
    if(currentMatrixMode == MODEL)
        modelMatrix[modelMatrix.size()-1]*= glm::rotate(angle, 0.0f, 0.0f, 1.0f);
    else if (currentMatrixMode == VIEW)
        viewMatrix[viewMatrix.size()-1]*= glm::rotate(angle, 0.0f, 0.0f, 1.0f);
    matricesReady = false;
}



// projection
void pipeline::perspective(float angle, float aRatio, float near, float far)
{
    projectionMatrix[projectionMatrix.size()-1] = glm::perspective(angle, aRatio, near, far);
}

void pipeline::ortho(float left, float right, float bottom, float top, float near, float far)  // parallel projection, like a box
{
    projectionMatrix[projectionMatrix.size()-1] = glm::ortho(left, right, bottom, top, near, far);
}


void pipeline::pushMatrix()
{
    glm::mat4 matrix;
    if(currentMatrixMode == MODEL)
    {
        matrix = modelMatrix[modelMatrix.size()-1];
        modelMatrix.push_back(matrix);
    }
    else if (currentMatrixMode == VIEW)
    {
        matrix = viewMatrix[viewMatrix.size()-1];
        viewMatrix.push_back(matrix);
    }
    else if (currentMatrixMode == PROJECTION)
    {
        matrix = projectionMatrix[projectionMatrix.size()-1];
        projectionMatrix.push_back(matrix);
    }
}

void pipeline::popMatrix()
{
    if(currentMatrixMode == MODEL)
    {
        if(modelMatrix.size()>1);
            modelMatrix.pop_back();
    }
    else if (currentMatrixMode == VIEW)
    {
        if(viewMatrix.size()>1);
            viewMatrix.pop_back();
    }
    else if (currentMatrixMode == PROJECTION)
    {
        if(projectionMatrix.size()>1);
            projectionMatrix.pop_back();
    }
}

glm::mat4& pipeline::getModelMatrix()
{
    return modelMatrix[modelMatrix.size()-1];
}

glm::mat4& pipeline::getModelViewMatrix()
{
    if(!matricesReady)
        modelViewMatrix = viewMatrix[viewMatrix.size()-1] *
                            modelMatrix[modelMatrix.size()-1];
    return modelViewMatrix;
}

glm::mat4& pipeline::getModelViewProjectionMatrix()
{
    if(!matricesReady)
    {
        modelViewProjectionMatrix = projectionMatrix[projectionMatrix.size()-1] *
                                            viewMatrix[viewMatrix.size()-1] *
                                            modelMatrix[modelMatrix.size()-1];
    }
    return modelViewProjectionMatrix;
}

glm::mat4& pipeline::getProjectionMatrix()
{
    return projectionMatrix[projectionMatrix.size()-1];
}


/*
glm::mat4& pipeline::getNormalMatrix()
{
    if(!matricesReady)
    {
        normalMatrix = glm::mat3(getModelViewMatrix());
    }
    return normalMatrix;
}
*/
// will upload the matrices to the GLSL shaders
void pipeline::updateMatrices(unsigned int programId)
{
    if(!matricesReady)
    {
		modelViewMatrix=viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		modelViewProjectionMatrix=projectionMatrix[projectionMatrix.size()-1]*viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		normalMatrix=glm::mat3(modelViewMatrix);
    }
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelMatrix"),1,GL_FALSE,&modelMatrix[modelMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"viewMatrix"),1,GL_FALSE,&viewMatrix[viewMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelViewMatrix"),1,GL_FALSE,&modelViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelViewProjectionMatrix"),1,GL_FALSE,&modelViewProjectionMatrix[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(programId,"normalMatrix"),1,GL_FALSE,&normalMatrix[0][0]);
}

#endif
