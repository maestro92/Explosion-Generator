
#include <assert.h>
#include "EG_Model.h"

using namespace std;


#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2

#define INVALID_GL_VALUE -1

EG_Model::Mesh::Mesh()
{
    VB = INVALID_GL_VALUE;
    IB = INVALID_GL_VALUE;
    NumIndices = 0;
    MaterialIndex = -1;
}


EG_Model::Mesh::~Mesh()
{
    if(VB != INVALID_GL_VALUE)
        glDeleteBuffers(1, &VB);

    if(VB != INVALID_GL_VALUE)
        glDeleteBuffers(1, &VB);
}



void EG_Model::Mesh::init(vector<Vertex>& Vertices, vector<unsigned int>& Indices)
{
    NumIndices = Indices.size();
   // genGLVertexBuffer(VB, Vertices);
   // genGLIndexBuffer(IB, Indices);

    glGenBuffers(1, &VB);
  	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);

}


EG_Model::EG_Model()
{

}


EG_Model::~EG_Model()
{
    clear();
}


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


void EG_Model::clear()
{
    for (unsigned int i=0; i<m_Textures.size(); i++)
        SAFE_DELETE(m_Textures[i]);

}

bool EG_Model::loadModel(string filename)
{
    clear();

    bool ret = false;

	const aiScene* tScene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (tScene)
    {
        ret = initFromAiScene(tScene, filename);
        cout << "Ret is " << ret << endl;
    }
    else {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), aiGetErrorString());
    }

    return ret;
}





bool EG_Model::initFromAiScene(const aiScene* tScene, const string& Filename)
{
    m_Entries.resize(tScene->mNumMeshes);
    m_Textures.resize(tScene->mNumMaterials);

    // Count the number of vertices and indices
    for (unsigned int i=0; i<m_Entries.size(); i++)
    {
        const aiMesh* tMesh = tScene->mMeshes[i];
        initMesh(i, tMesh, tScene);
    }

    return initMaterials(tScene, Filename);
}

void EG_Model::copyVec3(glm::vec3& v, aiVector3D& c)
{
    v.x = c.x;
    v.y = c.y;
    v.z = c.z;
}


