
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
    /// load the model
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		tmp.position.change(-1.0,1.0,1.0);  tmp.color.change(1.0,0.0,0.0);
		vertices.push_back(tmp);

		tmp.position.change(-1.0,-1.0,1.0);	tmp.color.change(0.0,1.0,0.0);
		vertices.push_back(tmp);

		tmp.position.change(1.0,-1.0,1.0);	tmp.color.change(0.0,0.0,1.0);
		vertices.push_back(tmp);

		tmp.position.change(1.0,1.0,1.0);	tmp.color.change(1.0,1.0,0.0);
		vertices.push_back(tmp);

		tmp.position.change(-1.0,1.0,-1.0); tmp.color.change(1.0,0.0,1.0);
		vertices.push_back(tmp);

		tmp.position.change(-1.0,-1.0,-1.0);tmp.color.change(1.0,0.0,0.0);
		vertices.push_back(tmp);

		tmp.position.change(1.0,-1.0,-1.0);	tmp.color.change(1.0,1.0,1.0);
		vertices.push_back(tmp);

		tmp.position.change(1.0,1.0,-1.0);	tmp.color.change(0.0,1.0,1.0);
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

    Dynamic_CubeMap_ColorTextureID = utility_function.Create_CubemapTexture();

    /// create the uniform depth buffer
    Dynamic_CubeMap_DepthTextureID = utility_function.Create_Texture(512, 512, true);
    glBindTexture(GL_TEXTURE_2D, Dynamic_CubeMap_DepthTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void EG_SkyBox::RenderSkyBox(shader* skybox_shader)
{
    RotationOnly_View_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, Static_CubeMap_ColorTextureID);
    //    glBindTexture(GL_TEXTURE_CUBE_MAP, Dynamic_CubeMap_ColorTextureID);

        glUniform1i(glGetUniformLocation(skybox_shader->getProgramId(),"cubeMap"),0);
        RotationOnly_View_pipeline.updateMatrices(skybox_shader->getProgramId());
        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}

void EG_SkyBox::RenderSkyBox(shader* skybox_shader, pipeline& m_pipeline)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Static_CubeMap_ColorTextureID);

        glUniform1i(glGetUniformLocation(skybox_shader->getProgramId(),"cubeMap"),0);
        m_pipeline.updateMatrices(skybox_shader->getProgramId());
        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}



void EG_SkyBox::RenderSkyBox_Dynamic(shader* skybox_shader, pipeline& m_pipeline)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Dynamic_CubeMap_ColorTextureID);

        glUniform1i(glGetUniformLocation(skybox_shader->getProgramId(),"cubeMap"),0);
        m_pipeline.updateMatrices(skybox_shader->getProgramId());
        Skybox_Cube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}


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


/*
void ExplosionGenerator::RenderSkybox(shader* skybox_shader, unsigned cubemapTexture, pipeline& m_pipeline)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    skybox_shader->useShader();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glUniform1i(glGetUniformLocation(skybox_shader->getProgramId(),"cubeMap"),0);
        m_pipeline.updateMatrices(skybox_shader->getProgramId());
        MyCube->draw(skybox_shader->getProgramId());
    skybox_shader->delShader();
}



void ExplosionGenerator::Render_to_CubeMapTexture(int face, shader* skybox_shader, pipeline& m_pipeline)
{
    glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);
    glViewport(0, 0, 512, 512);

    for(int i=0; i<6; i++)
    {
        Render_to_CubeMapFace(i, cubemapTexture_write, SkyboxShader, CubeMap, tPipeline, MyCube, cubemapDepthTexture_write);
        RenderScene(mainShader, CubeMesh, tPipeline);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 640, 480);
}



void ExplosionGenerator::Render_to_CubeMapFace(int face, shader* skybox_shader, unsigned int cubemap_color_texture,
                                                    unsigned int cubemap_depth_texture, pipeline& m_pipeline, mesh* skybox_cube)
{

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)face, cubemap_color_texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, cubemap_depth_texture, 0);
    glClearColor(0.0,0.0,0.5,1.0);

    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();

    m_pipeline.perspective(90,       // the camera angle
                            1,              // width to height ratio
                            0.5,            // the near z clippFing coordinate
                            1000.0);


    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadIdentity();


    switch (face)
    {
        case POSITIVE_X:
            tPipeline.rotateZ(180);
            tPipeline.rotateY(-90);
            break;

        case NEGATIVE_X:
            tPipeline.rotateZ(180);
            tPipeline.rotateY(90);
            break;

        case POSITIVE_Y:
            tPipeline.rotateX(90);
            break;

        case NEGATIVE_Y:
            tPipeline.rotateX(-90);
            break;

        case POSITIVE_Z:
            tPipeline.rotateZ(180);
            tPipeline.rotateY(180);
            break;

        case NEGATIVE_Z:
            tPipeline.rotateZ(180);
            break;
        default:
            break;
    };


    m_pipeline.matrixMode(MODEL_MATRIX);
    RenderSkyBox(skybox_shader, cubemap_color_texture, m_pipeline, skybox_cube);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

}
*/
