
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
    //m_allDataPairs = new DataPair
    m_allDataPairs.resize(nShaders);
}


void EG_Renderer::initMemberVariables()
{
    for(int i=0; i<m_numShaders; i++)
        Init_Shader_GL_Location(m_shaders[i], Matrices_UniLoc[i]);

}






GLuint EG_Renderer::GetUniformLocation(Shader* s, const char* name)
{
    unsigned int shaderID = s->getProgramId();

    GLuint location = glGetUniformLocation(shaderID, name);

    if(location == -1)
    {
        cout << "Error in Init_Shader with " << name << endl;
    }
    return location;
}
/*
void EG_Renderer::initDataPairUniLoc(DataPair* p, Shader* s, const char* name)
{
    p->uniLoc = getUniLoc(s, name);
}
*/
void EG_Renderer::initDataPairUniLoc(DataPair* p, Shader* s, int pass, const char* name)
{
    p->uniLoc = getUniLoc(s, name);
    m_allDataPairs[pass].push_back(p);
}


GLuint EG_Renderer::getUniLoc(Shader* s, const char* name)
{
    return GetUniformLocation(s, name);
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

void EG_Renderer::setUniformLocation(GLuint location, glm::vec2 value)
{
    glUniform2f(location, value.x, value.y);
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

void EG_Renderer::setUniformLocation(GLuint location, glm::mat3 value)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
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




void EG_Renderer::getAllMatrixUniLocs()
{
    for(int i=0; i<m_numShaders; i++)
        getMatrixUniLocs(m_shaders[i], Matrices_UniLoc[i]);

}


bool EG_Renderer::getMatrixUniLocs(Shader* s, Matrices_Location& Mat)
{
    Mat.ModelviewProjection = getUniLoc( s, "m_ModelviewProjection");
    Mat.ModelviewMatrix     = getUniLoc( s, "m_Modelview");
    Mat.ViewMatrix          = getUniLoc( s, "m_ViewMatrix");
    Mat.ProjectionMatrix    = getUniLoc( s, "m_ProjectionMatrix");
    Mat.ModelMatrix         = getUniLoc( s, "m_ModelMatrix");
    Mat.ViewNoRotateMatrix  = getUniLoc( s, "m_ViewNoRotateMatrix");
    Mat.NormalMatrix        = getUniLoc( s, "m_normalMatrix");

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





void EG_Renderer::setAllDataPairUniLocs(int pass)
{
    int size = m_allDataPairs[pass].size();
    for(int i=0; i<size; i++)
        m_allDataPairs[pass][i]->setUniLoc();
 //       setDataPairUniLoc( m_allDataPairs[pass][i]->uniLoc, m_allDataPairs[pass][i]->value);
}




void EG_Renderer::setDataPairUniLoc(IntDataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void EG_Renderer::setDataPairUniLoc(FloatDataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void EG_Renderer::setDataPairUniLoc(Vec2DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value.x, dp.value.y);
}

void EG_Renderer::setDataPairUniLoc(Vec3DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void EG_Renderer::setDataPairUniLoc(Vec4DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void EG_Renderer::setDataPairUniLoc(Mat3DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void EG_Renderer::setDataPairUniLoc(Mat4DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}