void EG_Model::initMesh(unsigned int Index, const aiMesh* m, const aiScene* scene)
{
    m_Entries[Index].MaterialIndex = m->mMaterialIndex;

    vector<Vertex> Vertices;
    vector<unsigned int> Indices;

	aiColor4D col;
	aiMaterial* mat=scene->mMaterials[m->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	glm::vec3 defaultColor(col.r,col.g,col.b);

    for (unsigned int i = 0 ; i < m->mNumVertices ; i++)
    {
        Vertex v;
        glm::vec3 tVec3;

        /// Vertices
        copyVec3(tVec3, m->mVertices[i]);
        v.m_pos = tVec3;

        /// normal
        copyVec3(tVec3, m->mNormals[i]);
        v.m_normal = tVec3;

        /// tangent
        if(m->mTangents)
            copyVec3(tVec3, m->mNormals[i]);
        else
            tVec3 = glm::vec3(1.0, 0.0, 0.0);
        v.m_tangent = tVec3;


        /// colors
        if(m->mColors[0])
        {
            tVec3.x = m->mColors[0][i].r;
            tVec3.y = m->mColors[0][i].g;
            tVec3.z = m->mColors[0][i].b;
        }
        else
            tVec3 = defaultColor;
        v.m_color = tVec3;

        /// UV
        if(m->mTextureCoords[0])
        {
            tVec3.x=m->mTextureCoords[0][i].x;
            tVec3.y=m->mTextureCoords[0][i].y;
        }
        else
            tVec3 = glm::vec3(0.0f, 0.0f, 0.0f);

        v.m_UV.x = tVec3.x;
        v.m_UV.y = tVec3.y;
        Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < m->mNumFaces ; i++)
    {
        const aiFace& Face = m->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_Entries[Index].init(Vertices, Indices);
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

    int VERTEX=0;
	int NORMAL=1;
	int TANGENT=2; //2
	int COLOR=3; //3
	int UV=4; //4

    glEnableVertexAttribArray(VERTEX);
    glEnableVertexAttribArray(NORMAL);
    glEnableVertexAttribArray(TANGENT);
    glEnableVertexAttribArray(COLOR);
    glEnableVertexAttribArray(UV);


    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
        glVertexAttribPointer(VERTEX,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(NORMAL,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3*sizeof(float)));
        glVertexAttribPointer(TANGENT,  3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(6*sizeof(float)));
        glVertexAttribPointer(COLOR,    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(9*sizeof(float)));
        glVertexAttribPointer(UV,       2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(12*sizeof(float)));


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
        if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);


        glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(VERTEX);
    glDisableVertexAttribArray(NORMAL);
    glDisableVertexAttribArray(TANGENT);
    glDisableVertexAttribArray(COLOR);
    glDisableVertexAttribArray(UV);
}



/*
void EG_Model::initMesh(unsigned int Index, const aiMesh* m, const aiScene* scene)
{
    m_Entries[Index].MaterialIndex = m->mMaterialIndex;

    vector<Vertex> Vertices;
    vector<unsigned int> Indices;

	aiColor4D col;
	aiMaterial* mat=scene->mMaterials[m->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	glm::vec3 defaultColor(col.r,col.g,col.b);

    for (unsigned int i = 0 ; i < m->mNumVertices ; i++)
    {
        Vertex v;
        glm::vec3 tVec3;

        /// Vertices
        copyVec3(tVec3, m->mVertices[i], );
        v.m_pos = tVec3;

        /// normal
        copyVec3(tVec3, m->mNormals[i]);
        v.m_normal = tVec3;

        /// tangent
        if(m->mTangents[i])
            copyVec3(tVec3, m->mNormals[i]);
        else
            tVec3 = glm::vec3(1.0, 0.0, 0.0);
        v.m_tangent = tVec3;


        /// colors
        if(m->mColors[0])
        {
            tVec3.x = m->mColors[0][i].r;
            tVec3.y = m->mColors[0][i].g;
            tVec3.z = m->mColors[0][i].b;
        }
        else
            tVec3 = defaultColor;
        v.m_color = tVec3;

        /// UV
        if(m->mTextureCoords[0])
        {
            tVec3.x=m->mTextureCoords[0][i].x;
            tVec3.y=m->mTextureCoords[0][i].y;
        }
        else
            tVec3 = glm::vec3(0.0f, 0.0f, 0.0f);

        v.m_UV.x = tVec3.x;
        v.m_UV.y = tVec3.y;
        Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < m->mNumFaces ; i++)
    {
        const aiFace& Face = m->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_Entries[Index].init(Vertices, Indices);
}
*/

/*

void EG_Model::initMesh(unsigned int Index, const aiMesh* tMesh)
{
    m_Entries[Index].MaterialIndex = tMesh->mMaterialIndex;

    vector<Vertex> Vertices;
    vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < tMesh->mNumVertices ; i++)
    {
        const aiVector3D* pPos      = &(tMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(tMesh->mNormals[i]);
        const aiVector3D* pTexCoord = tMesh->HasTextureCoords(0) ? &(tMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
                 glm::vec2(pTexCoord->x, pTexCoord->y),
                 glm::vec3(pNormal->x, pNormal->y, pNormal->z));

        Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < tMesh->mNumFaces ; i++)
    {
        const aiFace& Face = tMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_Entries[Index].init(Vertices, Indices);
}



void EG_Model::render()
{

    int VERTEX=0;
	int NORMAL=1;
	int TANGENT=2; //2
	int COLOR=3; //3
	int UV=4; //4



    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
        if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
        {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }

        glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
*/



/*
void EG_Mesh::clear()
{
    for (unsigned int i = 0 ; i < m_Textures.size() ; i++) {
        SAFE_DELETE(m_Textures[i]);
    }

    if (m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }

    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}
*/

/// takes a filename
/// loads vertex buffers, index buffers, and texture objects
/// Assimp uses and aiScene object
/// m_Entries member of EG_Mesh class is a vector of the MeshEntry struct



/*
bool EG_Mesh::loadMesh(const string& Filename)
{
    // Release the previously loaded EG_Mesh (if it exists)
    clear();

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool Ret = false;
 //   Assimp::Importer importer;

//    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);


	const aiScene* pScene=aiImportFile(Filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (pScene) {
        Ret = initFromAiScene(pScene, Filename);
        cout << "Ret is " << Ret << endl;
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), aiGetErrorString());
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);

    Ret = true;
    return Ret;
}





bool EG_Mesh::initFromAiScene(const aiScene* pScene, const string& Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    vector<glm::vec3> Positions;
    vector<glm::vec3> Normals;
    vector<glm::vec2> TexCoords;
    vector<unsigned int> Indices;

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    // Count the number of vertices and indices
    for (unsigned int i=0; i<m_Entries.size(); i++)
    {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex = NumVertices;
        m_Entries[i].BaseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Entries[i].NumIndices;
    }
    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(paiMesh, Positions, Normals, TexCoords, Indices);
    }

    if (!initMaterials(pScene, Filename)) {
        return false;
    }

    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    return GLCheckError();
}



void EG_Mesh::initMesh(const aiMesh* paiMesh,
                    vector<glm::vec3>& Positions,
                    vector<glm::vec3>& Normals,
                    vector<glm::vec2>& TexCoords,
                    vector<unsigned int>& Indices)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++)
    {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
        Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
    }

    // Populate the index buffer
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++)
    {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}

bool EG_Mesh::initMaterials(const aiScene* pScene, const string& Filename)
{
    // Extract the directory part from the file name
    string::size_type SlashIndex = Filename.find_last_of("/");
    string Dir;

    if (SlashIndex == string::npos) {
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

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                string p(Path.data);

                if (p.substr(0, 2) == ".\\") {
                    p = p.substr(2, p.size() - 2);
                }

                string FullPath = Dir + "/" + p;

                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load())
                {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else
                {
                    printf("Loaded texture '%s'\n", FullPath.c_str());
                }
            }
        }
    }

    return Ret;
}


void EG_Mesh::render()
{
    glBindVertexArray(m_VAO);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());

        if (m_Textures[MaterialIndex])
        {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }

		glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
                                 m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}

void EG_Mesh::render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());

        if (m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
                                          m_Entries[i].NumIndices,
                                          GL_UNSIGNED_INT,
                                          (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
                                          NumInstances,
                                          m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}
*/








