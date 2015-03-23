
#include "EG_WorldReflectiveSphere.h"


EG_WorldReflectiveSphere::EG_WorldReflectiveSphere()
{

}

EG_WorldReflectiveSphere::~EG_WorldReflectiveSphere()
{

}


void EG_WorldReflectiveSphere::render(pipeline &m_pipeline, EG_DeferredShadingReflection& RenderTechnique,
                        int RenderPassID, unsigned int textureId, meshLoader* model, glm::vec3 EyePoint)
{
    RenderTechnique.enableShader(RENDER_PASS1);
    m_pipeline.pushMatrix();
        RenderTechnique.setStencilFlag(glm::vec3(1.0,0.0,0.0));

        RenderTechnique.setEyeWorldPos(EyePoint);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
        RenderTechnique.setCubeMapTextureUnit(0);

        m_pipeline.translate(w_Position);

        RenderTechnique.loadUniformLocations(m_pipeline, RENDER_PASS1);
        model->draw();
    m_pipeline.popMatrix();
    RenderTechnique.disableShader(RENDER_PASS1);
}

