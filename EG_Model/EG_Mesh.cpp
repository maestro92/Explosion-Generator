#include "EG_Mesh.h"


EG_Mesh::EG_Mesh(vector<EG_VertexData>& Vertices, vector<unsigned int>& Indices)
{
    m_numIndices = Indices.size();

    glGenBuffers(1, &m_vbo);
  	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(EG_VertexData) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_ind);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ind);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numIndices, &Indices[0], GL_STATIC_DRAW);

    /// we unbind the buffers, so that no one accidentally unbind the buffers
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}



EG_Mesh::EG_Mesh(vector<EG_VertexData>& Vertices, vector<unsigned int>& Indices, vector<EG_TextureData>& textures)
{
    m_numIndices = Indices.size();
   // genGLVertexBuffer(VB, Vertices);
   // genGLIndexBuffer(IB, Indices);

    m_textures = textures;

    glGenBuffers(1, &m_vbo);
  	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(EG_VertexData) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_ind);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ind);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numIndices, &Indices[0], GL_STATIC_DRAW);

    /// we unbind the buffers, so that no one accidentally unbind the buffers
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}



