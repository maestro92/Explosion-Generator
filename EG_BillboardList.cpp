#include "EG_BillboardList.h"

#define NUM_ROWS 10
#define NUM_COLUMNS 10

EG_BillboardList::EG_BillboardList()
{

}


EG_BillboardList::~EG_BillboardList()
{

}



void EG_BillboardList::init(string tex)
{
    m_billboardTexture = EG_Utility::loadTexture(tex);
    createPositionBuffer();
}

/*
void EG_BillboardList::render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_billboardTexture);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // position

    glDrawArrays(GL_POINTS, 0, NUM_ROWS * NUM_COLUMNS);

    glDisableVertexAttribArray(0);
}
*/

void EG_BillboardList::render(pipeline& p, EG_Renderer* Renderer, int pass)
{
    glDisable(GL_CULL_FACE);
    Renderer->enableShader(pass);
    p.pushMatrix();

//        p.scale(20.0f,20.0f,1.0f);
//        p.scale(20.0f);
   //     p.translate(0,5,0);
/*
        glm::mat4 mvp = m_pipeline.getModelViewProjectionMatrix();
        r_billBoardRenderer.setViewProjectionMatrix(mvp);
        r_billBoardRenderer.setCameraPosition(m_orbitCamera.getEyePoint());
*/
        Renderer->loadUniformLocations(p, pass);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_billboardTexture);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // position

        glDrawArrays(GL_POINTS, 0, NUM_ROWS * NUM_COLUMNS);

        glDisableVertexAttribArray(0);

    p.popMatrix();
    Renderer->disableShader(pass);
}


void EG_BillboardList::createPositionBuffer()
{
    glm::vec3 Positions[NUM_ROWS * NUM_COLUMNS];

    for (unsigned int j = 0 ; j < NUM_ROWS ; j++)
    {
        for (unsigned int i = 0 ; i < NUM_COLUMNS ; i++)
        {
            glm::vec3 Pos((float)i, 0.0f, (float)j);
            Positions[j * NUM_COLUMNS + i] = Pos;
        }
    }

    glGenBuffers(1, &m_VB);
  	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions), &Positions[0], GL_STATIC_DRAW);
}
