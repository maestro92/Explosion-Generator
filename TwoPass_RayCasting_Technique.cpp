
#include "TwoPass_RayCasting_Technique.h"



TwoPass_RayCasting_Technique::TwoPass_RayCasting_Technique()
{

}


TwoPass_RayCasting_Technique::~TwoPass_RayCasting_Technique()
{

}


void TwoPass_RayCasting_Technique::init(int w, int h)
{
    // generate a frame buffer
    glGenFramebuffers(1, &IntervalsFbo1);
    glBindFramebuffer(GL_FRAMEBUFFER,IntervalsFbo1);

    glGenTextures(1, &TextureFbo1);
    glBindTexture(GL_TEXTURE_2D, TextureFbo1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureFbo1, 0);

    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "TextureFBO1 error" << endl;
        exit(1);
    }

    // switch back to window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    // generate a frame buffer
    glGenFramebuffers(1, &IntervalsFbo2);
    glBindFramebuffer(GL_FRAMEBUFFER,IntervalsFbo2);

    glGenTextures(1, &TextureFbo2);
    glBindTexture(GL_TEXTURE_2D, TextureFbo2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureFbo2, 0);

    // check FBO status
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "TextureFBO2 error" << endl;
        exit(1);
    }

    // switch back to window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    /// Interval
    TwoPassIntervals = new shader("TwoPass.vs", "TwoPass.Cube", "TwoPass.Intervals");
    Init_Shader_GL_Location(TwoPassIntervals->getProgramId(), Matrices_Loc1);

    /// Cube depth
    TwoPass_CubeDepth = new shader("TwoPass.vs", "TwoPass.Cube", "TwoPass_Depth.fs");
    Init_Shader_GL_Location(TwoPass_CubeDepth->getProgramId(), Matrices_CubeDepth);

    /// Raycast
    TwoPassRaycast = new shader("TwoPass.vs", "TwoPass.Fullscreen", "TwoPass.Raycast");
    Init_Shader_GL_Location(TwoPassRaycast->getProgramId(), Matrices_Loc2);

    RayStartPoints_Location2 = glGetUniformLocation( TwoPassRaycast->getProgramId(), "RayStartPoints");
    RayStopPoints_Location2 = glGetUniformLocation( TwoPassRaycast->getProgramId(), "RayStopPoints");
    Depth_Location2 = glGetUniformLocation( TwoPassRaycast->getProgramId(), "DepthMap");
    Depth_TwoPassLocation_Front2 = glGetUniformLocation( TwoPassRaycast->getProgramId(), "Depth_TwoPassFront");
    Depth_TwoPassLocation_Back2 = glGetUniformLocation( TwoPassRaycast->getProgramId(), "Depth_TwoPassBack");


    if (Matrices_Loc2.ModelviewProjection == -1)
    {
        cout << "TwoPass get location error" << endl;
        exit(1);
    }


 //   glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    TwoPass_CubeDepthTexture_Front = createTexture(w, h, true);
    glBindTexture(GL_TEXTURE_2D, TwoPass_CubeDepthTexture_Front);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);

    TwoPass_CubeDepthTexture_Back = createTexture(w, h, true);
    glBindTexture(GL_TEXTURE_2D, TwoPass_CubeDepthTexture_Back);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);

}




// assign GL location
void TwoPass_RayCasting_Technique::Init_Shader_GL_Location(unsigned int shaderID, Matrices_Location& Mat)
{
    Mat.ModelviewProjection = glGetUniformLocation( shaderID, "m_ModelviewProjection");
    Mat.ModelviewMatrix = glGetUniformLocation( shaderID, "m_Modelview");
    Mat.ViewMatrix = glGetUniformLocation( shaderID, "m_ViewMatrix");
    Mat.ProjectionMatrix = glGetUniformLocation( shaderID, "m_ProjectionMatrix");
    Mat.ModelMatrix = glGetUniformLocation( shaderID, "m_ModelMatrix");
    Mat.ViewNoRotateMatrix = glGetUniformLocation( shaderID, "m_ViewNoRotateMatrix");

    if(Mat.ModelviewProjection == -1 ||
       Mat.ModelviewMatrix == -1 ||
       Mat.ViewMatrix == -1 ||
       Mat.ProjectionMatrix == -1 ||
       Mat.ModelMatrix == -1 ||
       Mat.ViewNoRotateMatrix == -1 )
    {
        cout << "Error in Init_Shader" << endl;
    //    exit(1);
    }

}



