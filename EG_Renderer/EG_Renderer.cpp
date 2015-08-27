
#include "EG_Renderer.h"


EG_Renderer::EG_Renderer()
{
    m_curShader = -1;
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
//    Matrices_UniLoc = new Matrices_Location[m_numShaders];
//    m_shaders = new Shader*[m_numShaders];
    //m_allDataPairs = new DataPair

    m_matricesUniLocs.resize(nShaders);
    m_shaders.resize(nShaders);
    m_allDataPairs.resize(nShaders);
}

void EG_Renderer::addShader(Shader* s)
{
    m_shaders.push_back(s);

    Matrices_Location mat;
    getMatrixUniLocs(s, mat);
    m_matricesUniLocs.push_back(mat);

    vector<DataPair*> v;
    m_allDataPairs.push_back(v);

    unordered_map<string, DataPair*> table;
    tables.push_back(table);
}


void EG_Renderer::initMemberVariables()
{
    for(int i=0; i<m_numShaders; i++)
        Init_Shader_GL_Location(m_shaders[i], m_matricesUniLocs[i]);


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

void EG_Renderer::initDataPairUniLoc(DataPair* p, int pass, const char* name)
{
    p->uniLoc = getUniLoc(m_shaders[pass], name);
    m_allDataPairs[pass].push_back(p);
}



void EG_Renderer::addDataPair(int pass, const char* name, int dataType)
{
    DataPair* p;

    switch(dataType)
    {
		case DP_INT:
        {
            tables[pass][name] = new IntDataPair(name);
            break;
        }
        case DP_FLOAT:
        {
            tables[pass][name] = new FloatDataPair(name);
            break;
        }
        case DP_VEC2:
        {
            tables[pass][name] = new Vec2DataPair(name);
            break;
        }
        case DP_VEC3:
        {
            tables[pass][name] = new Vec3DataPair(name);
            break;
        }
        case DP_VEC4:
        {
            tables[pass][name] = new Vec4DataPair(name);
            break;
        }
        case DP_MAT3:
        {
            tables[pass][name] = new Mat3DataPair(name);
            break;
        }
        case DP_MAT4:
        {
            tables[pass][name] = new Mat4DataPair(name);
            break;
        }
    }

    tables[pass][name]->uniLoc = getUniLoc(m_shaders[pass], name);

//    p->label = "here";
//    tables[pass][name] = p;

/*
    DataPair* dpdp = tables[RENDER_PASS2]["u_color"];
    dpdp->setValue(glm::vec4(1.0,0.0,0.0,1.0));
    EG_Utility::debug("it is", dpdp->value)
*/



/*
    (tables[RENDER_PASS2]["u_color"])->setValue(glm::vec4(1.0,0.0,0.0,1.0));
   // EG_Utility::debug("it is", (tables[RENDER_PASS2]["u_color"])->value);
    tables[RENDER_PASS2]["u_color"]->printValue();
    int a = 1;
*/

//    std::pair<string, DataPair*> newPair(name, p);
//    tables[pass].insert(newPair);
}




void EG_Renderer::setData(int pass, const char* name, int value)
{
    tables[pass][name]->setValue(value);
}

void EG_Renderer::setData(int pass, const char* name, int value, GLuint unit, GLuint textureId)
{
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, textureId);

    textureUnitStack.push(unit);
    tables[pass][name]->setValue(value);
}

void EG_Renderer::setData(int pass, const char* name, float value)
{
    tables[pass][name]->setValue(value);
}

void EG_Renderer::setData(int pass, const char* name, glm::vec2 value)
{
    tables[pass][name]->setValue(value);
}

void EG_Renderer::setData(int pass, const char* name, glm::vec3 value)
{
    tables[pass][name]->setValue(value);
}

void EG_Renderer::setData(int pass, const char* name, glm::vec4 value)
{
    tables[pass][name]->setValue(value);
}

void EG_Renderer::setData(int pass, const char* name, glm::mat3 value)
{
    tables[pass][name]->setValue(value);
}

void EG_Renderer::setData(int pass, const char* name, glm::mat4 value)
{
    tables[pass][name]->setValue(value);
}






void EG_Renderer::printTables()
{

    DataPair* dp = tables[1]["u_color"];

    for (int i=0; i<tables.size(); i++)
    {
        EG_Utility::debug("Table ", i);

        for ( auto it = tables[i].begin(); it != tables[i].end(); ++it )
        {
            cout << " " << it->first;
            cout << ":" << it->second << endl;
        }

        EG_Utility::debugLn(2);
    }
}

