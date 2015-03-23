#ifndef MESH38_H
#define	MESH38_H


#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "define.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "texture.h"
#include "math_3d.h"

using namespace std;


struct VertexData38
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;
    glm::vec3 m_color;

    VertexData38() {}

    VertexData38(const glm::vec3& pos,
                 const glm::vec2& tex,
                 const glm::vec3& normal,
                 const glm::vec3& color )
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = normal;
        m_color = color;
    }

//    VertexBoneData Bones;
};


class Mesh38
{

    public:
        Mesh38();

        ~Mesh38();

        bool LoadMesh(const std::string& Filename);
        void Render();
   //     void Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);


        unsigned int NumBones() const
        {
            return m_NumBones;
        }
        void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);

        void BoneTransform(float TimeInSeconds, vector<Matrix4f>& Transforms);




     private:
        #define NUM_BONES_PER_VEREX 4


        struct BoneInfo
        {
            Matrix4f BoneOffset;
            Matrix4f FinalTransformation;

            BoneInfo()
            {
                BoneOffset.SetZero();
                FinalTransformation.SetZero();
            }
        };

        struct VertexBoneData
        {
            unsigned int IDs[NUM_BONES_PER_VEREX];
            float Weights[NUM_BONES_PER_VEREX];

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
                for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++)
                {
                    if (Weights[i] == 0.0)
                    {
                        IDs[i]     = BoneID;
                        Weights[i] = Weight;
                        return;
                    }
                }
                // should never get here - more bones than we have space for
                assert(0);
            }
        };






    private:



        void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
        unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
        unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
        const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
        void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);
        bool InitFromScene(const aiScene* pScene, const std::string& Filename);
        void InitMesh(unsigned int MeshIndex,
                      const aiMesh* paiMesh,
                      vector<Vector3f>& Positions,
                      vector<Vector3f>& Normals,
                      vector<Vector2f>& TexCoords,
                      vector<VertexBoneData>& Bones,
                      vector<unsigned int>& Indices);

        void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
        bool InitMaterials(const aiScene* pScene, const std::string& Filename);
        void Clear();


        void AssignBoneOffset(glm::mat4& m, const aiMesh* pMesh, int i);
        void CopyAiMatrixToGlmMat1(glm::mat4& m1, aiMatrix4x4 m2);
        void CopyAiMatrixToGlmMat(glm::mat4& m1, aiMatrix3x3 m2);




        void CheckMatrix(glm::mat4 m1);
#define INVALID_MATERIAL 0xFFFFFFFF


    enum VB_TYPES {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        BONE_VB,
        NUM_VBs
    };

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

        struct MeshEntry
        {
            MeshEntry()
            {
                NumIndices = 0;
                BaseVertex = 0;
                BaseIndex = 0;
                MaterialIndex = INVALID_MATERIAL;
            }

            unsigned int NumIndices;
            unsigned int BaseVertex;
            unsigned int BaseIndex;
            unsigned int MaterialIndex;
        };

        vector<MeshEntry> m_Entries;
        vector<Texture*>  m_Textures;

        map<string, unsigned int> m_BoneMapping;
        unsigned int m_NumBones;
        vector<BoneInfo> m_BoneInfo;

        Matrix4f m_GlobalInverseTransform;

        const aiScene* m_pScene;
      //  Assimp::Importer m_Importer;
};











/*
struct VertexData38
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;
    glm::vec3 m_color;

    VertexData38() {}

    VertexData38(const glm::vec3& pos,
                 const glm::vec2& tex,
                 const glm::vec3& normal,
                 const glm::vec3& color )
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = normal;
        m_color = color;
    }

//    VertexBoneData Bones;
};


class Mesh38
{
     private:
        #define NUM_BONES_PER_VEREX 4

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
            unsigned int IDs[NUM_BONES_PER_VEREX];
            float Weights[NUM_BONES_PER_VEREX];

            void AddBoneData(unsigned int BoneID, float Weight)
            {
                for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++)
                {
                    if (Weights[i] == 0.0)
                    {
                        IDs[i]     = BoneID;
                        Weights[i] = Weight;
                        return;
                    }
                }
                // should never get here - more bones than we have space for
                assert(0);
            }

            VertexBoneData()
            {
                Reset();
            };

            void Reset()
            {
                ZERO_MEM(IDs);
                ZERO_MEM(Weights);
            }
        };



    public:
        Mesh38();

        ~Mesh38();

        bool LoadMesh(const std::string& Filename);
        void Render();
   //     void Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);


        unsigned int NumBones() const
        {
            return m_NumBones;
        }
        void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);





    private:
        bool InitFromScene(const aiScene* pScene, const std::string& Filename);
        void InitMesh(const aiMesh* paiMesh,
                      std::vector<glm::vec3>& Positions,
                      std::vector<glm::vec3>& Normals,
                      std::vector<glm::vec2>& TexCoords,
                      std::vector<unsigned int>& Indices);

        void InitMesh(unsigned int MeshIndex,
                    const aiMesh* paiMesh,
                    std::vector<glm::vec3>& Positions,
                    std::vector<glm::vec3>& Normals,
                    std::vector<glm::vec2>& TexCoords,
                    vector<VertexBoneData>& Bones,
                    vector<unsigned int>& Indices);

        void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
        bool InitMaterials(const aiScene* pScene, const std::string& Filename);
        void Clear();

        void AssignBoneOffset(glm::mat4& m, const aiMesh* pMesh, int i);

        void CopyAiMatrixToGlmMat1(glm::mat4& m1, aiMatrix4x4 m2);
        void CopyAiMatrixToGlmMat(glm::mat4& m1, aiMatrix3x3 m2);
        void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);

        void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
        unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
        unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
        const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);

        void CheckMatrix(glm::mat4 m1);
#define INVALID_MATERIAL 0xFFFFFFFF


    enum VB_TYPES {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        BONE_VB,
        NUM_VBs
    };

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

        struct MeshEntry
        {
            MeshEntry()
            {
                NumIndices = 0;
                BaseVertex = 0;
                BaseIndex = 0;
                MaterialIndex = INVALID_MATERIAL;
            }

            unsigned int NumIndices;
            unsigned int BaseVertex;
            unsigned int BaseIndex;
            unsigned int MaterialIndex;
        };

        vector<MeshEntry> m_Entries;
        vector<Texture*>  m_Textures;

        map<string, unsigned int> m_BoneMapping;
        unsigned int m_NumBones;
        vector<BoneInfo> m_BoneInfo;

        glm::mat4 m_GlobalInverseTransform;

        const aiScene* m_pScene;
      //  Assimp::Importer m_Importer;
};
*/
#endif // MESH38_H
