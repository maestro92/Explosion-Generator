#include "EG_RenderTechnique_Skinning.h"





void EG_RenderTechnique_Skinning::init(int nShaders)
{
    allocateMemberVariables(nShaders);

    m_shaders[RENDER_PASS1] = new Shader("EG_DynamicModel.vs", "EG_DynamicModel.fs");

    initMemberVariables();
}



void EG_RenderTechnique_Skinning::setBoneTransform(unsigned int index, const glm::mat4& transform)
{
    assert(index < MAX_BONES);
    setUniformLocation(m_boneLocation[index], transform);
}
