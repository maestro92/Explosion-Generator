#ifndef EG_SKYBOX_H_
#define EG_SKYBOX_H_


#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include "EG_utility.h"
#include "EG_Shader.h"
#include "sceneLoader.h"
#include "pipeline.h"
#include "EG_Renderer.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

using namespace std;

class EG_SkyBox : public EG_Renderer
{
    public:

        Shader* SkyboxShader;

        EG_SkyBox();
        ~EG_SkyBox();

        GLuint Cubemap_UniLoc;

        void init();
        void init(int nShaders);
        void RenderSkyBox(Shader* skybox_shader);
        void RenderSkyBox(Shader* skybox_shader, pipeline& m_pipeline);
        void render(Shader* skybox_shader);
  //      void RenderSkyBox_Dynamic(shader* skybox_shader, pipeline& m_pipeline);
        void render1(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID);
        void renderStatic(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID);
        void render(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID);


        void Render_to_CubeMapFace();
        void Render_to_CubeMapTexture();
        void UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline);
        void UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline, glm::mat4 myMatrix);
        void UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline, glm::mat4 myMatrix, float m_pitch, float m_Yaw);

        EG_Utility utility_function;
        unsigned int Static_CubeMap_ColorTextureID;
        unsigned int Dynamic_CubeMap_ColorTextureID;
        unsigned int Dynamic_CubeMap_DepthTextureID;
        unsigned int CubeMapFBO;

        mesh* Skybox_Cube;
        pipeline RotationOnly_View_pipeline;
};


#endif

