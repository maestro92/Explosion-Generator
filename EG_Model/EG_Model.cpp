
#include <assert.h>
#include "EG_Model.h"

using namespace std;



EG_Model::EG_Model() : EG_Model(4)
{}


EG_Model::EG_Model(int count)
{
    m_Buffers = new GLuint[count];
    for(int i=0; i<count; i++)
        m_Buffers[i] = 0;
    m_VboCount = count;
}


EG_Model::~EG_Model()
{
    clear();
}

/*
void EG_Model::genGLVertexBuffer(GLuint& id, vector<Vertex>& Vertices)
{
    glGenBuffers(1, &id);
  	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
}

void EG_Model::genGLIndexBuffer(GLuint& id, vector<unsigned int>& Indices)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}
*/

void EG_Model::clear()
{
    for (unsigned int i=0; i<m_Textures.size(); i++)
        SAFE_DELETE(m_Textures[i]);

}



bool EG_Model::loadModel(string filename)
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





void EG_Model::transferDataToBuffer(vector<glm::vec3>& vec, unsigned id)
{
    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[id]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec[0]) * vec.size(), &vec[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void EG_Model::transferDataToBuffer(vector<glm::vec2>& vec, unsigned id)
{
    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[id]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec[0]) * vec.size(), &vec[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void EG_Model::transferDataToBuffer(vector<unsigned int>& vec, unsigned id)
{
    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[id]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vec[0]) * vec.size(), &vec[0], GL_STATIC_DRAW);
}

bool EG_Model::initFromAiScene(const aiScene* s, const string& Filename)
{
    m_Entries.resize(s->mNumMeshes);
    m_Textures.resize(s->mNumMaterials);

    // Count the number of vertices and indices

/*
    vector<glm::vec3> Positions;
    vector<glm::vec3> Normals;
    vector<glm::vec3> Tangents;
    vector<glm::vec3> Colors;
    vector<glm::vec2> UVs;
    vector<unsigned int> Indices;
*/
    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    for (unsigned int i = 0; i < m_Entries.size(); i++)
    {
        m_Entries[i].MaterialIndex = s->mMeshes[i]->mMaterialIndex;
        m_Entries[i].NumIndices = s->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex = NumVertices;
        m_Entries[i].BaseIndex = NumIndices;

        NumVertices += s->mMeshes[i]->mNumVertices;
        NumIndices += m_Entries[i].NumIndices;
    }



    vector<glm::vec3> Positions     = EG_Utility::reserveVector<glm::vec3> (NumVertices);
    vector<glm::vec3> Normals       = EG_Utility::reserveVector<glm::vec3> (NumVertices);
    vector<glm::vec3> Tangents      = EG_Utility::reserveVector<glm::vec3> (NumVertices);
    vector<glm::vec3> Colors        = EG_Utility::reserveVector<glm::vec3> (NumVertices);
    vector<glm::vec2> UVs           = EG_Utility::reserveVector<glm::vec2> (NumVertices);
    vector<unsigned int> Indices    = EG_Utility::reserveVector<unsigned int> (NumIndices);


/*
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    Tangents.reserve(NumVertices);
    Colors.reserve(NumVertices);
    UVs.reserve(NumVertices);
    Indices.reserve(NumIndices);
*/





    for (unsigned int i=0; i<m_Entries.size(); i++)
    {
        const aiMesh* m = s->mMeshes[i];
//        initMesh(i, m, s);
        initMesh(m, s, Positions, Normals, Tangents, Colors, UVs, Indices);
    }

    if (!initMaterials(s, Filename))
    {
        return false;
    }

    transferDataToBuffer(Positions, POS_VB);
    transferDataToBuffer(Normals, NORMAL_VB);
    transferDataToBuffer(Tangents, TANGENT_VB);
    transferDataToBuffer(Colors, COLOR_VB);
    transferDataToBuffer(UVs, UV_VB);
    transferDataToBuffer(Indices, INDEX_BUFFER);
}




void EG_Model::initMesh(const aiMesh* m, const aiScene* s,
                          vector<glm::vec3>& Positions,
                          vector<glm::vec3>& Normals,
                          vector<glm::vec3>& Tangents,
                          vector<glm::vec3>& Colors,
                          vector<glm::vec2>& UVs,
                          vector<unsigned int>& Indices)
{
	aiColor4D col;
	aiMaterial* mat=s->mMaterials[m->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	glm::vec3 defaultColor(col.r,col.g,col.b);

    for (unsigned int i = 0 ; i < m->mNumVertices ; i++)
    {
        glm::vec3 tVec3;

        /// Vertices
   //     copyVec3(tVec3, m->mVertices[i]);
        tVec3 = EG_Utility::toGlmVec(m->mVertices[i]);
        Positions.push_back(tVec3);

        /// normal
   //     copyVec3(tVec3, m->mNormals[i]);
        tVec3 = EG_Utility::toGlmVec(m->mNormals[i]);
        Normals.push_back(tVec3);

        /// tangent
        if(m->mTangents)
            tVec3 = EG_Utility::toGlmVec(m->mTangents[i]);
//            copyVec3(tVec3, m->mTangents[i]);
        else
            tVec3 = glm::vec3(1.0, 0.0, 0.0);
        Tangents.push_back(tVec3);


        /// colors
        if(m->mColors[0])
        {
            tVec3.x = m->mColors[0][i].r;
            tVec3.y = m->mColors[0][i].g;
            tVec3.z = m->mColors[0][i].b;
        }
        else
            tVec3 = defaultColor;
        Colors.push_back(tVec3);

        /// UV
        if(m->mTextureCoords[0])
        {
            tVec3.x=m->mTextureCoords[0][i].x;
            tVec3.y=m->mTextureCoords[0][i].y;
        }
        else
            tVec3 = glm::vec3(0.0f, 0.0f, 0.0f);

        UVs.push_back(glm::vec2(tVec3.x, tVec3.y));
    }

    for (unsigned int i = 0 ; i < m->mNumFaces ; i++)
    {
        const aiFace& Face = m->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}


bool EG_Model::initMaterials(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "/" + Path.data;
                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded texture '%s'\n", FullPath.c_str());
                }
            }
        }

        // Load a white texture in case the model does not include its own texture
        if (!m_Textures[i]) {
            m_Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");

            Ret = m_Textures[i]->Load();
        }
    }

    return Ret;
}




void EG_Model::render()
{
    glBindVertexArray(m_VAO);

    for(unsigned int i=0; i < m_Entries.size(); i++)
    {
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());

        if(m_Textures[MaterialIndex])
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
                                 m_Entries[i].BaseVertex);
    }
    glBindVertexArray(0);
}
