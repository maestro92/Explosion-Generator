#ifndef MESH37_H
#define	MESH37_H


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

using namespace std;

struct VertexData37
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;
    glm::vec3 m_color;

    VertexData37() {}

    VertexData37(const glm::vec3& pos,
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


class Mesh
{
    public:
        Mesh();

        ~Mesh();

        bool LoadMesh(const std::string& Filename);
        void Render();
        void Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);

    private:
        bool InitFromAiScene(const aiScene* pScene, const std::string& Filename);
        void InitMesh(const aiMesh* paiMesh,
                      std::vector<glm::vec3>& Positions,
                      std::vector<glm::vec3>& Normals,
                      std::vector<glm::vec2>& TexCoords,
                      std::vector<unsigned int>& Indices);

        bool InitMaterials(const aiScene* pScene, const std::string& Filename);
        void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
/*
#define INDEX_BUFFER 0
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3
#define WVP_MAT_VB   4
#define WORLD_MAT_VB 5
*/
    enum VB_TYPES {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        WVP_MAT_VB,
        WORLD_MAT_VB
    };

        /// instead of having VB/IB (Vertex Buffer and Index Buffer) Objects, we
        /// have four buffers - index, position, normal and texture coordinates
        /// also a new member variable called m_VAO that stores the vertex array object
        ///
        GLuint m_VAO;
        GLuint m_Buffers[6];

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
};

#endif // MESH37_H
