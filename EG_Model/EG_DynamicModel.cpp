
#include "EG_DynamicModel.h"


EG_DynamicModel::EG_DynamicModel() : EG_Model(6)
{
    m_NumBones = 0;
}


EG_DynamicModel::~EG_DynamicModel()
{
    clear();
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
        ret = initFromAiScene(m_Scene, filename);
        cout << "Ret is " << ret << endl;
    }
    else {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), aiGetErrorString());
    }

    glBindVertexArray(0);
    return ret;
}





void EG_DynamicModel::transferDataToBuffer(vector<VertexBoneData>& vec, unsigned int bufferIndex, unsigned int location)
{
    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[bufferIndex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec[0]) * vec.size(), &vec[0], GL_STATIC_DRAW);

    /// weight pointer starts at the 16th byte(4 ints/floats)
    glEnableVertexAttribArray(location);
    glVertexAttribIPointer(location, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(location+1);
    glVertexAttribPointer(location+1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
}




bool EG_DynamicModel::initFromAiScene(const aiScene* s, const string& Filename)
{
    m_Entries.resize(s->mNumMeshes);
    m_Textures.resize(s->mNumMaterials);

    // Count the number of vertices and indices
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
    vector<VertexBoneData> Bones    = EG_Utility::reserveVector<VertexBoneData> (NumVertices);


    for (unsigned int i=0; i<m_Entries.size(); i++)
    {
        const aiMesh* m = s->mMeshes[i];
//        initMesh(i, m, s);
//        initMesh(m, s, Positions, Normals, Tangents, Colors, UVs, Indices, Bones);
    }
/*
    if (!initMaterials(s, Filename))
    {
        return false;
    }
*/

    EG_Model::transferDataToBuffer(Positions, POS_VB, POS_UNI_LOC);
    EG_Model::transferDataToBuffer(Normals, NORMAL_VB, NORMAL_UNI_LOC);
    EG_Model::transferDataToBuffer(Tangents, TANGENT_VB, TANGENT_UNI_LOC);
    EG_Model::transferDataToBuffer(Colors, COLOR_VB, COLOR_UNI_LOC);
    EG_Model::transferDataToBuffer(UVs, UV_VB, UV_UNI_LOC);
    EG_Model::transferDataToBuffer(Indices, INDEX_BUFFER);
    transferDataToBuffer(Bones, BONE_VB, BONE_ID_UNI_LOC);

}




void EG_DynamicModel::initMesh(unsigned int MeshIndex, const aiMesh* m, const aiScene* s,
                                  vector<glm::vec3>& Positions,
                                  vector<glm::vec3>& Normals,
                                  vector<glm::vec3>& Tangents,
                                  vector<glm::vec3>& Colors,
                                  vector<glm::vec2>& UVs,
                                  vector<unsigned int>& Indices,
                                  vector<VertexBoneData>& Bones)
{
    glm::vec3 defaultColor = EG_Model::getMaterialColor(m, s);
/*
    // Populate the vertex attribute vectors
    for (unsigned int i = 0 ; i < m->mNumVertices ; i++)
    {
        /// position, normal, tangent, color, uv
        glm::vec3 pos   = EG_Utility::toGlmVec(m->mVertices[i]);
        glm::vec3 norm  = EG_Utility::toGlmVec(m->mNormals[i]);
        glm::vec3 tang  = glm::vec3(1.0, 0.0, 0.0);
        glm::vec3 color = defaultColor;
        glm::vec2 uv    = glm::vec2(0.0f, 0.0f);

        if(m->mTangents)
            tang = EG_Utility::toGlmVec(m->mTangents[i]);

        if(m->mColors[0])
        {
            color.x = m->mColors[0][i].r;
            color.y = m->mColors[0][i].g;
            color.z = m->mColors[0][i].b;
        }

        if(m->mTextureCoords[0])
        {
            uv.x=m->mTextureCoords[0][i].x;
            uv.y=m->mTextureCoords[0][i].y;
        }

        Positions.push_back(pos);
        Normals.push_back(norm);
        Tangents.push_back(tang);
        Colors.push_back(color);
        UVs.push_back(uv);
    }

    loadBones(MeshIndex, paiMesmh, Bones);

    EG_Model::initIndexVectors(m, Indices);
    */
}



/// loads the vertex bone information for a single mesh object
void EG_DynamicModel::loadBones(unsigned int MeshIndex, const aiMesh* m, vector<VertexBoneData>& Bones)
{
    for (unsigned int i=0; i<m->mNumBones; i++)
    {
        unsigned int BoneIndex = 0;
        string BoneName(m->mBones[i]->mName.data);

        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
        {
            BoneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            bi.BoneOffset = EG_Utility::toGlmMat(m->mBones[i]->mOffsetMatrix);
            m_BoneInfo.push_back(bi);
        }
        else
            BoneIndex = m_BoneMapping[BoneName];


        for(int j=0; j<m->mBones[i]->mNumWeights; j++)
        {

        }
    }

    /// iterating through weights


}