void TwoPass_RayCasting_Technique::Render_TwoPass_RayCasting_1(Matrices_t& Mat)
{
    TwoPassIntervals->useShader();

        Load_glUniform(Matrices_Loc1, Mat);
        glClearColor(0, 0, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, IntervalsFbo1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);
        glDrawArrays(GL_POINTS, 0, 1);


        glBindFramebuffer(GL_FRAMEBUFFER, IntervalsFbo2);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    TwoPassIntervals->delShader();
        glCullFace(GL_BACK);
}

void TwoPass_RayCasting_Technique::Render_TwoPass_RayCasting_1_draft(Matrices_t& Mat)
{
    TwoPassIntervals->useShader();

        Load_glUniform(Matrices_Loc1, Mat);
        glClearColor(0, 0, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, IntervalsFbo1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);
        glDrawArrays(GL_POINTS, 0, 1);


        glBindFramebuffer(GL_FRAMEBUFFER, IntervalsFbo2);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    TwoPassIntervals->delShader();
        glCullFace(GL_BACK);
}



void TwoPass_RayCasting_Technique::Render_TwoPass_RayCasting_CubeDepth(Matrices_t& Mat, GLuint fbo)
{
    /// Back Cube depth
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, TwoPass_CubeDepthTexture_Back, 0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    //    m_pipeline.pushMatrix();
        TwoPass_CubeDepth->useShader();
            Load_glUniform(Matrices_CubeDepth, Mat);
            glDrawArrays(GL_POINTS, 0, 1);
        TwoPass_CubeDepth->delShader();
   //     m_pipeline.popMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    /// Front Cube depth
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glCullFace(GL_BACK);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, TwoPass_CubeDepthTexture_Front, 0);

    glEnable(GL_DEPTH_TEST);
    TwoPass_CubeDepth->useShader();
        glClear(GL_DEPTH_BUFFER_BIT);
    //    m_pipeline.pushMatrix();
            Load_glUniform(Matrices_CubeDepth, Mat);
            glDrawArrays(GL_POINTS, 0, 1);
    //    m_pipeline.popMatrix();
    TwoPass_CubeDepth->delShader();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void TwoPass_RayCasting_Technique::Render_TwoPass_RayCasting_2(Matrices_t& Mat, GLuint depthTexture)
{
    glEnable(GL_BLEND);
    TwoPassRaycast->useShader();
        Load_glUniform(Matrices_Loc2, Mat);
        glUniform1i(RayStartPoints_Location2, 1);
        glUniform1i(RayStopPoints_Location2, 2);
        glUniform1i(Depth_Location2, 3);
        glUniform1i(Depth_TwoPassLocation_Front2, 4);
        glUniform1i(Depth_TwoPassLocation_Back2, 5);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, TextureFbo1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, TextureFbo2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, TwoPass_CubeDepthTexture_Front);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, TwoPass_CubeDepthTexture_Back);

        glDrawArrays(GL_POINTS, 0, 1);
    TwoPassRaycast->delShader();

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}



void TwoPass_RayCasting_Technique::Load_glUniform(Matrices_Location& Mat_Loc, Matrices_t& Mat)
{
    glUniformMatrix4fv(Mat_Loc.ModelviewProjection,1,GL_FALSE,&Mat.ModelviewProjection[0][0]);
    glUniformMatrix4fv(Mat_Loc.ModelviewMatrix,1,GL_FALSE,&Mat.Modelview[0][0]);
    glUniformMatrix4fv(Mat_Loc.ViewMatrix,1,GL_FALSE,&Mat.View[0][0]);
    glUniformMatrix4fv(Mat_Loc.ProjectionMatrix,1,GL_FALSE,&Mat.Projection[0][0]);
    glUniformMatrix4fv(Mat_Loc.ModelMatrix,1,GL_FALSE,&Mat.Model[0][0]);
    glUniformMatrix4fv(Mat_Loc.ViewNoRotateMatrix,1,GL_FALSE,&Mat.ViewNoRotate[0][0]);
}



unsigned int TwoPass_RayCasting_Technique::createTexture(int w, int h, bool isDepth)
{
    unsigned int textureID;

    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexImage2D(GL_TEXTURE_2D,0,(!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT),w,h,0,isDepth ? GL_DEPTH_COMPONENT : GL_RGBA,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    int i;
    i = glGetError();
    if(i!=0)
    {
        std::cout << "Error happened while loading the texture: " << i << std::endl;
    }
    // unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}





