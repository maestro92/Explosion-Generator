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

/*
    m_modelViewProjectionMatrixUniLoc = getUniLoc(m_shaders[RENDER_PASS1], "gModelViewProjectionMatrix");
    m_cameraPositionUniLoc = getUniLoc(m_shaders[RENDER_PASS1], "gCameraPosition");
    m_cameraViewDirectionUniLoc = getUniLoc(m_shaders[RENDER_PASS1], "gCameraViewDir");
//    m_textureUniLoc = getUniformLocation(m_shaders[RENDER_PASS1], "gTexture");
*/
    initDataPairUniLoc(&m_MVPMatrixDataPair,            m_shaders[RENDER_PASS1], "gModelViewProjectionMatrix");
    initDataPairUniLoc(&m_cameraPositionDataPair,       m_shaders[RENDER_PASS1], "gCameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        m_shaders[RENDER_PASS1], "gCameraViewDir");

    initMemberVariables();
}


void EG_BillboardRenderer::setModelViewProjectionMatrix(glm::mat4 vpMat)
{
//    m_modelViewProjectionMatrix = vpMat;
    m_MVPMatrixDataPair.value = vpMat;
}

void EG_BillboardRenderer::setCameraPosition(glm::vec3 camPos)
{
//    m_cameraPosition = camPos;
    m_cameraPositionDataPair.value = camPos;
}

void EG_BillboardRenderer::setCameraViewDir(glm::vec3 dir)
{
//    m_cameraViewDir = dir;;
    m_cameraViewDirDataPair.value = dir;
}

void EG_BillboardRenderer::loadUniformLocations(pipeline& p, int RenderPassID)
{
    setDataPairUniLoc(m_MVPMatrixDataPair);
    setDataPairUniLoc(m_cameraPositionDataPair);
    setDataPairUniLoc(m_cameraViewDirDataPair);

    EG_Renderer::loadUniformLocations(p, RenderPassID);
}
