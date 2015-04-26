
#include "EG_DynamicModel.h"


EG_DynamicModel::EG_DynamicModel() : EG_Model(7)
{
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
    m_NumBones = 0;
    m_Scene = NULL;
    m_TicksPerSecond = 0.0f;
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
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec[0]) * vec.size(), &vec[0], GL_STATIC_DRAW);

    /// weight pointer starts at the 16th byte(4 ints/floats)
    glEnableVertexAttribArray(BONE_ID_UNI_LOC);
    glVertexAttribIPointer(BONE_ID_UNI_LOC, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_UNI_LOC);
    glVertexAttribPointer(BONE_WEIGHT_UNI_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
}




bool EG_DynamicModel::initFromAiScene(const aiScene* s, const string& Filename)
{


    if(m_Scene->mAnimations)
    {
        m_TicksPerSecond = (float)(s->mAnimations[0]->mTicksPerSecond != 0 ? s->mAnimations[0]->mTicksPerSecond : 25.0f);
        m_AnimFrameDuration = (float)s->mAnimations[0]->mDuration;
    }

    m_Meshes.resize(s->mNumMeshes);
    m_Textures.resize(s->mNumMaterials);


    // Count the number of vertices and indices
    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i].MaterialIndex = s->mMeshes[i]->mMaterialIndex;
        m_Meshes[i].NumIndices = s->mMeshes[i]->mNumFaces * 3;
        m_Meshes[i].BaseVertex = NumVertices;
        m_Meshes[i].BaseIndex = NumIndices;

        NumVertices += s->mMeshes[i]->mNumVertices;
        NumIndices += m_Meshes[i].NumIndices;
    }

    vector<glm::vec3> Positions     = EG_Utility::reserveVector<glm::vec3> (NumVertices);
    vector<glm::vec3> Normals       = EG_Utility::reserveVector<glm::vec3> (NumVertices);
    vector<glm::vec3> Tangents      = EG_Utility::reserveVector<glm::vec3> (NumVertices);
    vector<glm::vec3> Colors        = EG_Utility::reserveVector<glm::vec3> (NumVertices);
    vector<glm::vec2> UVs           = EG_Utility::reserveVector<glm::vec2> (NumVertices);
    vector<unsigned int> Indices    = EG_Utility::reserveVector<unsigned int> (NumIndices);
    vector<VertexBoneData> Bones;
    Bones.resize(NumVertices);

    //    = EG_Utility::reserveVector<VertexBoneData> (NumVertices);
  //  vector<glm::vec3> Positions;
  //  vector<glm::vec3> Normals;
  //  vector<glm::vec3> Tangents;
  //  vector<glm::vec3> Colors;
  //  vector<glm::vec2> UVs;
  //  vector<unsigned int> Indices;
 //   vector<VertexBoneData> Bones;


    // Reserve space in the vectors for the vertex attributes and indices
  //  Positions.reserve(NumVertices);
  //  Normals.reserve(NumVertices);



    for (unsigned int i=0; i<m_Meshes.size(); i++)
    {
        const aiMesh* m = s->mMeshes[i];
        initMesh(i, m, s, Positions, Normals, Tangents, Colors, UVs, Indices, Bones);
    }

    if (!EG_Model::initMaterials(s, Filename))
    {
        return false;
    }

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
    EG_Model::initVertexVectors(m, Positions, Normals, Tangents, Colors, UVs, defaultColor);
    loadBones(MeshIndex, m, Bones);
    EG_Model::initIndexVectors(m, Indices);
}


