#include "EG_TextureRenderer.h"



EG_TextureRenderer::EG_TextureRenderer()
{
}



EG_TextureRenderer::~EG_TextureRenderer()
{
}



void EG_TextureRenderer::init(int nShaders)
{
//    m_quad1.init();
    allocateMemberVariables(nShaders);
    m_shaders[RENDER_PASS1] = new Shader("EG_TextureRendererShader.vs", "EG_TextureRendererShader.fs");

    m_textureUnitUniLoc    = getUniLoc(m_shaders[RENDER_PASS1], "gTexture");
    m_pixelSizeUniLoc      = getUniLoc(m_shaders[RENDER_PASS1], "gPixelSize");


//    initDataPairUniLoc(&m_colorPair,        m_shaders[RENDER_PASS1], "gColor");

    initMemberVariables();


    o_fullScreenQuad.init();
}



void EG_TextureRenderer::init()
{
    Shader* s = new Shader("EG_TextureRenderer.vs", "EG_TextureRenderer.fs");

    addShader(s);
    addDataPair(RENDER_PASS1, "u_texture",        DP_INT);


    m_textureUnitUniLoc    = getUniLoc(m_shaders[RENDER_PASS1], "gTexture");


//    initDataPairUniLoc(&m_colorPair,        m_shaders[RENDER_PASS1], "gColor");

    initMemberVariables();


    o_fullScreenQuad.init();
}


void EG_TextureRenderer::setTextureUnit(unsigned int TextureUnit)
{
    m_texUnit = TextureUnit;
}

void EG_TextureRenderer::setPixelSize(unsigned int Width, unsigned int Height)
{
//    glUniform2f(m_pixelSizeUniLoc, 1.0f/Width, 1.0f/Height);
    m_width = Width;
    m_height = Height;
}



void EG_TextureRenderer::loadUniformLocations(pipeline& p, int pass)
{
    setUniformLocation(m_textureUnitUniLoc, m_texUnit);
    setUniformLocation(m_pixelSizeUniLoc, 1.0f/m_width, 1.0f/m_height);
    EG_Renderer::loadUniformLocations(p, pass);
}


void EG_TextureRenderer::renderFullScreen(GLuint TextureID)
{
    renderFullScreen(TextureID, 0);
    /*
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	pipeline m_pipeline;
	m_pipeline.loadIdentity();
	m_pipeline.ortho(0,1,0,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureId);
        loadUniformLocations(m_pipeline, RENDER_PASS1);
        model.render();
        // m_quad1.quad->draw(m_shaders[RENDER_PASS1]->getProgramId());

	m_shaders[RENDER_PASS1]->delShader();
    glEnable(GL_DEPTH_TEST);
*/
}

void EG_TextureRenderer::renderFullScreen(GLuint TextureID, GLuint FboTarget)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //render texture to screen
	pipeline m_pipeline;
	m_pipeline.loadIdentity();
	m_pipeline.ortho(0,1,0,1,-1,1);

    glBindFramebuffer(GL_FRAMEBUFFER, FboTarget);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        loadUniformLocations(m_pipeline, RENDER_PASS1);
        o_fullScreenQuad.render();

	m_shaders[RENDER_PASS1]->delShader();
    glEnable(GL_DEPTH_TEST);
}

/*
void EG_TextureRenderer::renderFullScreen(unsigned int TextureId, GLuint FboTarget, int Width, int Height)
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
        EG_Renderer::loadUniformLocations(m_pipeline, RENDER_PASS1);
        m_quad1.quad->draw(m_shaders[RENDER_PASS1]->getProgramId());

	m_shaders[RENDER_PASS1]->delShader();

    glEnable(GL_DEPTH_TEST);
}


void EG_TextureRenderer::render(unsigned int TextureId, GLuint FboTarget, int Width, int Height, pipeline& m_pipeline)
{
    glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, FboTarget);
	glClear(GL_COLOR_BUFFER_BIT);
	m_shaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureId);
        setPixelSize(Width, Height);
        setTextureUnit(0);
        EG_Renderer::loadUniformLocations(m_pipeline, RENDER_PASS1);
        m_quad1.quad->draw(m_shaders[RENDER_PASS1]->getProgramId());

	m_shaders[RENDER_PASS1]->delShader();
}

*/


/*
void EG_TextureRenderer::render(unsigned int TextureId, GLuint FboTarget, int Width, int Height, pipeline& m_pipeline)
{
    glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, FboTarget);
	glClear(GL_COLOR_BUFFER_BIT);
	m_shaders[RENDER_PASS1]->useShader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureId);
        setPixelSize(Width, Height);
        setTextureUnit(0);
        EG_Renderer::loadUniformLocations(m_pipeline, RENDER_PASS1);
        m_quad.render();

	m_shaders[RENDER_PASS1]->delShader();
}
*/

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
//    o_fullScreenQuad.quad->draw(quadRenderShader->getProgramId());
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
//    o_fullScreenQuad.quad->draw(quadRenderShader->getProgramId());
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
//    o_fullScreenQuad.quad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());
    m_pipeline.popMatrix();


    /// this deferredShading Quad is motherFxxking from the OpenGL UK, so it's reversed.. so you have to have
//    glDisable(GL_CULL_FACE);
//    deferredShadingQuad->draw(r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->getProgramId());


	r_deferredShadingDirectionalLightPass.progShaders[RENDER_PASS1]->delShader();


 //   glEnable(GL_DEPTH_TEST);
 //   m_pipeline.popMatrix();
}
*/
