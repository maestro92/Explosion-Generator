
#include "EG_InstancedModel.h"

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TANGENT_LOCATION 2
#define COLOR_LOCATION 3
#define UV_LOCATION 4
#define WVP_LOCATION 5
#define WORLD_LOCATION 9


EG_InstancedModel::EG_InstancedModel() : EG_Model(8)
{
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
}



bool EG_InstancedModel::loadModel(string filename)
{
    clear();

    /// Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    /// Create the buffers for the vertices attributes
    glGenBuffers(m_VboCount, m_Buffers);

    bool ret = false;

	m_Scene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (m_Scene)
    {
        ret = initFromAiScene(m_Scene, filename);
        cout << "Ret is " << ret << endl;
    }
    else {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), aiGetErrorString());
    }

    glBindVertexArray(0);

    return ret;
}



bool EG_InstancedModel::initFromAiScene(const aiScene* s, const string& Filename)
{
    EG_Model::initFromAiScene(s, Filename);


    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
    for (unsigned int i = 0; i < 4 ; i++)
    {
        glEnableVertexAttribArray(WVP_LOCATION + i);
        glVertexAttribPointer(WVP_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(WVP_LOCATION + i, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);

    for (unsigned int i = 0; i < 4 ; i++)
    {
        glEnableVertexAttribArray(WORLD_LOCATION + i);
        glVertexAttribPointer(WORLD_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(WORLD_LOCATION + i, 1);
    }


    EG_Utility::checkGLError();

/*
    GLenum err;
    if( (err = GLCheckError()) != GL_NO_ERROR)
    {
        cout << "Checking error " << err << endl;
    }
*/
}


void EG_InstancedModel::render(unsigned int NumInstances, glm::mat4* WVPMats, glm::mat4* WorldMats)
{

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);

 //   EG_Model::render();

    glBindVertexArray(m_VAO);

    for (unsigned int i = 0 ; i < m_Meshes.size() ; i++) {
        const unsigned int MaterialIndex = m_Meshes[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());

        if (m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }

        glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
                                         m_Meshes[i].NumIndices,
                                         GL_UNSIGNED_INT,
                                         (void*)(sizeof(unsigned int) * m_Meshes[i].BaseIndex),
                                         NumInstances,
                                         m_Meshes[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}


