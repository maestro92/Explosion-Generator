#ifndef EG_WORLD_AXIS_H_
#define EG_WORLD_AXIS_H_

#include "EG_WorldAxis.h"

EG_WorldAxis::EG_WorldAxis()
{
}

EG_WorldAxis::~EG_WorldAxis()
{
    delete worldAxis;
    worldAxis = NULL;
}

void EG_WorldAxis::init()
{
    std::vector<unsigned int> axisIndices;
    std::vector<vertexData> axisVertices;
    vertexData axisVertex;

    /// X axis
    axisVertex.position = glm::vec3(0.0,0.0,0.0);
    axisVertex.normal = glm::vec3(0.0,1.0,0.0);
    axisVertex.color = glm::vec3(1.0,0.0,0.0);
    axisVertices.push_back(axisVertex);

    axisVertex.position = glm::vec3(100.0,0.0,0.0);
    axisVertex.normal = glm::vec3(0.0,1.0,0.0);
    axisVertex.color = glm::vec3(1.0,0.0,0.0);
    axisVertices.push_back(axisVertex);

    /// Y axis
    axisVertex.position = glm::vec3(0.0,0.0,0.0);
    axisVertex.normal = glm::vec3(0.0,1.0,0.0);
    axisVertex.color = glm::vec3(0.0,1.0,0.0);
    axisVertices.push_back(axisVertex);

    axisVertex.position = glm::vec3(0.0,100.0,0.0);
    axisVertex.normal = glm::vec3(0.0,1.0,0.0);
    axisVertex.color = glm::vec3(0.0,1.0,0.0);
    axisVertices.push_back(axisVertex);

    /// Z axis
    axisVertex.position = glm::vec3(0.0,0.0,0.0);
    axisVertex.normal = glm::vec3(0.0,1.0,0.0);
    axisVertex.color = glm::vec3(0.0,0.0,1.0);
    axisVertices.push_back(axisVertex);

    axisVertex.position = glm::vec3(0.0,0.0,100.0);
    axisVertex.normal = glm::vec3(0.0,1.0,0.0);
    axisVertex.color = glm::vec3(0.0,0.0,1.0);
    axisVertices.push_back(axisVertex);


    axisIndices.push_back(0);
    axisIndices.push_back(1);
    axisIndices.push_back(2);
    axisIndices.push_back(3);
    axisIndices.push_back(4);
    axisIndices.push_back(5);

    worldAxis = new mesh(&axisVertices, &axisIndices);

}




void EG_WorldAxis::render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID)
{
    RenderTechnique->enableShader(RenderPassID);
    m_pipeline.pushMatrix();

        m_pipeline.translate(position);
        RenderTechnique->loadUniformLocations(m_pipeline, RENDER_PASS1);
        worldAxis->drawLines();

    m_pipeline.popMatrix();
    RenderTechnique->disableShader(RenderPassID);
}

/*
void EG_WorldAxis::render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID)
{
    m_pipeline.pushMatrix();
        RenderTechnique->loadUniformLocations(m_pipeline, RENDER_PASS1);
        worldAxis->drawLines();
    m_pipeline.popMatrix();
}
*/


