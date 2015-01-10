
#include "EG_RenderTechnique.h"


EG_RenderTechnique::EG_RenderTechnique()
{

}

EG_RenderTechnique::~EG_RenderTechnique()
{

}


void EG_RenderTechnique::init()
{

}


void EG_RenderTechnique::allocateMemberVariables(int numOfShaders)
{
    numOfShaders_ = numOfShaders;
    Matrices_UniLoc = new Matrices_Location[numOfShaders_];
    progShaders = new Shader*[numOfShaders_];
}


void EG_RenderTechnique::initMemberVariables()
{
    for(int i=0; i<numOfShaders_; i++)
        Init_Shader_GL_Location(progShaders[i], Matrices_UniLoc[i]);

}


GLuint EG_RenderTechnique::GetUniformLocation(Shader* s, const char* UniformName)
{
    unsigned int shaderID = s->getProgramId();

    GLuint Location = glGetUniformLocation(shaderID, UniformName);

    if(Location == -1)
    {
        cout << "Error in Init_Shader" << endl;
    }
    return Location;
}


// assign GL location
bool EG_RenderTechnique::Init_Shader_GL_Location(Shader* s, Matrices_Location& Mat)
{
    unsigned int shaderID = s->getProgramId();

    Mat.ModelviewProjection = glGetUniformLocation( shaderID, "m_ModelviewProjection");
    Mat.ModelviewMatrix = glGetUniformLocation( shaderID, "m_Modelview");
    Mat.ViewMatrix = glGetUniformLocation( shaderID, "m_ViewMatrix");
    Mat.ProjectionMatrix = glGetUniformLocation( shaderID, "m_ProjectionMatrix");
    Mat.ModelMatrix = glGetUniformLocation( shaderID, "m_ModelMatrix");
    Mat.ViewNoRotateMatrix = glGetUniformLocation( shaderID, "m_ViewNoRotateMatrix");
    Mat.NormalMatrix = glGetUniformLocation( shaderID, "m_normalMatrix");

    if(Mat.ModelviewProjection == -1 ||
       Mat.ModelviewMatrix == -1 ||
       Mat.ViewMatrix == -1 ||
       Mat.ProjectionMatrix == -1 ||
       Mat.ModelMatrix == -1 ||
       Mat.ViewNoRotateMatrix == -1 ||
       Mat.NormalMatrix)
    {
        cout << "Error in Init_Shader" << endl;
        return false;
    }
    return true;
}



void EG_RenderTechnique::loadUniformLocations(pipeline& p, int RenderPassID)
{
	if(!p.matricesReady)
	{
		p.modelViewMatrix=p.viewMatrix[p.viewMatrix.size()-1]*p.modelMatrix[p.modelMatrix.size()-1];
		p.modelViewProjectionMatrix=p.projectionMatrix[p.projectionMatrix.size()-1]*
                                    p.viewMatrix[p.viewMatrix.size()-1]*
                                    p.modelMatrix[p.modelMatrix.size()-1];
		p.normalMatrix=glm::mat3(p.modelViewMatrix);
	}

	glUniformMatrix4fv(Matrices_UniLoc[RenderPassID].ModelMatrix,1,GL_FALSE,&p.modelMatrix[p.modelMatrix.size()-1][0][0]);
    glUniformMatrix4fv(Matrices_UniLoc[RenderPassID].ViewMatrix,1,GL_FALSE,&p.viewMatrix[p.viewMatrix.size()-1][0][0]);
	glUniformMatrix4fv(Matrices_UniLoc[RenderPassID].ModelviewMatrix,1,GL_FALSE,&p.modelViewMatrix[0][0]);
	glUniformMatrix4fv(Matrices_UniLoc[RenderPassID].ModelviewProjection,1,GL_FALSE,&p.modelViewProjectionMatrix[0][0]);
	glUniformMatrix3fv(Matrices_UniLoc[RenderPassID].NormalMatrix,1,GL_FALSE,&p.normalMatrix[0][0]);
}


/*
void EG_RenderTechnique::Setup_ShadowMatrix_forRender(pipeline& p, int RenderPassID)
{

}


void EG_RenderTechnique::Setup_Special_glUniform(pipeline& p, int RenderPassID)
{

}
*/
/*
void EG_RenderTechnique::loadSpecialUniformLocation(pipeline& p, int RenderPassID)
{

}
*/

void EG_RenderTechnique::enableShader(int RenderPassID)
{
    progShaders[RenderPassID]->useShader();
}


void EG_RenderTechnique::disableShader(int RenderPassID)
{
    progShaders[RenderPassID]->delShader();
}