#ifndef EG_SKYBOX_H
#define EG_SKYBOX_H


#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include "EG_utility.h"
#include "shader.h"
#include "sceneLoader.h"
#include "pipeline.h"
#include "EG_Technique.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

using namespace std;

class EG_SkyBox : public Technique
{
    public:

        shader* SkyboxShader;

        EG_SkyBox();
        ~EG_SkyBox();

        GLuint Cubemap_UniLoc;

        void init();
        void RenderSkyBox(shader* skybox_shader);
        void RenderSkyBox(shader* skybox_shader, pipeline& m_pipeline);
  //      void RenderSkyBox_Dynamic(shader* skybox_shader, pipeline& m_pipeline);

        void Render_to_CubeMapFace();
        void Render_to_CubeMapTexture();
        void UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline);



        EG_utility utility_function;
        unsigned int Static_CubeMap_ColorTextureID;
        unsigned int Dynamic_CubeMap_ColorTextureID;
        unsigned int Dynamic_CubeMap_DepthTextureID;
        unsigned int CubeMapFBO;

        mesh* Skybox_Cube;
        pipeline RotationOnly_View_pipeline;
};


#endif
