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
    progShaders[RENDER_PASS1] = new Shader("EG_RenderTexture.vs", "EG_RenderTexture.fs");

    m_textureUnitUniformLocation    = GetUniformLocation(progShaders[RENDER_PASS1], "gTexture");
    m_pixelSizeUniformLocation      = GetUniformLocation(progShaders[RENDER_PASS1], "gPixelSize");

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
	progShaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureId);
        setPixelSize(Width, Height);
        setTextureUnit(0);
        EG_RenderTechnique::loadUniformLocations(m_pipeline, RENDER_PASS1);
        m_quad.quad->draw(progShaders[RENDER_PASS1]->getProgramId());

	progShaders[RENDER_PASS1]->delShader();

    glEnable(GL_DEPTH_TEST);
}



void EG_RenderTechnique_RenderTexture::render(unsigned int TextureId, GLuint FboTarget, int Width, int Height, pipeline& m_pipeline)
{
    glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, FboTarget);
	glClear(GL_COLOR_BUFFER_BIT);
	progShaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureId);
        setPixelSize(Width, Height);
        setTextureUnit(0);
        EG_RenderTechnique::loadUniformLocations(m_pipeline, RENDER_PASS1);
        m_quad.quad->draw(progShaders[RENDER_PASS1]->getProgramId());

	progShaders[RENDER_PASS1]->delShader();
}

/*
void ExplosionGenerator::RenderTexture(GLuint TextureId)
{

    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	m_pipeline.loadIdentity();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	quadRenderShader->useShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,TextureId);
	glUniform1i(glGetUniformLocation(quadRenderShader->getProgramId(),"texture"),0);
	glUniform2f(glGetUniformLocation(quadRenderShader->getProgramId(),"pixelSize"),1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT);
	m_pipeline.updateMatrices(quadRenderShader->getProgramId());
//	quad->draw(quadRenderShader->getProgramId());
    o_fullScreenQuad.quad->draw(quadRenderShader->getProgramId());
//    deferredShadingQuad->draw(quadRenderShader->getProgramId());
	quadRenderShader->delShader();

    glEnable(GL_DEPTH_TEST);
}

void ExplosionGenerator::RenderQuad(GLuint TextureId)
{
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //render texture to screen
//	m_pipeline.loadIdentity();
//	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
	quadRenderShader->useShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,TextureId);
	glUniform1i(glGetUniformLocation(quadRenderShader->getProgramId(),"texture"),0);
	glUniform2f(glGetUniformLocation(quadRenderShader->getProgramId(),"pixelSize"),1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT);
	m_pipeline.updateMatrices(quadRenderShader->getProgramId());
//	quad->draw(quadRenderShader->getProgramId());
    o_fullScreenQuad.quad->draw(quadRenderShader->getProgramId());
//    deferredShadingQuad->draw(quadRenderShader->getProgramId());
	quadRenderShader->delShader();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}



void ExplosionGenerator::RenderTexture2()
{

  //  m_pipeline.pushMatrix();
//    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //render texture to screen
	m_pipeline.loadIdentity();

    m_pipeline.matrixMode(PROJECTION_MATRIX);
	m_pipeline.pushMatrix();
	m_pipeline.ortho(-1,1,-1,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
	r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->useShader();
    gbuffer.bindForReadingLightPass();
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D,TextureId);
//	glUniform1i(glGetUniformLocation(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId(),"texture1"),0);

    r_deferredShadingDirectionalLightPass.setEyeWorldPos(  glm::vec3(thirdPersonPovCamera.m_eye.x,
                                                    thirdPersonPovCamera.m_eye.y,
                                                    thirdPersonPovCamera.m_eye.z));


//	m_pipeline.updateMatrices(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());
    r_deferredShadingDirectionalLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);

//	quad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());
    o_fullScreenQuad.quad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());
    m_pipeline.popMatrix();


    /// this deferredShading Quad is motherFxxking from the OpenGL UK, so it's reversed.. so you have to have
//    glDisable(GL_CULL_FACE);
//    deferredShadingQuad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());


	r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->delShader();


 //   glEnable(GL_DEPTH_TEST);
 //   m_pipeline.popMatrix();
}
*/