#if 1
/// loads the vertex bone information for a single mesh object
void EG_DynamicModel::loadBones(unsigned int MeshIndex, const aiMesh* m, vector<VertexBoneData>& Bones)
{
    m_boneTransforms.resize(m_NumBones);

    for (unsigned int i=0; i<m->mNumBones; i++)
    {
        /// we first write down the bone's name
        /// and it's index in the mesh?
        unsigned int BoneIndex = 0;
        string BoneName(m->mBones[i]->mName.data);

        /// we store the bones according to names, so we can use it later when we traverse the tree


        /// if it's a new bone
        /// we push it in m_BoneInfo
        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
        {
            /*
            BoneIndex = m_NumBones;
            m_NumBones++;

            BoneInfo bi;
            bi.boneOffset = EG_Utility::toGlmMat(m->mBones[i]->mOffsetMatrix);
            m_BoneInfo.push_back(bi);
            */

            BoneIndex = m_NumBones;
            m_NumBones++;
	        BoneInfo bi;
			m_BoneInfo.push_back(bi);

			m_BoneInfo[BoneIndex].boneOffset = EG_Utility::toGlmMat(m->mBones[i]->mOffsetMatrix);

			//   m_BoneInfo[BoneIndex].BoneOffset1 = pMesh->mBones[i]->mOffsetMatrix;

            m_BoneMapping[BoneName] = BoneIndex;

        }
        else
        {
            BoneIndex = m_BoneMapping[BoneName];
        }


        /// for each bone, we go through the Vertex that it influences
        /// then
        for(int j=0; j<m->mBones[i]->mNumWeights; j++)
        {
            aiBone* b = m->mBones[i];

            /// each mesh has vertices
            /// VertexID is the mesh.index + vertex.index
            /// for each bone, it influences different Vertex, so it's not in order
            /// example: Bone1 - v1, v4, v123, v44
            ///          Bone2 - v1, v4, v233, v43
            /// for this reason, we had to do it this way, instead of Bones.push_back...
            unsigned VertexID = m_Meshes[MeshIndex].BaseVertex + b->mWeights[j].mVertexId;
            float Weight = b->mWeights[j].mWeight;
            Bones[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }

    /// iterating through weights
}
#endif




void EG_DynamicModel::boneTransform(float timeInSeconds, vector<glm::mat4>& transforms)
{
    glm::mat4 identity = glm::mat4(1.0f);
    float timeInTicks = timeInSeconds * m_TicksPerSecond;
    float animationTime = fmod(timeInTicks, m_AnimFrameDuration);

    readNodeHierarchy(animationTime, m_Scene->mRootNode, identity);
    transforms.resize(m_NumBones);

    for(int i=0; i<m_NumBones; i++)
        transforms[i] = m_BoneInfo[i].finalTransformation;
}

void EG_DynamicModel::boneTransform(float timeInSeconds)
{
    glm::mat4 identity = glm::mat4(1.0f);
    float timeInTicks = timeInSeconds * m_TicksPerSecond;
    float animationTime = fmod(timeInTicks, m_AnimFrameDuration);

    readNodeHierarchy(animationTime, m_Scene->mRootNode, identity);

    for(int i=0; i<m_NumBones; i++)
        m_boneTransforms[i] = m_BoneInfo[i].finalTransformation;
}



void EG_DynamicModel::readNodeHierarchy(float animationTime, const aiNode* node, const glm::mat4& parentTransform)
{
    string nodeName(node->mName.data);
    const aiAnimation* animation = m_Scene->mAnimations[0];

    glm::mat4 nodeTransformation = EG_Utility::toGlmMat(node->mTransformation);

    const aiNodeAnim* nodeAnim = findNodeAnim(animation, nodeName);

    /// from http://assimp.sourceforge.net/lib_html/structai_node_anim.html
    /// he name specifies the bone/node which is affected by this animation channel.
    /// The keyframes are given in three separate series of values, one each for position, rotation and scaling.
    /// The transformation matrix computed from these values replaces the node's original transformation matrix at a specific time.
    if(nodeAnim)
    {
        glm::mat4 scalingMat = computeInterpolatedScalingMatrix(animationTime, nodeAnim);
        glm::mat4 rotationMat = computeInterpolatedRotationMatrix(animationTime, nodeAnim);
        glm::mat4 translationMat = computeInterpolatedPositionMatrix(animationTime, nodeAnim);

        nodeTransformation = scalingMat * rotationMat * translationMat;
//        nodeTransformation = translationMat * rotationMat * scalingMat;
    }

  //  glm::mat4 globalTransformation = parentTransform * nodeTransformation;
    glm::mat4 globalTransformation = nodeTransformation * parentTransform;

    if(m_BoneMapping.find(nodeName) != m_BoneMapping.end())
    {
        unsigned int boneIndex = m_BoneMapping[nodeName];

        // m_BoneInfo[boneIndex].finalTransformation = m_GlobalInverseTransform * globalTransformation * m_BoneInfo[boneIndex].boneOffset;
        m_BoneInfo[boneIndex].finalTransformation = m_BoneInfo[boneIndex].boneOffset * globalTransformation * m_GlobalInverseTransform;
    }

    for (unsigned int i=0; i < node->mNumChildren; i++)
        readNodeHierarchy(animationTime, node->mChildren[i], globalTransformation);
}


void EG_DynamicModel::boneTransformTranspose(float timeInSeconds, vector<glm::mat4>& transforms)
{
    glm::mat4 identity = glm::mat4(1.0f);
    float timeInTicks = timeInSeconds * m_TicksPerSecond;
    float animationTime = fmod(timeInTicks, m_AnimFrameDuration);

    readNodeHierarchy(animationTime, m_Scene->mRootNode, identity);
    transforms.resize(m_NumBones);

    for(int i=0; i<m_NumBones; i++)
        transforms[i] = m_BoneInfo[i].finalTransformation;
}



void EG_DynamicModel::readNodeHierarchyTranspose(float animationTime, const aiNode* node, const glm::mat4& parentTransform)
{
    string nodeName(node->mName.data);
    const aiAnimation* animation = m_Scene->mAnimations[0];

    glm::mat4 nodeTransformation = EG_Utility::toGlmMat(node->mTransformation);

    const aiNodeAnim* nodeAnim = findNodeAnim(animation, nodeName);

    if(nodeAnim)
    {
        glm::mat4 scalingMat = computeInterpolatedScalingMatrix(animationTime, nodeAnim);
        glm::mat4 rotationMat = computeInterpolatedRotationMatrix(animationTime, nodeAnim);
        glm::mat4 translationMat = computeInterpolatedPositionMatrix(animationTime, nodeAnim);

        nodeTransformation = scalingMat * rotationMat * translationMat;
    }

    glm::mat4 globalTransformation = nodeTransformation * parentTransform;

    if(m_BoneMapping.find(nodeName) != m_BoneMapping.end())
    {
        unsigned int boneIndex = m_BoneMapping[nodeName];

        m_BoneInfo[boneIndex].finalTransformation = m_BoneInfo[boneIndex].boneOffset * globalTransformation * m_GlobalInverseTransform;
    }

    for (unsigned int i=0; i < node->mNumChildren; i++)
        readNodeHierarchy(animationTime, node->mChildren[i], globalTransformation);
}


glm::mat4 EG_DynamicModel::computeInterpolatedScalingMatrix(float animationTime, const aiNodeAnim* nodeAnim)
{
    int nKeys = nodeAnim->mNumScalingKeys;
    glm::mat4 out;
    if(nKeys == 1)
    {
        glm::vec3 v = EG_Utility::toGlmVec(nodeAnim->mScalingKeys[0].mValue);
        out = glm::scale(v.x, v.y, v.z);
        return out;
    }

    aiVectorKey* keys = nodeAnim->mScalingKeys;

    unsigned int index1 = findScaling(animationTime, nodeAnim);
    unsigned int index2 = index1 + 1;
    assert(index2 < nKeys);

    aiVectorKey key1 = keys[index1];
    aiVectorKey key2 = keys[index2];

    glm::vec3 v = computeInterpolatedVector(animationTime, key2, key1);
    out = glm::scale(v.x, v.y, v.z);
    return out;
}



glm::mat4 EG_DynamicModel::computeInterpolatedRotationMatrix(float animationTime, const aiNodeAnim* nodeAnim)
{
    int nKeys = nodeAnim->mNumRotationKeys;
    glm::mat4 out;
    if(nKeys == 1)
    {
        aiMatrix3x3 m = nodeAnim->mRotationKeys[0].mValue.GetMatrix();
        out = EG_Utility::toGlmMat(m);
        return out;
    }

    aiQuatKey* rotationKeys = nodeAnim->mRotationKeys;

    unsigned int index1 = findRotation(animationTime, nodeAnim);
    unsigned int index2 = index1 + 1;
    assert(index2 < nKeys);

    aiQuatKey key1 = rotationKeys[index1];
    aiQuatKey key2 = rotationKeys[index2];

    aiQuaternion q = computeInterpolatedQuaternion(animationTime, key2, key1);
    aiMatrix3x3 m = q.GetMatrix();
    out = EG_Utility::toGlmMat(m);
    return out;
}




glm::mat4 EG_DynamicModel::computeInterpolatedPositionMatrix(float animationTime, const aiNodeAnim* nodeAnim)
{
    int nKeys = nodeAnim->mNumPositionKeys;
    glm::mat4 out;

    if(nKeys == 1)
    {
        glm::vec3 v = EG_Utility::toGlmVec(nodeAnim->mPositionKeys[0].mValue);
        out = glm::transpose(glm::translate(v.x, v.y, v.z));
        return out;
    }

    aiVectorKey* keys = nodeAnim->mPositionKeys;

    unsigned int index1 = findPosition(animationTime, nodeAnim);
    unsigned int index2 = index1 + 1;
    assert(index2 < nKeys);

    aiVectorKey key1 = keys[index1];
    aiVectorKey key2 = keys[index2];

    glm::vec3 v = computeInterpolatedVector(animationTime, key2, key1);
    out = glm::transpose(glm::translate(v.x, v.y, v.z));
    return out;
}



unsigned int EG_DynamicModel::findPosition(float animationTime, const aiNodeAnim* nodeAnim)
{
    for (unsigned int i = 0 ; i < nodeAnim->mNumPositionKeys - 1 ; i++)
    {
        if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime)
            return i;
    }

    assert(0);
    return 0;
}


unsigned int EG_DynamicModel::findRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
    assert(nodeAnim->mNumRotationKeys > 0);

    for (unsigned int i = 0 ; i < nodeAnim->mNumRotationKeys - 1 ; i++)
    {
        if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime)
            return i;
    }

    assert(0);
    return 0;
}

