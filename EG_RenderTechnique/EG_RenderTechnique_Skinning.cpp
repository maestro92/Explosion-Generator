#include "EG_RenderTechnique_Skinning.h"



EG_RenderTechnique_Skinning::EG_RenderTechnique_Skinning()
{

}



EG_RenderTechnique_Skinning::~EG_RenderTechnique_Skinning()
{

}



void EG_RenderTechnique_Skinning::init(int nShaders)
{
    allocateMemberVariables(nShaders);

    m_shaders[RENDER_PASS1] = new Shader("EG_DynamicModel.vs", "EG_DynamicModel.fs");

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gBones[%d]", i);
    //    m_boneLocation[i] = glGetUniformLocation( shaderID, Name);
        m_boneLocation[i] = GetUniformLocation( m_shaders[RENDER_PASS1], Name);
    }


    initMemberVariables();
}



void EG_RenderTechnique_Skinning::setBoneTransform(unsigned int index, const glm::mat4& transform)
{
    assert(index < MAX_BONES);
    glUniformMatrix4fv(m_boneLocation[index], 1, GL_TRUE, &transform[0][0]);
//    setUniformLocation(m_boneLocation[index], transform);
}
