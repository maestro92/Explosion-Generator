
#include "EG_XYZAxisModel.h"

EG_XYZAxisModel::EG_XYZAxisModel()
{
    m_modelGeometry = GL_LINES;
}

EG_XYZAxisModel::~EG_XYZAxisModel()
{
}

void EG_XYZAxisModel::init()
{


    std::vector<unsigned int> indices;
    std::vector<EG_VertexData> vertices;
    EG_VertexData v;

    float scale = 50;

        /// X axis
    v.m_position = glm::vec3(0.0,0.0,0.0);
    v.m_color = glm::vec3(1.0,0.0,0.0);
    vertices.push_back(v);

    v.m_position = glm::vec3(scale,0.0,0.0);
    v.m_color = glm::vec3(1.0,0.0,0.0);
    vertices.push_back(v);

    /// Y axis
    v.m_position = glm::vec3(0.0,0.0,0.0);
    v.m_color = glm::vec3(0.0,1.0,0.0);
    vertices.push_back(v);;

    v.m_position = glm::vec3(0.0,scale,0.0);
    v.m_color = glm::vec3(0.0,1.0,0.0);
    vertices.push_back(v);;

    /// Z axis
    v.m_position = glm::vec3(0.0,0.0,0.0);
    v.m_color = glm::vec3(0.0,0.0,1.0);
    vertices.push_back(v);;

    v.m_position = glm::vec3(0.0,0.0,scale);
    v.m_color = glm::vec3(0.0,0.0,1.0);
    vertices.push_back(v);;


    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);

 //   worldAxis = new mesh(&axisVertices, &axisIndices);
    EG_Mesh m(vertices, indices);
    m_meshes.push_back(m);
}
