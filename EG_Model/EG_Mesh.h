#ifndef EG_MESH_H_
#define EG_MESH_H_

#include "EG_VertexData.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

#include <vector>
#include <string>

using namespace std;

struct EG_Mesh
{
    vector<EG_TextureData> m_textures;


    GLuint m_vbo;
    GLuint m_ind;
    int m_numIndices;

    // EG_Mesh();
    EG_Mesh(vector<EG_VertexData>& Vertices, vector<unsigned int>& Indices);
    EG_Mesh(vector<EG_VertexData>& Vertices, vector<unsigned int>& Indices, vector<EG_TextureData>& textures);
};



#endif // EG_MESH_H_
