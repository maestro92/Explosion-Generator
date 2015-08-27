
#include "EG_FullScreenQuad.h"

EG_FullScreenQuad::EG_FullScreenQuad() : WorldObject()
{
    defaultModel = new EG_QuadModelABS(0);
}


EG_FullScreenQuad::~EG_FullScreenQuad()
{

}

void EG_FullScreenQuad::render(EG_Renderer* Renderer, int pass)
{
    render(Renderer, pass, 0);

}

void EG_FullScreenQuad::render(EG_Renderer* Renderer, int pass, GLuint FboTarget)
{
    if(defaultModel != NULL)
    {
        pipeline temp_pipeline;
        temp_pipeline.loadIdentity();
        temp_pipeline.ortho(0,1,0,1,-1,1);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glBindFramebuffer(GL_FRAMEBUFFER, FboTarget);

        Renderer->enableShader(pass);
            Renderer->loadUniformLocations(temp_pipeline, pass);
            defaultModel->render();
        Renderer->disableShader(pass);

        glEnable(GL_CULL_FACE);
    }
}