/*
void EG_Renderer::initDataPairUniLoc(DataPair* p, Shader* s, int pass, const char* name)
{
    p->uniLoc = getUniLoc(s, name);
    m_allDataPairs[pass].push_back(p);
}
*/


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
    if(RenderPassID != m_curShader)
    {
        EG_Utility::debug("######### RenderPassID Not Matching");
    }

	if(!p.matricesReady)
	{
		p.modelViewMatrix=p.viewMatrix[p.viewMatrix.size()-1]*p.modelMatrix[p.modelMatrix.size()-1];
		p.modelViewProjectionMatrix=p.projectionMatrix[p.projectionMatrix.size()-1]*
                                    p.viewMatrix[p.viewMatrix.size()-1]*
                                    p.modelMatrix[p.modelMatrix.size()-1];
		p.normalMatrix=glm::mat3(p.modelViewMatrix);
	}
/*
	glUniformMatrix4fv(Matrices_UniLoc[RenderPassID].ModelMatrix,1,GL_FALSE,&p.modelMatrix[p.modelMatrix.size()-1][0][0]);
    glUniformMatrix4fv(Matrices_UniLoc[RenderPassID].ViewMatrix,1,GL_FALSE,&p.viewMatrix[p.viewMatrix.size()-1][0][0]);
	glUniformMatrix4fv(Matrices_UniLoc[RenderPassID].ModelviewMatrix,1,GL_FALSE,&p.modelViewMatrix[0][0]);
	glUniformMatrix4fv(Matrices_UniLoc[RenderPassID].ModelviewProjection,1,GL_FALSE,&p.modelViewProjectionMatrix[0][0]);
	glUniformMatrix3fv(Matrices_UniLoc[RenderPassID].NormalMatrix,1,GL_FALSE,&p.normalMatrix[0][0]);
*/
	glUniformMatrix4fv(m_matricesUniLocs[RenderPassID].ModelMatrix,1,GL_FALSE,&p.modelMatrix[p.modelMatrix.size()-1][0][0]);
    glUniformMatrix4fv(m_matricesUniLocs[RenderPassID].ViewMatrix,1,GL_FALSE,&p.viewMatrix[p.viewMatrix.size()-1][0][0]);
	glUniformMatrix4fv(m_matricesUniLocs[RenderPassID].ModelviewMatrix,1,GL_FALSE,&p.modelViewMatrix[0][0]);
	glUniformMatrix4fv(m_matricesUniLocs[RenderPassID].ModelviewProjection,1,GL_FALSE,&p.modelViewProjectionMatrix[0][0]);
	glUniformMatrix3fv(m_matricesUniLocs[RenderPassID].NormalMatrix,1,GL_FALSE,&p.normalMatrix[0][0]);
}



void EG_Renderer::enableShader(int RenderPassID)
{
    m_shaders[RenderPassID]->useShader();
    m_curShader = RenderPassID;
}


void EG_Renderer::disableShader(int RenderPassID)
{
    m_shaders[RenderPassID]->delShader();
    m_curShader = -1;
    while(!textureUnitStack.empty())
    {
        glActiveTexture(textureUnitStack.top());
        glBindTexture(GL_TEXTURE_2D, 0);
        textureUnitStack.pop();
    }
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
        getMatrixUniLocs(m_shaders[i], m_matricesUniLocs[i]);

    //    getMatrixUniLocs(m_shaders[i], Matrices_UniLoc[i]);

}


bool EG_Renderer::getMatrixUniLocs(Shader* s, Matrices_Location& Mat)
{
    Mat.ModelviewProjection = getUniLoc( s, "u_modelViewProjMat");
    Mat.ModelviewMatrix     = getUniLoc( s, "u_modelViewMat");
    Mat.ViewMatrix          = getUniLoc( s, "u_viewMat");
    Mat.ProjectionMatrix    = getUniLoc( s, "u_projMat");
    Mat.ModelMatrix         = getUniLoc( s, "u_modelMat");
    Mat.ViewNoRotateMatrix  = getUniLoc( s, "u_viewNoRotateMat");
    Mat.NormalMatrix        = getUniLoc( s, "u_normalMat");

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