unsigned int EG_DynamicModel::findScaling(float animationTime, const aiNodeAnim* nodeAnim)
{
    assert(nodeAnim->mNumScalingKeys > 0);

    for (unsigned int i = 0 ; i < nodeAnim->mNumScalingKeys - 1 ; i++)
    {
        if (animationTime < (float)nodeAnim->mScalingKeys[i + 1].mTime)
            return i;
    }

    assert(0);
    return 0;
}


float EG_DynamicModel::computeInterpolationTimeFactor(float animationTime, double t2, double t1)
{
    float deltaTime = (float)(t2 - t1);
    float factor = (animationTime - (float)t1) / deltaTime;
    assert(factor >= 0.0f && 0.0f <= 1.0f);
    return factor;
}


glm::vec3 EG_DynamicModel::computeInterpolatedVector(float animationTime, aiVectorKey& k2, aiVectorKey& k1)
{
    float factor = computeInterpolationTimeFactor(animationTime, k2.mTime, k1.mTime);

    glm::vec3 start = EG_Utility::toGlmVec(k1.mValue);
    glm::vec3 end = EG_Utility::toGlmVec(k2.mValue);
    glm::vec3 delta = end - start;

    glm::vec3 out = start + factor * delta;
    return out;
}


aiQuaternion EG_DynamicModel::computeInterpolatedQuaternion(float animationTime, aiQuatKey& k2, aiQuatKey& k1)
{
    float factor = computeInterpolationTimeFactor(animationTime, k2.mTime, k1.mTime);
    aiQuaternion out;

    const aiQuaternion& quat1 = k1.mValue;
    const aiQuaternion& quat2 = k2.mValue;
    aiQuaternion::Interpolate(out, quat1, quat2, factor);

    out = out.Normalize();
    return out;
}


const aiNodeAnim* EG_DynamicModel::findNodeAnim(const aiAnimation* animation, const string nodeName)
{
    for(unsigned int i=0; i< animation->mNumChannels; i++)
    {
        const aiNodeAnim* nodeAnim = animation->mChannels[i];
        if( string(nodeAnim->mNodeName.data) == nodeName )
            return nodeAnim;
    }
    return NULL;
}




/*
void EG_DynamicModel::render()
{
    EG_Model::render();
}
*/
