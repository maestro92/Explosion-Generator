#include "EG_QuadModelABS.h"


EG_QuadModelABS::EG_QuadModelABS()
{}

EG_QuadModelABS::~EG_QuadModelABS()
{}



EG_QuadModelABS::EG_QuadModelABS(int i)
{
    init();
}



void EG_QuadModelABS::initIndices(vector<unsigned int>& indices)
{
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
}

void EG_QuadModelABS::init()
{
    init(1, 1, 0, 0, 1);
}



void EG_QuadModelABS::init(int w, int h)
{
    init(w, h, 0, 0, 1);
}





void EG_QuadModelABS::init(int w, int h, float cx, float cy, float cw)
{
    /// assigning a quad for rendering textures
    std::vector<unsigned int> indices;
    std::vector<EG_VertexData> vertices;
    EG_VertexData tmp;

    float cx1 = cx;  float cx2 = cx+1;
    float cy1 = cy;  float cy2 = cy+1;
    //1.
    tmp.m_position = glm::vec3(0.0, h, 0.0);
    tmp.m_UV = glm::vec2(cx1 * cw, cy1 * cw);
    vertices.push_back(tmp);
    //2.
    tmp.m_position = glm::vec3(0.0, 0.0, 0.0);
    tmp.m_UV = glm::vec2(cx1 * cw, cy2 * cw);
    vertices.push_back(tmp);

    //3.
    tmp.m_position = glm::vec3(w, 0.0, 0.0);
    tmp.m_UV = glm::vec2(cx2 * cw, cy2 * cw);
    vertices.push_back(tmp);
    //4.
    tmp.m_position = glm::vec3(w, h, 0.0);
    tmp.m_UV = glm::vec2(cx2 * cw, cy1 * cw);
    vertices.push_back(tmp);

    initIndices(indices);

    EG_Mesh m(vertices, indices);
    m_meshes.push_back(m);
}


void EG_QuadModelABS::init(int w, int h, glm::vec3 c)
{

    init(w, h, c, c, c, c);
}


void EG_QuadModelABS::init(int w, int h, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4)
{
    /// assigning a quad for rendering textures


    std::vector<unsigned int> indices;
    std::vector<EG_VertexData> vertices;
    EG_VertexData tmp;

    //1.
    tmp.m_position = glm::vec3(0.0, h, 0.0);
    tmp.m_color = c1;
    vertices.push_back(tmp);
    //2.
    tmp.m_position = glm::vec3(0.0, 0.0, 0.0);
    tmp.m_color = c2;
    vertices.push_back(tmp);

    //3.
    tmp.m_position = glm::vec3(w, 0.0, 0.0);
    tmp.m_color = c3;
    vertices.push_back(tmp);
    //4.
    tmp.m_position = glm::vec3(w, h, 0.0);
    tmp.m_color = c4;
    vertices.push_back(tmp);

    initIndices(indices);

    EG_Mesh m(vertices, indices);
    m_meshes.push_back(m);
}



void EG_QuadModelABS::initCentered()
{
    init(-0.5, 0.5, -0.5, 0.5,
         0.0, 0.0, 1.0);
}


void EG_QuadModelABS::init(float l, float r,
                           float b, float t,
                           float cx, float cy, float cw)
{
/// assigning a quad for rendering textures
    m_modelGeometry = GL_TRIANGLES;

    std::vector<unsigned int> indices;
    std::vector<EG_VertexData> vertices;
    EG_VertexData tmp;

    float cx1 = cx;  float cx2 = cx+1;
    float cy1 = cy;  float cy2 = cy+1;

    ///1. left-top
    tmp.m_position = glm::vec3(l, t, 0.0);
    tmp.m_UV = glm::vec2(cx1 * cw, cy1 * cw);
    vertices.push_back(tmp);

    ///2. left-bottom
    tmp.m_position = glm::vec3(l, b, 0.0);
    tmp.m_UV = glm::vec2(cx1 * cw, cy2 * cw);
    vertices.push_back(tmp);

    ///3. right-bottom
    tmp.m_position = glm::vec3(r, b, 0.0);
    tmp.m_UV = glm::vec2(cx2 * cw, cy2 * cw);
    vertices.push_back(tmp);

    ///4. right-top
    tmp.m_position = glm::vec3(r, t, 0.0);
    tmp.m_UV = glm::vec2(cx2 * cw, cy1 * cw);
    vertices.push_back(tmp);

    initIndices(indices);

    EG_Mesh m(vertices, indices);
    m_meshes.push_back(m);
}



