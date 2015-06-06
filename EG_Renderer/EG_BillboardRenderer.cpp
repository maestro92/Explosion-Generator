#include "EG_BillboardRenderer.h"

EG_BillboardRenderer::EG_BillboardRenderer()
{

}


EG_BillboardRenderer::~EG_BillboardRenderer()
{

}


void EG_BillboardRenderer::init(int nShaders)
{
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("EG_BillboardingShader.vs", "EG_BillboardingShader.gs", "EG_BillboardingShader.fs");

    m_modelViewProjectionMatrixUniLoc = getUniLoc(m_shaders[RENDER_PASS1], "gModelViewProjectionMatrix");
    m_cameraPositionUniLoc = getUniLoc(m_shaders[RENDER_PASS1], "gCameraPosition");
    m_cameraViewDirectionUniLoc = getUniLoc(m_shaders[RENDER_PASS1], "gCameraViewDir");
//    m_textureUniLoc = getUniformLocation(m_shaders[RENDER_PASS1], "gTexture");

    initMemberVariables();
}


void EG_BillboardRenderer::setModelViewProjectionMatrix(glm::mat4 vpMat)
{
    m_modelViewProjectionMatrix = vpMat;
}


void EG_BillboardRenderer::setCameraPosition(glm::vec3 camPos)
{
    m_cameraPosition = camPos;
}

void EG_BillboardRenderer::setCameraViewDir(glm::vec3 dir)
{
    m_cameraViewDir = dir;;
}



void EG_BillboardRenderer::loadUniformLocations(pipeline& p, int RenderPassID)
{
    setUniformLocation(m_modelViewProjectionMatrixUniLoc, m_modelViewProjectionMatrix);
    setUniformLocation(m_cameraPositionUniLoc, m_cameraPosition);
    setUniformLocation(m_cameraViewDirectionUniLoc, m_cameraViewDir);
    EG_Renderer::loadUniformLocations(p, RenderPassID);
}
