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

    initDataPairUniLoc(&m_MVPMatrixDataPair,            RENDER_PASS1, "gModelViewProjectionMatrix");
    initDataPairUniLoc(&m_cameraPositionDataPair,       RENDER_PASS1, "gCameraPosition");
    initDataPairUniLoc(&m_cameraViewDirDataPair,        RENDER_PASS1, "gCameraViewDir");
    initMemberVariables();
}


void EG_BillboardRenderer::setModelViewProjectionMatrix(glm::mat4 vpMat)
{
    m_MVPMatrixDataPair.value = vpMat;
}

void EG_BillboardRenderer::setCameraPosition(glm::vec3 camPos)
{
    m_cameraPositionDataPair.value = camPos;
}

void EG_BillboardRenderer::setCameraViewDir(glm::vec3 dir)
{
    m_cameraViewDirDataPair.value = dir;
}

void EG_BillboardRenderer::loadUniformLocations(pipeline& p, int RenderPassID)
{
    setAllDataPairUniLocs(RenderPassID);
    EG_Renderer::loadUniformLocations(p, RenderPassID);
}
