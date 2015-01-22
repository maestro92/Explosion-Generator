#include "EG_RenderTechniqueRenderTexture.h"



EG_RenderTechniqueRenderTexture::EG_RenderTechniqueRenderTexture()
{
}



EG_RenderTechniqueRenderTexture::~EG_RenderTechniqueRenderTexture()
{
}



void EG_RenderTechniqueRenderTexture::init(int Shader_Num)
{
    allocateMemberVariables(Shader_Num);
    progShaders[RENDER_PASS1] = new Shader("EG_RenderTechniqueRenderTexture.vs", "EG_RenderTechniqueRenderTexture.fs");

    m_textureUnitUniformLocation    = GetUniformLocation(progShaders[RENDER_PASS1], "gTexture");
    m_pixelSizeUniformLocation      = GetUniformLocation(progShaders[RENDER_PASS1], "gPixelSize");

    initMemberVariables();
}


void EG_RenderTechniqueRenderTexture::setTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_textureUnitUniformLocation, TextureUnit);
}

void EG_RenderTechniqueRenderTexture::setPixelSize(unsigned int Width, unsigned int Height)
{
    glUniform2f(m_pixelSizeUniformLocation, 1.0f/Width, 1.0f/Height);
}


void EG_RenderTechniqueRenderTexture::render(unsigned int TextureId, GLuint FboTarget, int Width, int Height)
{

    glDisable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	pipeline m_pipeline;
	m_pipeline.loadIdentity();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, FboTarget);
	glClear(GL_COLOR_BUFFER_BIT);
	progShaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,TextureId);
        setPixelSize(Width, Height);
        setTextureUnit(0);
        EG_RenderTechnique::loadUniformLocations(m_pipeline, RENDER_PASS1);
        m_quad.quad->draw(progShaders[RENDER_PASS1]->getProgramId());

	progShaders[RENDER_PASS1]->delShader();

    glEnable(GL_DEPTH_TEST);

}