/*

void ExplosionGenerator::drawAxis(float fScale, glm::vec3 translate)
{
    glPushAttrib( GL_ENABLE_BIT );

 //   glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    glPushMatrix();
        glTranslatef( translate.x, translate.y, translate.z );
        glScalef( fScale, fScale, fScale );
        glColor3f( 0.0f, 0.0f, 1.0f );

        glBegin( GL_LINES );
        {
            // x axis: red
            glColor3f( 1.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 0.0 );
            glVertex3f( 1.0f, 0.0f, 0.0f );

            // y axis: green
            glColor3f( 0.0f, 1.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 1.0f, 0.0f );

            // zaxis: blue
            glColor3f( 0.0f, 0.0f, 1.0f );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 1.0f );
        }
        glEnd();

    glPopMatrix();
    glPopAttrib();

}


void ExplosionGenerator::drawAxis(float fScale, pipeline& m_pipeline, glm::vec3 translate)
{
 //   glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    m_pipeline.pushMatrix();
    glPushMatrix();

        glTranslatef( translate.x, translate.y, translate.z );
        glScalef( fScale, fScale, fScale );
        glColor3f( 0.0f, 0.0f, 1.0f );

        m_pipeline.matrixMode(MODEL_MATRIX);
        m_pipeline.translate(translate.x, translate.y, translate.z);
        m_pipeline.scale(fScale);
   //     r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        glBegin( GL_LINES );
        {
            // x axis: red
            glColor3f( 1.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 0.0 );
            glVertex3f( 1.0f, 0.0f, 0.0f );

            // y axis: green
            glColor3f( 0.0f, 1.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 1.0f, 0.0f );

            // zaxis: blue
            glColor3f( 0.0f, 0.0f, 1.0f );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 1.0f );
        }
        glEnd();

    glPopMatrix();
    m_pipeline.popMatrix();
    glEnable(GL_LIGHTING);
//   glPopAttrib();
}


void ExplosionGenerator::drawCubeFrame(float size, int offset = 0)
{
    glPushMatrix();
        glDisable( GL_LIGHTING );
        glBegin(GL_LINES);
            // front face
            glColor3f(1.0,0.0,1.0);
            glVertex3f(size/2, size/2 +offset, size/2);
            glVertex3f(-size/2, size/2 +offset, size/2);

            glVertex3f(-size/2, -size/2 +offset, size/2);
            glVertex3f(size/2, -size/2 +offset, size/2);

            glVertex3f(-size/2, size/2 +offset, size/2);
            glVertex3f(-size/2, -size/2 +offset, size/2);

            glVertex3f(size/2, size/2 +offset, size/2);
            glVertex3f(size/2, -size/2 +offset, size/2);


            // back face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, size/2 +offset, -size/2);
            glVertex3f(-size/2, size/2+offset, -size/2);

            glVertex3f(-size/2, -size/2+offset, -size/2);
            glVertex3f(size/2, -size/2+offset, -size/2);

            glVertex3f(size/2, size/2 +offset, -size/2);
            glVertex3f(size/2, -size/2+offset, -size/2);

            glVertex3f(-size/2, size/2+offset, -size/2);
            glVertex3f(-size/2, -size/2+offset, -size/2);

            // left face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(-size/2, size/2+offset, size/2);
            glVertex3f(-size/2, size/2+offset, -size/2);
            glVertex3f(-size/2, -size/2+offset, -size/2);
            glVertex3f(-size/2, -size/2+offset, size/2);

            // right face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, size/2+offset, -size/2);
            glVertex3f(size/2, size/2+offset, size/2);
            glVertex3f(size/2, -size/2+offset, size/2);
            glVertex3f(size/2, -size/2+offset, -size/2);

        glEnd();
        glEnable( GL_LIGHTING );
    glPopMatrix();
}


void ExplosionGenerator::drawCube(float size)
{
    glBegin(GL_QUADS);
        // OpenGL use normal vectors to calculate light
        // if we don't ENABLE(COLOR_MATERIAL)

        // we can manually create a material color here
//       glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, difamb);

        // front face
        glNormal3f(0.0,0.0,1.0);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, size/2);
        glVertex3f(-size/2, -size/2, size/2);
        glVertex3f(size/2, -size/2, size/2);

        // back face
        glNormal3f(0.0,0.0,-1.0);
        glColor3f(0.0,0.0,1.0);
        glVertex3f(size/2, size/2, -size/2);
        glVertex3f(-size/2, size/2, -size/2);
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(size/2, -size/2, -size/2);

        // left face
        glNormal3f(-1.0,0.0,0.0);
        glColor3f(0.0,1.0,0.0);
        glVertex3f(-size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, -size/2);
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(-size/2, -size/2, size/2);

        // right face
        glNormal3f(1.0,0.0,0.0);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(size/2, size/2, -size/2);
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(size/2, -size/2, size/2);
        glVertex3f(size/2, -size/2, -size/2);

        // top face
        glNormal3f(0.0,1.0,0.0);
        glColor3f(1.0,0.0,1.0);
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, -size/2);
        glVertex3f(size/2, size/2, -size/2);

        // bottom face
        glNormal3f(0.0,-1.0,0.0);
        glVertex3f(size/2, -size/2, size/2);
        glVertex3f(-size/2, -size/2, size/2);
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(size/2, -size/2, -size/2);
    glEnd();
}

void ExplosionGenerator::drawGround(float size, unsigned int textureId)
{
    glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDisable( GL_LIGHTING );
    glBegin(GL_QUADS);

        glNormal3f(0.0,1.0,0.0);
        glColor3f(1.0,1.0,1.0);
        glTexCoord2f(0.0,2.0);
        glVertex3f(size/2, 0, size/2);

        glTexCoord2f(0.0,0.0);
        glVertex3f(-size/2, 0, size/2);

        glTexCoord2f(2.0,0.0);
        glVertex3f(-size/2, 0, -size/2);

        glTexCoord2f(2.0,2.0);
        glVertex3f(size/2, 0, -size/2);
    glEnd();
    glEnable( GL_LIGHTING );
    glBindTexture(GL_TEXTURE_2D,0);
    glEnable(GL_CULL_FACE);
}

*/

#endif
