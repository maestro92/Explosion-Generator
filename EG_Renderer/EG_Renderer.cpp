
#include "EG_Renderer.h"


EG_Renderer::EG_Renderer()
{

}

EG_Renderer::~EG_Renderer()
{

}


void EG_Renderer::init()
{

}

void EG_Renderer::init(int nShaders)
{

}


void EG_Renderer::allocateMemberVariables(int nShaders)
{
    m_numShaders = nShaders;
    Matrices_UniLoc = new Matrices_Location[m_numShaders];
    m_shaders = new Shader*[m_numShaders];
}


void EG_Renderer::initMemberVariables()
{
    for(int i=0; i<m_numShaders; i++)
        Init_Shader_GL_Location(m_shaders[i], Matrices_UniLoc[i]);

}


GLuint EG_Renderer::GetUniformLocation(Shader* s, const char* UniformName)
{
    unsigned int shaderID = s->getProgramId();

    GLuint Location = glGetUniformLocation(shaderID, UniformName);

    if(Location == -1)
    {
        cout << "Error in Init_Shader with " << UniformName << endl;
    }
    return Location;
}


// assign GL location
bool EG_Renderer::Init_Shader_GL_Location(Shader* s, Matrices_Location& Mat)
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



void EG_Renderer::loadUniformLocations(pipeline& p, int RenderPassID)
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



void EG_Renderer::enableShader(int RenderPassID)
{
    m_shaders[RenderPassID]->useShader();
}


void EG_Renderer::disableShader(int RenderPassID)
{
    m_shaders[RenderPassID]->delShader();
}


void EG_Renderer::setUniformLocation(GLuint location, int value)
{
    glUniform1i(location, value);
}

void EG_Renderer::setUniformLocation(GLuint location, float value)
{
    glUniform1f(location, value);
}

void EG_Renderer::setUniformLocation(GLuint location, float x, float y)
{
    glUniform2f(location, x, y);
}

void EG_Renderer::setUniformLocation(GLuint location, glm::vec3 value)
{
    glUniform3f(location, value.x, value.y, value.z);
}

/// http://glm.g-truc.net/0.9.2/api/a00132_source.html
/// source code of glm::vec4, you see the union
void EG_Renderer::setUniformLocation(GLuint location, glm::vec4 value)
{
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void EG_Renderer::setUniformLocation(GLuint location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void EG_Renderer::setUniformLocationTranspose(GLuint location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_TRUE, &value[0][0]);
}

void EG_Renderer::render()
{

}
