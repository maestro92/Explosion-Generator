
#include <assert.h>
#include "EG_ModelABS.h"

using namespace std;



EG_ModelABS::EG_ModelABS()
{
    m_modelGeometry = GL_TRIANGLES;
}







EG_ModelABS::~EG_ModelABS()
{
    clear();
}


void EG_ModelABS::clear()
{
 //   for (unsigned int i=0; i<m_Textures.size(); i++)
  //      SAFE_DELETE(m_Textures[i]);

}


void EG_ModelABS::addMesh(EG_Mesh m)
{
    m_meshes.push_back(m);
}

void EG_ModelABS::init()
{
    m_modelGeometry = GL_TRIANGLES;
}

void EG_ModelABS::render()
{
    glEnableVertexAttribArray(POSITION);
    glEnableVertexAttribArray(NORMAL);
    glEnableVertexAttribArray(COLOR);
    glEnableVertexAttribArray(UV);

    /// this means we're going to use these two buffer object
    for (int i=0; i<m_meshes.size(); i++)
    {
        EG_Mesh m = m_meshes[i];

        string str="texture";
        for(int i=0;i<m.m_textures.size();i++)
        {
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D, m.m_textures[i].m_id);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m.m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.m_ind);

        glVertexAttribPointer(POSITION,   3, GL_FLOAT, GL_FALSE, sizeof(EG_VertexData), 0);
        glVertexAttribPointer(NORMAL,   3, GL_FLOAT, GL_FALSE, sizeof(EG_VertexData), (void*)(3*sizeof(float)));
        glVertexAttribPointer(COLOR,    3, GL_FLOAT, GL_FALSE, sizeof(EG_VertexData), (void*)(6*sizeof(float)));
        glVertexAttribPointer(UV,       2, GL_FLOAT, GL_FALSE, sizeof(EG_VertexData), (void*)(9*sizeof(float)));

        /// glDrawElements, you need to supply an index buffer
        /// glDrawArrays submits the vertices in linear order
//        glDrawElements(GL_TRIANGLES, m.m_numIndices, GL_UNSIGNED_INT, 0);
        glDrawElements(m_modelGeometry, m.m_numIndices, GL_UNSIGNED_INT, 0);
    }


    glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

//	glDisableVertexAttribArray(POSITION);
	glDisableVertexAttribArray(NORMAL);
	glDisableVertexAttribArray(COLOR);
	glDisableVertexAttribArray(UV);

}

