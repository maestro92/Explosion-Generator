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
#include "EG_WorldObject.h"
#include "EG_CubeModel.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

using namespace std;

class EG_SkyBox : public WorldObject
{
    public:
        EG_SkyBox();
        ~EG_SkyBox();

        void init();

        void render(EG_Renderer* Renderer, int RenderPassID);
        void renderStatic(EG_Renderer* Renderer, int RenderPassID);
        void renderDynamic(EG_Renderer* Renderer, int RenderPassID);

        void renderToCubeMapFace();
        void renderToCubeMapTexture();
        void UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline);
        void UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline, glm::mat4 myMatrix);
        void UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline, glm::mat4 myMatrix, float m_pitch, float m_Yaw);


    //    void render(Shader* skybox_shader);
    //    void render(pipeline& m_pipeline, EG_Renderer* Renderer, int RenderPassID);


 //       void RenderSkyBox(Shader* skybox_shader);
 //       void RenderSkyBox(Shader* skybox_shader, pipeline& m_pipeline);

 //       void RenderSkyBox2();
  //      void RenderSkyBox2(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID);

 //       void Render111(Shader* skybox_shader);
//        void renderDynamic



//        EG_Utility utility_function;
    //    unsigned int Static_CubeMap_ColorTextureID;
        unsigned int Dynamic_CubeMap_ColorTextureID;
        unsigned int Dynamic_CubeMap_DepthTextureID;
        unsigned int CubeMapFBO;

  //      mesh* Skybox_Cube;
        EG_CubeModel m_cube;

        pipeline RotationOnly_View_pipeline;

        GLuint m_cubeMapFBO;
        GLuint m_staticCubeMapID;
        GLuint m_dynamicCubeMapID;
        GLuint m_dynamicCubeMapDepthID;
};


#endif

