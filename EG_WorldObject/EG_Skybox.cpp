
#include "EG_Skybox.h"


EG_SkyBox::EG_SkyBox()
{
}

EG_SkyBox::~EG_SkyBox()
{
}



void EG_SkyBox::init()
{
    m_cube.init();

    string cubemapTextures[6];
    cubemapTextures[0]="./Assets/Images/Skybox pictures/Skybox_Moon_Right.png";   /// positive x
	cubemapTextures[1]="./Assets/Images/Skybox pictures/Skybox_Moon_Left.png";    /// negative x
    cubemapTextures[2]="./Assets/Images/Skybox pictures/Skybox_Moon_Top.png";     /// positive y
	cubemapTextures[3]="./Assets/Images/Skybox pictures/Skybox_Moon_Bottom.png";     /// negative y
	cubemapTextures[4]="./Assets/Images/Skybox pictures/Skybox_Moon_Front.png";   /// positive z
	cubemapTextures[5]="./Assets/Images/Skybox pictures/Skybox_Moon_Back.png";    /// negative z
    m_staticCubeMapID = EG_Utility::createCubemapTexture(cubemapTextures);


    /// create the fbo
    /// 1 is to specify the number of framebuffer objects to be specified
    glGenFramebuffers(1,&m_cubeMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_cubeMapFBO);

    m_dynamicCubeMapID = EG_Utility::createCubemapTexture(cubemapTextures);


    /// create the uniform depth buffer
    m_dynamicCubeMapDepthID = EG_Utility::createDepthTexture(512, 512);

    glBindTexture(GL_TEXTURE_2D, m_dynamicCubeMapDepthID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void EG_SkyBox::renderStatic(EG_Renderer* Renderer, int RenderPassID)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    Renderer->enableShader(RenderPassID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);
        Renderer->loadUniformLocations(RotationOnly_View_pipeline, RenderPassID);
        m_cube.render();
    Renderer->disableShader(RenderPassID);
}

void EG_SkyBox::render(EG_Renderer* Renderer, int RenderPassID)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    Renderer->enableShader(RenderPassID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);
        Renderer->loadUniformLocations(RotationOnly_View_pipeline, RenderPassID);
        m_cube.render();
    Renderer->disableShader(RenderPassID);
}

void EG_SkyBox::UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline, glm::mat4 myMatrix)
{
    m_pipeline.pushMatrix();
        m_pipeline.loadMatrix(myMatrix);
        RotationOnly_View_pipeline = m_pipeline;
    m_pipeline.popMatrix();
}



void EG_SkyBox::UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline)
{
    RotationOnly_View_pipeline = m_pipeline;
}

void EG_SkyBox::renderToCubeMapFace()
{

}

void EG_SkyBox::renderToCubeMapTexture()
{

}


/*
void EG_SkyBox::render(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    Renderer->enableShader(RenderPassID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, Dynamic_CubeMap_ColorTextureID);
        Renderer->loadUniformLocations(RotationOnly_View_pipeline, RenderPassID);

        Skybox_Cube->draw();
    Renderer->disableShader(RenderPassID);
}
*/


    /*
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    m_shaders[RENDER_PASS2]->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);
        glUniform1i(uni_Loc,0);
        loadUniformLocations(RotationOnly_View_pipeline, RENDER_PASS2);

        m_cube.render();
    m_shaders[RENDER_PASS2]->delShader();
*/




/*
void EG_SkyBox::Render111(Shader* skybox_shader)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);
    m_shaders[RENDER_PASS2]->useShader();
 //   skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);
   //     glBindTexture(GL_TEXTURE_CUBE_MAP, Dynamic_CubeMap_ColorTextureID);


        glUniform1i(uni_Loc,0);
        loadUniformLocations(RotationOnly_View_pipeline, RENDER_PASS2);
        m_cube.render();
  //      Skybox_Cube->draw(skybox_shader->getProgramId());
//    skybox_shader->delShader();
    m_shaders[RENDER_PASS2]->delShader();
}
*/

/*
void EG_SkyBox::RenderSkyBox(Shader* skybox_shader)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

//    skybox_shader->useShader();
    m_shaders[RENDER_PASS1]->useShader();
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);
   //     glBindTexture(GL_TEXTURE_CUBE_MAP, Dynamic_CubeMap_ColorTextureID);


        glUniform1i(Cubemap_UniLoc,0);
        loadUniformLocations(RotationOnly_View_pipeline, RENDER_PASS1);

   //     .(RotationOnly_View_pipeline);

    //    Skybox_Cube->draw(skybox_shader->getProgramId());
        Skybox_Cube->draw(m_shaders[RENDER_PASS1]->getProgramId());

    m_shaders[RENDER_PASS1]->delShader();
  //  skybox_shader->delShader();
}

*/


/*
void EG_SkyBox::RenderSkyBox(Shader* skybox_shader, pipeline& m_pipeline)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);


        glUniform1i(Cubemap_UniLoc,0);
        loadUniformLocations(m_pipeline, RENDER_PASS1);


        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}
*/

/*
void EG_SkyBox::render(Shader* skybox_shader)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);

        GLuint tempCubemap_UniLoc = glGetUniformLocation(skybox_shader->getProgramId(), "cubeMap");
        glUniform1i(tempCubemap_UniLoc,0);

        loadUniformLocations(RotationOnly_View_pipeline, RENDER_PASS1);

        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}
*/


/*
void EG_SkyBox::renderStatic(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    Renderer->enableShader(RenderPassID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMapID);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);

        Skybox_Cube->draw();
    Renderer->disableShader(RenderPassID);
}
*/

/*
void EG_SkyBox::UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline, glm::mat4 myMatrix, float m_pitch, float m_Yaw)
{
    m_pipeline.pushMatrix();

 //       m_pipeline.rotateX(m_pitch);
 //       m_pipeline.rotateY(m_Yaw);
 //       m_pipeline.matrixMode(MODEL_MATRIX);
        m_pipeline.LoadMatrix(myMatrix);
        RotationOnly_View_pipeline = m_pipeline;
    m_pipeline.popMatrix();
}
*/

/*
void EG_SkyBox::RenderSkyBox_Dynamic(shader* skybox_shader, pipeline& m_pipeline)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Dynamic_CubeMap_ColorTextureID);


        glUniform1i(Cubemap_UniLoc,0);
        m_pipeline.updateMatrices(skybox_shader->getProgramId());
        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}
*/
