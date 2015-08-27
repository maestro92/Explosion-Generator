
#include "EG_RendererManager.h"

EG_RendererManager::EG_RendererManager()
{

}

EG_RendererManager::~EG_RendererManager()
{

}


void EG_RendererManager::init()
{
    Shader* s;

    s = new Shader("EG_DepthShader.vs", "EG_DepthShader.fs");
    r_depthRenderer.addShader(s);


    /// r_fullColorRenderer
    s = new Shader("EG_FullVertexColorShader.vs", "EG_FullVertexColorShader.fs");
    r_fullColorRenderer.addShader(s);


    /// r_skyboxRenderer
    s = new Shader("EG_SkyboxShaderNew.vs", "EG_SkyboxShaderNew.fs");
    r_skyboxRenderer.addShader(s);
    r_skyboxRenderer.addDataPair(RENDER_PASS1, "u_cubeMapTexture",        DP_INT);



    /// r_GUIRenderer
    s = new Shader("/EG_GUIShaders/EG_GUI.vs", "/EG_GUIShaders/EG_GUI.fs");
    r_GUIRenderer.addShader(s);


    /// r_GUITextRenderer
 //   s = new Shader("/EG_GUIShaders/EG_GUIText.vs", "/EG_GUIShaders/EG_GUIText.fs");
  //  r_GUITextRenderer.addShader(s);


    ///r_particleCompositeRenderer
    s = new Shader("/EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.vs",
                   "/EG_NoiseBasedParticleEffectShaders/EG_CompositeShader.fs");
    r_particleCompositeRenderer.addShader(s);
    r_particleCompositeRenderer.addDataPair(RENDER_PASS1, "u_backgroundTexture",    DP_INT);
    r_particleCompositeRenderer.addDataPair(RENDER_PASS1, "u_particlesTexture",     DP_INT);
}






