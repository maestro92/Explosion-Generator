#include "EG_GBuffer.h"



EG_GBuffer::EG_GBuffer()
{
    m_fbo = 0;
	m_depthTexture = 0;
    ZERO_MEM(m_textures);
}

EG_GBuffer::~EG_GBuffer()
{
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
    }

    if (m_textures[0] != 0) {
        glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);
    }

	if (m_depthTexture != 0) {
		glDeleteTextures(1, &m_depthTexture);
	}
}





void EG_GBuffer::init(unsigned int width, unsigned int height)
{
    // Create the FBO
    glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create the gbuffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);
	glGenTextures(1, &m_depthTexture);

    // we are writing on all four textures,
    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures) ; i++)
    {
    	glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
    }

	// depth
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

#if 1
    /// the glDrawBuffers command makes rendering to multiple destinations possible
	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
						     GL_COLOR_ATTACHMENT1,
						     GL_COLOR_ATTACHMENT2,
						     GL_COLOR_ATTACHMENT3 };
#else
	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
						     GL_COLOR_ATTACHMENT1,
						     GL_COLOR_ATTACHMENT2,
						     GL_COLOR_ATTACHMENT3};
#endif

    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return;
    }

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}



void EG_GBuffer::bindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}


void EG_GBuffer::bindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}


void EG_GBuffer::setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}


void EG_GBuffer::bindForReadingMrtDemo()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void EG_GBuffer::bindForReadingLightPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
	}
}


GLuint EG_GBuffer::get_depth_texture()
{
    return m_depthTexture;
}


void EG_GBuffer::bindForReading36()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++)
    {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
	}
}








void EG_GBuffer::init37(unsigned int width, unsigned int height)
{
    // Create the FBO
    glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create the gbuffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);
	glGenTextures(1, &m_depthTexture);
    glGenTextures(1, &m_finalTexture);

    // we are writing on all four textures,
    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures) ; i++)
    {
    	glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
    }

	// depth
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);


	// final
	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, m_finalTexture, 0);

/*
    /// the glDrawBuffers command makes rendering to multiple destinations possible
	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
						     GL_COLOR_ATTACHMENT1,
						     GL_COLOR_ATTACHMENT2,
						     GL_COLOR_ATTACHMENT3 };

    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);
*/
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return;
    }

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}



void EG_GBuffer::StartFrame37()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT7);
	glClear(GL_COLOR_BUFFER_BIT);
}


void EG_GBuffer::BindForGeomPass37()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
						     GL_COLOR_ATTACHMENT1,
						     GL_COLOR_ATTACHMENT2,
						     GL_COLOR_ATTACHMENT3,
						     GL_COLOR_ATTACHMENT4,
						     GL_COLOR_ATTACHMENT5 };
    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);
}


void EG_GBuffer::BindForStencilPass37()
{
    // must disable the draw buffers
	glDrawBuffer(GL_NONE);
}

void EG_GBuffer::BindForLightPass37()
{
    // we set the target to be the final buffer and bind the attribute buffers as a source
	glDrawBuffer(GL_COLOR_ATTACHMENT7);

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
	}
}

void EG_GBuffer::BindForFinalPass37()
{
    // set default FBO to draw destination
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// set our G_Buffer as the source
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT7);
}
