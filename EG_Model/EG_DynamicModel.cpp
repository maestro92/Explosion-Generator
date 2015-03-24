
#include "EG_DynamicModel.h"


EG_DynamicModel::EG_DynamicModel() : EG_Model(6)
{

}


bool EG_DynamicModel::loadModel(string filename)
{
    clear();

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(m_VboCount, m_Buffers);

    bool ret = false;

	m_Scene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (m_Scene)
    {
        m_GlobalInverseTransform = EG_Utility::toGlmMat(m_Scene->mRootNode->mTransformation);
        m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
   //     ret = initFromAiScene(m_Scene, filename);
        cout << "Ret is " << ret << endl;
    }
    else {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), aiGetErrorString());
    }

    glBindVertexArray(0);
    return ret;
}


/*

bool EG_Model::initFromAiScene(const aiScene* s, const string& Filename)
{
    // Count the number of vertices and indices
    m_Entries.resize(s->mNumMeshes);
    m_Textures.resize(s->mNumMaterials);

    vector<glm::vec3> Positions;
    vector<glm::vec3> Normals;
    vector<glm::vec3> Tangents;
    vector<glm::vec3> Colors;
    vector<glm::vec2> UVs;
    vector<unsigned int> Indices;
    vector<VertexBoneData> Bones;

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

    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    Tangents.reserve(NumVertices);
    Colors.reserve(NumVertices);
    UVs.reserve(NumVertices);
    Indices.reserve(NumIndices);
    Bones.resize(NumVertices);

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

*/
