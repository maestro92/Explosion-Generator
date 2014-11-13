
#include "EG_Technique_Shadow_Render.h"


Technique_Shadow_Render::Technique_Shadow_Render()
{


}



Technique_Shadow_Render::~Technique_Shadow_Render()
{



}



void Technique_Shadow_Render::init(int w, int h, int Shader_Num)
{
    allocate_memberVariables(Shader_Num);

    ShadowMapWidth = w*2;
    ShadowMapHeight = h*2;

    /// FirstPass_LightPOV
    /// SecondPass_CameraPOV
    ProgShaders[RENDER_PASS1] = new shader("shadow_FirstRender.vs", "shadow_FirstRender.fs");
    ProgShaders[RENDER_PASS2] = new shader("shadow_SecondRender.vs", "shadow_SecondRender.fs");


    ShadowMatrix_UniLoc[RENDER_PASS1] = GetUniformLocation(ProgShaders[RENDER_PASS1], "lightModelViewProjectionMatrix");
    ShadowMatrix_UniLoc[RENDER_PASS2] = GetUniformLocation(ProgShaders[RENDER_PASS2],  "lightModelViewProjectionMatrix");

    LightMVPmatrix_UniLoc =             GetUniformLocation(ProgShaders[RENDER_PASS2], "lightModelViewProjectionMatrix");
    shadowMap_UniLoc =                  GetUniformLocation(ProgShaders[RENDER_PASS2], "shadowMap");
    LightPosition_MVmatrix_UniLoc =     GetUniformLocation(ProgShaders[RENDER_PASS2], "LightPosition_ModelViewMatrix");
    LightPosition_UniLoc =              GetUniformLocation(ProgShaders[RENDER_PASS2], "LightPosition");
    CameraPosition_UniLoc =             GetUniformLocation(ProgShaders[RENDER_PASS2], "cameraPosition");

    init_memberVariables();
}



void Technique_Shadow_Render::Setup_ShadowMatrix_forRender(pipeline& p, int RenderPassID)
{
    p.shadowMatrix = p.Light_BiasMatrix * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
    glUniformMatrix4fv(ShadowMatrix_UniLoc[RenderPassID],1,GL_FALSE,&p.shadowMatrix[0][0]);
}


#if 0
void Technique_Shadow_Render::Setup_Matrix_forRender1(pipeline& p)
{
    Load_glUniform(Matrices_Loc1, p);
}


void Technique_Shadow_Render::Setup_Matrix_forRender2(pipeline& p)
{
    Load_glUniform(Matrices_Loc2, p);
}

#else

/*
void Technique_Shadow_Render::Setup_Matrix_forRender(pipeline& p, int RenderPassID)
{
    Load_glUniform(Matrices_UniLoc[RenderPassID], p);
}
*/


/*
void Technique_Shadow_Render::Setup_Matrix_forRender1(pipeline& p)
{
    Load_glUniform(Matrices_UniLoc[RENDER_PASS1], p);
}


void Technique_Shadow_Render::Setup_Matrix_forRender2(pipeline& p)
{
    Load_glUniform(Matrices_UniLoc[RENDER_PASS2], p);
//    Load_glUniform(Matrices_Loc2, p);
}
*/


/*
void Technique_Shadow_Render::Setup_Matrix_forRender(pipeline& p, int RenderPassID)
{
    if(RenderPassID == RENDER_PASS2)
    {
		glUniformMatrix4fv(LightMVPmatrix_UniLoc,1,GL_FALSE,&shadowMatrix[0][0]);
		glUniform1i(shadowMap_UniLoc,"shadowMap"),0);

        glUniformMatrix4fv(LightPosition_MVmatrix_UniLoc,1,GL_FALSE,&LightPos_modelViewMatrix[0][0]);
		glUniform3f(LightPosition_UniLoc,lightPosition.x,lightPosition.y,lightPosition.z);
		glUniform3f(CameraPosition_UniLoc,cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);
    }
    Load_glUniform(Matrices_UniLoc[RenderPassID], p);
}
*/



#endif
