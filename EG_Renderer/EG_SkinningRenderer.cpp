#include "EG_SkinningRenderer.h"



EG_SkinningRenderer::EG_SkinningRenderer()
{

}



EG_SkinningRenderer::~EG_SkinningRenderer()
{

}



void EG_SkinningRenderer::init(int nShaders)
{
    for(int i=0; i<nShaders; i++)
    {
        vector<GLuint> v;
        v.resize(MAX_BONES);
        m_boneUniformLocations.push_back(v);
    }


    allocateMemberVariables(nShaders);


    m_shaders[RENDER_PASS1] = new Shader("EG_DynamicModelShadowMap.vs", "EG_DynamicModelShadowMap.fs");
    m_shaders[RENDER_PASS2] = new Shader("EG_DynamicModel.vs", "EG_DynamicModel.fs");

    initDirLightUniformLocations(RENDER_PASS2);

//    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(MAX_BONES) ; i++)
    for (unsigned int i = 0 ; i < MAX_BONES ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gBones[%d]", i);
    //    m_boneLocation[i] = GetUniformLocation( m_shaders[RENDER_PASS1], Name);
  //      m_boneLocation[i] = GetUniformLocation( m_shaders[RENDER_PASS2], Name);

        m_boneUniformLocations[RENDER_PASS1][i] = GetUniformLocation( m_shaders[RENDER_PASS1], Name);
        m_boneUniformLocations[RENDER_PASS2][i] = GetUniformLocation( m_shaders[RENDER_PASS2], Name);
    }

/*
    m_shaders[RENDER_PASS1] = new Shader("EG_ShadowMapDynamicModel.vs", "EG_ShadowMapDynamicModel.fs");
    m_shaders[RENDER_PASS2] = new Shader("EG_DynamicModel.vs", "EG_DynamicModel.fs");

    initDirLightUniformLocations(RENDER_PASS2);
  //  initPointLightUniformLocations();
  //  initSpotLightUniformLocations();

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gBones[%d]", i);
    //    m_boneLocation[i] = GetUniformLocation( m_shaders[RENDER_PASS1], Name);
        m_boneLocation[i] = GetUniformLocation( m_shaders[RENDER_PASS2], Name);
    }
*/

    initMemberVariables();
}



void EG_SkinningRenderer::loadUniformLocations(pipeline& p, int RenderPassID)
{
    EG_Renderer::loadUniformLocations(p, RenderPassID);

    for(unsigned int i = 0; i < m_boneTransforms.size(); i++)
    {
        assert(i < MAX_BONES);
        setUniformLocationTranspose(m_boneUniformLocations[RenderPassID][i], m_boneTransforms[i]);
    }

//    for(unsigned int i = 0; i < m_boneTransforms.size(); i++)
//        setBoneTransform(RenderPassID, i, m_boneTransforms[i]);
}


void EG_SkinningRenderer::setBoneTransforms(vector<glm::mat4>& transforms)
{
    m_boneTransforms = transforms;
}


void EG_SkinningRenderer::renewVector()
{
    m_boneTransforms.clear();
}
