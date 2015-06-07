#include "EG_NoiseBasedParticleEffect.h"


EG_SurfacePod EG_NoiseBasedParticleEffect::createSurface(int width, int height)
{
    EG_SurfacePod pod;

    /// Create a FBO
    pod.FBO = EG_Utility::createFBO();

    /// Create a depth texture:
    pod.depthTexture = EG_Utility::createDepthTexture(width, height);
    /// Attach the depth texture
    glBindFramebuffer(GL_FRAMEBUFFER, pod.FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, pod.depthTexture, 0);


    /// Create a color texture
    pod.colorTexture = EG_Utility::createTexture(width, height);

    /// Attach the color buffer:
    GLuint colorBuffer;
    glGenRenderbuffers(1, &colorBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pod.colorTexture, 0);

    EG_Utility::errorCheckFBO();
    EG_Utility::errorCheck();

    return pod;
}




void EG_NoiseBasedParticleEffect::init(int width, int height)
{
    m_backgroundSurface = createSurface(width, height);
    m_particleSurface = createSurface(width, height);

    m_backgroundTexture = EG_Utility::loadTexture("Assets/Images/Scroll.png");
    m_spriteTexture = EG_Utility::loadTexture("Assets/Images/Sprite.png");

    m_screenQuad.init();
}
