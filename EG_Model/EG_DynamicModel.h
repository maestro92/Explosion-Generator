#ifndef EG_DYNAMIC_MODEL_H
#define	EG_DYNAMIC_MODEL_H

#include "EG_Model.h"
#include "define.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#define NUM_BONES_PER_VERTEX 4

struct BoneInfo
{
    glm::mat4 BoneOffset;
    glm::mat4 FinalTransformation;

    BoneInfo()
    {
        BoneOffset = glm::mat4(0.0f);
        FinalTransformation = glm::mat4(0.0f);
    }
};


struct VertexBoneData
{
    unsigned int IDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];

    VertexBoneData()
    {
        Reset();
    };

    void Reset()
    {
        ZERO_MEM(IDs);
        ZERO_MEM(Weights);
    }

    void AddBoneData(unsigned int BoneID, float Weight)
    {
        for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++)
        {
            if (Weights[i] == 0.0)
            {
                IDs[i]      = BoneID;
                Weights[i]  = Weight;
                return;
            }
        }
        assert(0);
    }
};


class EG_DynamicModel : public EG_Model
{
    public:
        EG_DynamicModel();
        ~EG_DynamicModel();

        bool loadModel(string filename);
        void render();



        void BoneTransform(float TimeInSeconds, vector<glm::mat4> Transforms);

    private:

        void computeInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void computeInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void computeInterpolatedPosition(aiVector3D&, float AnimationTime, const aiNodeAnim* pNodeAnim);

        unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
        unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
        unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

        const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const string NodeName);
        void readNodeHierarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
        bool initFromAiScene(const aiScene* pScene, const std::string& Filename);

        void initMesh(const aiMesh* m, const aiScene* s,
                      vector<glm::vec3>& Positions,
                      vector<glm::vec3>& Normals,
                      vector<glm::vec3>& Tangents,
                      vector<glm::vec3>& Colors,
                      vector<glm::vec2>& UVs,
                      vector<VertexBoneData>& Bones,
                      vector<unsigned int>& Indices);

        void loadBones(unsigned int MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
        bool initMaterials(const aiScene* pScene, const std::string& Filename);



        map<string, unsigned int> m_BoneMapping;
        unsigned int m_NumBones;
        vector<BoneInfo> m_BoneInfo;

        glm::mat4 m_GlobalInverseTransform;
};

#endif
