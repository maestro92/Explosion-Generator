
#include "EG_XYZAxis.h"

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


void EG_WorldAxis::renderSingle(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID)
{
    RenderTechnique->enableShader(RenderPassID);
    render(m_pipeline, RenderTechnique, RenderPassID);
    RenderTechnique->disableShader(RenderPassID);
}


void EG_WorldAxis::render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID)
{
    m_pipeline.pushMatrix();

        m_pipeline.translate(m_position);
        m_pipeline.scale(m_scale);
        RenderTechnique->loadUniformLocations(m_pipeline, RENDER_PASS1);
        worldAxis->drawLines();

    m_pipeline.popMatrix();
}
/*
#include "EG_WorldXYZAxis.h"

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


void EG_WorldAxis::renderSingle(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID)
{
    RenderTechnique->enableShader(RenderPassID);
    render(m_pipeline, RenderTechnique, RenderPassID);
    RenderTechnique->disableShader(RenderPassID);
}


void EG_WorldAxis::render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID)
{
    m_pipeline.pushMatrix();

        m_pipeline.translate(m_position);
        m_pipeline.scale(m_scale);
        RenderTechnique->loadUniformLocations(m_pipeline, RENDER_PASS1);
        worldAxis->drawLines();

    m_pipeline.popMatrix();
}
*/
