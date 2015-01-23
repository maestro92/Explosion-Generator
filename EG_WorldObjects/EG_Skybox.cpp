
#include "EG_Skybox.h"


EG_SkyBox::EG_SkyBox()
{
}

EG_SkyBox::~EG_SkyBox()
{
    delete Skybox_Cube;
}

void EG_SkyBox::init()
{
    allocateMemberVariables(1);

//    SkyboxShader = new shader("skybox.vs", "skybox.frag");
//    Init_Shader_GL_Location(SkyboxShader, Matrices_UniLoc[RENDER_PASS1]);
    progShaders[RENDER_PASS1] = new Shader("skybox.vs", "skybox.frag");


    initMemberVariables();
    Cubemap_UniLoc = GetUniformLocation(progShaders[RENDER_PASS1], "cubeMap");

    /// load the model
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
        tmp.position = glm::vec3(-1.0,1.0,1.0);  tmp.color = glm::vec3(1.0,0.0,0.0);
		vertices.push_back(tmp);

		tmp.position = glm::vec3(-1.0,-1.0,1.0);	tmp.color = glm::vec3(0.0,1.0,0.0);
		vertices.push_back(tmp);

		tmp.position = glm::vec3(1.0,-1.0,1.0);	tmp.color = glm::vec3(0.0,0.0,1.0);
		vertices.push_back(tmp);

		tmp.position = glm::vec3(1.0,1.0,1.0);	tmp.color = glm::vec3(1.0,1.0,0.0);
		vertices.push_back(tmp);

		tmp.position = glm::vec3(-1.0,1.0,-1.0); tmp.color = glm::vec3(1.0,0.0,1.0);
		vertices.push_back(tmp);

		tmp.position = glm::vec3(-1.0,-1.0,-1.0);tmp.color = glm::vec3(1.0,0.0,0.0);
		vertices.push_back(tmp);

		tmp.position = glm::vec3(1.0,-1.0,-1.0);	tmp.color = glm::vec3(1.0,1.0,1.0);
		vertices.push_back(tmp);

		tmp.position = glm::vec3(1.0,1.0,-1.0);	tmp.color = glm::vec3(0.0,1.0,1.0);
		vertices.push_back(tmp);

		//front face
		indices.push_back(0);   indices.push_back(1);	indices.push_back(2);
		indices.push_back(0);  	indices.push_back(3);   indices.push_back(2);

		//left face
		indices.push_back(2);	indices.push_back(3);	indices.push_back(6);
		indices.push_back(3);	indices.push_back(7);	indices.push_back(6);

		//back face
		indices.push_back(4);	indices.push_back(5);	indices.push_back(6);
		indices.push_back(4);	indices.push_back(7);	indices.push_back(6);

		//right face
		indices.push_back(0);	indices.push_back(1);	indices.push_back(5);
		indices.push_back(0);	indices.push_back(4);	indices.push_back(5);

		//top face
		indices.push_back(0);	indices.push_back(3);	indices.push_back(4);
		indices.push_back(3);	indices.push_back(4);	indices.push_back(7);

		//bottom face
		indices.push_back(1);	indices.push_back(2);	indices.push_back(6);
		indices.push_back(1);	indices.push_back(5);	indices.push_back(6);
		Skybox_Cube=new mesh(&vertices,&indices);
	}


    /// init skybox textures
    std::string cube_filename[6];
    cube_filename[0]="./images/Skybox pictures/Skybox_Moon_Right.png";   /// positive x
	cube_filename[1]="./images/Skybox pictures/Skybox_Moon_Left.png";    /// negative x
    cube_filename[2]="./images/Skybox pictures/Skybox_Moon_Top.png";     /// positive y
	cube_filename[3]="./images/Skybox pictures/Skybox_Moon_Bottom.png";     /// negative y
	cube_filename[4]="./images/Skybox pictures/Skybox_Moon_Front.png";   /// positive z
	cube_filename[5]="./images/Skybox pictures/Skybox_Moon_Back.png";    /// negative z
    Static_CubeMap_ColorTextureID = utility_function.Create_CubemapTexture(cube_filename);

    /// create the fbo
    /// 1 is to specify the number of framebuffer objects to be specified
    glGenFramebuffers(1,&CubeMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);

//    Dynamic_CubeMap_ColorTextureID = utility_function.Create_CubemapTexture();
    Dynamic_CubeMap_ColorTextureID = utility_function.Create_CubemapTexture(cube_filename);

    /// create the uniform depth buffer
    Dynamic_CubeMap_DepthTextureID = utility_function.Create_Texture(512, 512, true);
    glBindTexture(GL_TEXTURE_2D, Dynamic_CubeMap_DepthTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void EG_SkyBox::RenderSkyBox(Shader* skybox_shader)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, Static_CubeMap_ColorTextureID);
   //     glBindTexture(GL_TEXTURE_CUBE_MAP, Dynamic_CubeMap_ColorTextureID);


        glUniform1i(Cubemap_UniLoc,0);
        loadUniformLocations(RotationOnly_View_pipeline, RENDER_PASS1);

   //     .(RotationOnly_View_pipeline);

        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}

void EG_SkyBox::RenderSkyBox(Shader* skybox_shader, pipeline& m_pipeline)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Static_CubeMap_ColorTextureID);


        glUniform1i(Cubemap_UniLoc,0);
        loadUniformLocations(m_pipeline, RENDER_PASS1);


        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}



void EG_SkyBox::render(Shader* skybox_shader)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Static_CubeMap_ColorTextureID);

        GLuint tempCubemap_UniLoc = glGetUniformLocation(skybox_shader->getProgramId(), "cubeMap");
        glUniform1i(tempCubemap_UniLoc,0);

        loadUniformLocations(RotationOnly_View_pipeline, RENDER_PASS1);

        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}




void EG_SkyBox::renderStatic(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    RenderTechnique->enableShader(RenderPassID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Static_CubeMap_ColorTextureID);
        RenderTechnique->loadUniformLocations(m_pipeline, RenderPassID);

        Skybox_Cube->draw();
    RenderTechnique->disableShader(RenderPassID);
}



void EG_SkyBox::render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    RenderTechnique->enableShader(RenderPassID);
//        r_deferredShadingSkyboxPass.setStencilFlag(glm::vec3(1.0,0.0,0.0));
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, Static_CubeMap_ColorTextureID);

//        GLuint tempCubemap_UniLoc = glGetUniformLocation(skybox_shader->getProgramId(), "cubeMap");
//        glUniform1i(tempCubemap_UniLoc,0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Static_CubeMap_ColorTextureID);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, Dynamic_CubeMap_ColorTextureID);
        RenderTechnique->loadUniformLocations(RotationOnly_View_pipeline, RenderPassID);

        Skybox_Cube->draw();
    RenderTechnique->disableShader(RenderPassID);
}

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



void EG_SkyBox::UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline, glm::mat4 myMatrix)
{
    m_pipeline.pushMatrix();
        m_pipeline.LoadMatrix(myMatrix);
        RotationOnly_View_pipeline = m_pipeline;
    m_pipeline.popMatrix();
}

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

void EG_SkyBox::UpdateRotationOnly_View_Pipeline(pipeline& m_pipeline)
{
    RotationOnly_View_pipeline = m_pipeline;
}

void EG_SkyBox::Render_to_CubeMapFace()
{

}

void EG_SkyBox::Render_to_CubeMapTexture()
{

}


