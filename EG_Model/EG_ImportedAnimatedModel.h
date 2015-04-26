#ifndef EG_IMPORTED_ANIMATED_MODEL_H
#define	EG_IMPORTED_ANIMATED_MODEL_H

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
    glm::mat4 boneOffset;
    glm::mat4 finalTransformation;

    BoneInfo()
    {
        boneOffset = glm::mat4(0.0f);
        finalTransformation = glm::mat4(0.0f);
    }
};

/// The Bone Influence information for each Vertex
struct VertexBoneData
{
    /// number of bones that will influence this Vertex
    /// Bone IDs are indices into an array of bone transformations
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



    /// called in initMesh->loadBones
    /// it finds a free slot in the IDs and Weights and adds the bone in it
    /// it's initialized to zero, therefore it finds a free slot by
    /// going through "if(Weights[i] == 0.0)"

    /// BoneID is the index of the bone in m_BoneInfo
    /// so we can get the transformation information from there
    void AddBoneData(unsigned int BoneID, float Weight)
    {
/*
        cout << "BoneID" << BoneID << endl;

        for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++)
        {
            cout << Weights[i] << " ";
        }
        cout << endl;
*/

        for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++)
        {
            if (Weights[i] < 0.00001)
            {
                IDs[i]      = BoneID;
                Weights[i]  = Weight;
                return;
            }
        }
    //    cout << "here" << endl;

        assert(0);
    }
};





class EG_ImportedAnimatedModel : public EG_Model
{
    public:
        EG_ImportedAnimatedModel();
        ~EG_ImportedAnimatedModel();

        bool loadModel(string filename);
//        void render();

        void transferDataToBuffer(vector<VertexBoneData>& vec, unsigned int bufferIndex, unsigned int location);

        void boneTransform(float timeInSeconds);
        void boneTransform(float timeInSeconds, vector<glm::mat4>& transforms);
        void boneTransformTranspose(float timeInSeconds, vector<glm::mat4>& transforms);
    private:
        float computeInterpolationTimeFactor(float animationTime, double t2, double t1);
        glm::vec3 computeInterpolatedVector(float animationTime, aiVectorKey& k2, aiVectorKey& k1);
        aiQuaternion computeInterpolatedQuaternion(float animationTime, aiQuatKey& k2, aiQuatKey& k1);



        glm::mat4 computeInterpolatedScalingMatrix(float animationTime, const aiNodeAnim* nodeAnim);
        glm::mat4 computeInterpolatedRotationMatrix(float animationTime, const aiNodeAnim* nodeAnim);
        glm::mat4 computeInterpolatedPositionMatrix(float animationTime, const aiNodeAnim* nodeAnim);

        unsigned int findScaling(float animationTime, const aiNodeAnim* nodeAnim);
        unsigned int findRotation(float animationTime, const aiNodeAnim* nodeAnim);
        unsigned int findPosition(float animationTime, const aiNodeAnim* nodeAnim);

        const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const string NodeName);

        void readNodeHierarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
        void readNodeHierarchyTranspose(float animationTime, const aiNode* node, const glm::mat4& parentTransform);



        bool initFromAiScene(const aiScene* pScene, const std::string& Filename);

        void initMesh(unsigned int MeshIndex, const aiMesh* m, const aiScene* s,
                      vector<glm::vec3>& Positions,
                      vector<glm::vec3>& Normals,
                      vector<glm::vec3>& Tangents,
                      vector<glm::vec3>& Colors,
                      vector<glm::vec2>& UVs,
                      vector<unsigned int>& Indices,
                      vector<VertexBoneData>& Bones);


   //     void loadBones(unsigned int MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
        void loadBones(unsigned int MeshIndex, const aiMesh* pMesh, vector<VertexBoneData>& Bones);

        bool initMaterials(const aiScene* pScene, const std::string& Filename);




        /// Total Number of bones from all of the meshes
        unsigned int m_NumBones;

        /// this keeps track of whether if we have a new incoming bone or an already existing bone
        /// once we keep track of all the bones, in the boneTransform function, whenever we have node
        /// we check if this node corresponds to a bone. If so, we store the final transformation in m_BoneInfo vector
        map<string, unsigned int> m_BoneMapping;

        /// the vector that holds all the each bone's transformation
        vector<BoneInfo> m_BoneInfo;

        vector<glm::mat4> m_boneTransforms;

        float m_TicksPerSecond;
        float m_AnimFrameDuration;
//        float m_transforms;


        glm::mat4 m_GlobalInverseTransform;
};

#endif
