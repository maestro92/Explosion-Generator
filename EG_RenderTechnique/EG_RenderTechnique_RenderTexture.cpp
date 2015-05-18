#include "EG_RenderTechnique_RenderTexture.h"



EG_RenderTechnique_RenderTexture::EG_RenderTechnique_RenderTexture()
{
}



EG_RenderTechnique_RenderTexture::~EG_RenderTechnique_RenderTexture()
{
}



void EG_RenderTechnique_RenderTexture::init(int Shader_Num)
{
    m_quad.init();
    allocateMemberVariables(Shader_Num);
    m_shaders[RENDER_PASS1] = new Shader("EG_RenderTexture.vs", "EG_RenderTexture.fs");

    m_textureUnitUniformLocation    = GetUniformLocation(m_shaders[RENDER_PASS1], "gTexture");
    m_pixelSizeUniformLocation      = GetUniformLocation(m_shaders[RENDER_PASS1], "gPixelSize");

    initMemberVariables();
}


void EG_RenderTechnique_RenderTexture::setTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_textureUnitUniformLocation, TextureUnit);
}

void EG_RenderTechnique_RenderTexture::setPixelSize(unsigned int Width, unsigned int Height)
{
    glUniform2f(m_pixelSizeUniformLocation, 1.0f/Width, 1.0f/Height);
}


void EG_RenderTechnique_RenderTexture::renderFullScreen(unsigned int TextureId, GLuint FboTarget, int Width, int Height)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	pipeline m_pipeline;
	m_pipeline.loadIdentity();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, FboTarget);
	glClear(GL_COLOR_BUFFER_BIT);
	m_shaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureId);
        setPixelSize(Width, Height);
        setTextureUnit(0);
        EG_RenderTechnique::loadUniformLocations(m_pipeline, RENDER_PASS1);
        m_quad.quad->draw(m_shaders[RENDER_PASS1]->getProgramId());

	m_shaders[RENDER_PASS1]->delShader();

    glEnable(GL_DEPTH_TEST);
}



void EG_RenderTechnique_RenderTexture::render(unsigned int TextureId, GLuint FboTarget, int Width, int Height, pipeline& m_pipeline)
{
    glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, FboTarget);
	glClear(GL_COLOR_BUFFER_BIT);
	m_shaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureId);
        setPixelSize(Width, Height);
        setTextureUnit(0);
        EG_RenderTechnique::loadUniformLocations(m_pipeline, RENDER_PASS1);
        m_quad.quad->draw(m_shaders[RENDER_PASS1]->getProgramId());

	m_shaders[RENDER_PASS1]->delShader();
}
