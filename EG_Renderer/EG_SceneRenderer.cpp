
#include "EG_SceneRenderer.h"


EG_SceneRenderer::EG_SceneRenderer()
{}



EG_SceneRenderer::~EG_SceneRenderer()
{}


/*
void EG_SceneRenderer::init(int w, int h, int Shader_Num)
{
    allocateMemberVariables(Shader_Num);

    ShadowMapWidth = w*2;
    ShadowMapHeight = h*2;

    /// FirstPass_LightPOV
    /// SecondPass_CameraPOV
//    progShaders[RENDER_PASS1] = new Shader("shadow_FirstRender.vs", "shadow_FirstRender.fs");
//    progShaders[RENDER_PASS2] = new Shader("shadow_SecondRender.vs", "shadow_SecondRender.fs");

    progShaders[RENDER_PASS1] = new Shader("shadow_FirstRender.vs", "shadow_FirstRender.fs");
    progShaders[RENDER_PASS2] = new Shader("shadow_SecondRender.vs", "shadow_SecondRender.fs");



  //  ShadowMatrix_UniLoc[RENDER_PASS1] = GetUniformLocation(progShaders[RENDER_PASS1], "l_modelViewProjectionMatrix");
  //  ShadowMatrix_UniLoc[RENDER_PASS2] = GetUniformLocation(progShaders[RENDER_PASS2],  "l_modelViewProjectionMatrix");

    LightMVPmatrix_UniLoc =             GetUniformLocation(progShaders[RENDER_PASS2], "l_modelViewProjectionMatrix");
    shadowMap_UniLoc =                  GetUniformLocation(progShaders[RENDER_PASS2], "shadowMap");
    LightPosition_MVmatrix_UniLoc =     GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ModelViewMatrix");
    LightPosition_UniLoc =              GetUniformLocation(progShaders[RENDER_PASS2], "lightPosition_ObjectSpace");
    CameraPosition_UniLoc =             GetUniformLocation(progShaders[RENDER_PASS2], "cameraPosition");

    initMemberVariables();
}
*/


void EG_SceneRenderer::init(int nShaders)
{

}


void EG_SceneRenderer::init(int w, int h, int Shader_Num)
{
    allocateMemberVariables(Shader_Num);


    /// FirstPass_LightPOV
    /// SecondPass_CameraPOV
    m_shaders[RENDER_PASS1] = new Shader("shadow_FirstRender.vs", "shadow_FirstRender.fs");
//    m_shaders[RENDER_PASS2] = new Shader("shadow_SecondRender_WithManyLights.vs", "shadow_SecondRender_WithManyLights.fs");
 //   m_shaders[RENDER_PASS2] = new Shader("shadow_SecondRender_dir_pt_include.vs", "shadow_SecondRender_dir_pt_include.fs");
    m_shaders[RENDER_PASS2] = new Shader("EG_SceneRenderer.vs", "EG_SceneRenderer.fs");
//    l_modelViewProjectionMatrix_UniLoc_ =       GetUniformLocation(progShaders[RENDER_PASS1], "l_modelViewProjectionMatrix");
//    lightPosition_ModelViewMatrix_UniLoc_ =     GetUniformLocation(progShaders[RENDER_PASS1], "lightPosition_ModelViewMatrix");
//    lightPosition_ObjectSpace_UniLoc_ =         GetUniformLocation(progShaders[RENDER_PASS1], "lightPosition");

//    m_shadowMapTextureUnitLocation  =       GetUniformLocation(progShaders[RENDER_PASS1], "gShadowMap");
    initDirLightUniformLocations(RENDER_PASS2);
    initPointLightUniformLocations(RENDER_PASS2);
    initSpotLightUniformLocations(RENDER_PASS2);

    l_modelViewProjectionMatrix_UniLoc_     = GetUniformLocation( m_shaders[RENDER_PASS2], "l_modelViewProjectionMatrix");
    shadowMap_UniLoc                        = GetUniformLocation( m_shaders[RENDER_PASS2], "gShadowMap");
    initMemberVariables();


}


void EG_SceneRenderer::loadUniformLocations(pipeline& p, int pass)
{
    p.updateShadowMatrix();
//    p.shadowMatrix = LIGHT_BIAS_MATRIX * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
    glUniformMatrix4fv(l_modelViewProjectionMatrix_UniLoc_,1,GL_FALSE,&p.m_shadowMatrix[0][0]);

    if(tables.size() != 0)
    {
        for ( auto local_it = tables[RENDER_PASS1].begin(); local_it!= tables[RENDER_PASS1].end(); ++local_it )
            (local_it->second)->setUniLoc();
    }

    EG_Renderer::loadUniformLocations(p, pass);
}



void EG_SceneRenderer::initDirLightUniformLocations(int pass)
{

    m_dirLightLocation.color                = GetUniformLocation( m_shaders[pass], "gDirectionalLight.base.color");
    m_dirLightLocation.ambientIntensity     = GetUniformLocation( m_shaders[pass], "gDirectionalLight.base.ambientIntensity");
    m_dirLightLocation.diffuseIntensity     = GetUniformLocation( m_shaders[pass], "gDirectionalLight.base.diffuseIntensity");
    m_dirLightLocation.direction            = GetUniformLocation( m_shaders[pass], "gDirectionalLight.direction");

    m_eyeWorldPosLocation                   = GetUniformLocation( m_shaders[pass], "gEyeWorldPos");
    m_matSpecularIntensityLocation          = GetUniformLocation( m_shaders[pass], "gMatSpecularIntensity");
    m_matSpecularPowerLocation              = GetUniformLocation( m_shaders[pass], "gSpecularPower");

}

void EG_SceneRenderer::initPointLightUniformLocations(int pass)
{
    gNumPointLights = GetUniformLocation( m_shaders[pass], "gNumPointLights");

    for (unsigned int i = 0 ; i < 103 ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gPointLights[%d].base.color", i);
        m_pointLightsLocation[i].color = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].base.ambientIntensity", i);
        m_pointLightsLocation[i].ambientIntensity = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].position", i);
        m_pointLightsLocation[i].position = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].base.diffuseIntensity", i);
        m_pointLightsLocation[i].diffuseIntensity = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].atten.constant", i);
        m_pointLightsLocation[i].attenuation.constant = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].atten.linear", i);
        m_pointLightsLocation[i].attenuation.linear = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].atten.exp", i);
        m_pointLightsLocation[i].attenuation.exp = GetUniformLocation( m_shaders[pass], Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].scale", i);
        m_pointLightsLocation[i].scale = GetUniformLocation( m_shaders[pass], Name);
    }

}



void EG_SceneRenderer::initSpotLightUniformLocations(int pass)
{

}




/*
void EG_SceneRenderer::loadUniformLocations1(pipeline& p, int pass)
{
    p.updateShadowMatrix();
    glUniformMatrix4fv(l_modelViewProjectionMatrix_UniLoc_,1,GL_FALSE,&p.m_shadowMatrix[0][0]);


    for ( auto local_it = tables[RENDER_PASS1].begin(); local_it!= tables[RENDER_PASS1].end(); ++local_it )
        (local_it->second)->setUniLoc();

    EG_Renderer::loadUniformLocations(p, pass);
}
*/


void EG_SceneRenderer::setDirectionalLight(EG_DirectionalLight& Light)
{
    /*
    glUniform3f(m_dirLightLocation.color, Light.color.x, Light.color.y, Light.color.z);
    glUniform1f(m_dirLightLocation.ambientIntensity, Light.ambientIntensity);
    glm::vec3 direction = Light.direction;
    direction = glm::normalize(direction);
    glUniform3f(m_dirLightLocation.direction, direction.x, direction.y, direction.z);
    glUniform1f(m_dirLightLocation.diffuseIntensity, Light.diffuseIntensity);
*/

    setUniformLocation(m_dirLightLocation.color, Light.color);
    setUniformLocation(m_dirLightLocation.ambientIntensity, Light.ambientIntensity);
    glm::vec3 direction = Light.direction;
    direction = glm::normalize(direction);
    setUniformLocation(m_dirLightLocation.direction, direction);
    setUniformLocation(m_dirLightLocation.diffuseIntensity, Light.diffuseIntensity);
}




void EG_SceneRenderer::setPointLight(int index, EG_PointLight& Light)
{
    setUniformLocation(m_pointLightsLocation[index].color, Light.color);
    setUniformLocation(m_pointLightsLocation[index].ambientIntensity, Light.ambientIntensity);
    setUniformLocation(m_pointLightsLocation[index].diffuseIntensity, Light.diffuseIntensity);
    setUniformLocation(m_pointLightsLocation[index].position, Light.position);
    setUniformLocation(m_pointLightsLocation[index].attenuation.constant, Light.attenuation.constant);
    setUniformLocation(m_pointLightsLocation[index].attenuation.linear, Light.attenuation.linear);
    setUniformLocation(m_pointLightsLocation[index].attenuation.exp, Light.attenuation.exp);
    float scalef = computePointLightBoundingSphere(Light);
    setUniformLocation(m_pointLightsLocation[index].scale, scalef);

/*
    glUniform3f(m_pointLightsLocation[index].color, Light.color.x, Light.color.y, Light.color.z);
    glUniform1f(m_pointLightsLocation[index].ambientIntensity, Light.ambientIntensity);
    glUniform1f(m_pointLightsLocation[index].diffuseIntensity, Light.diffuseIntensity);
    glUniform3f(m_pointLightsLocation[index].position, Light.position.x, Light.position.y, Light.position.z);
    glUniform1f(m_pointLightsLocation[index].attenuation.constant, Light.attenuation.constant);
    glUniform1f(m_pointLightsLocation[index].attenuation.linear, Light.attenuation.linear);
    glUniform1f(m_pointLightsLocation[index].attenuation.exp, Light.attenuation.exp);
    float scalef = computePointLightBoundingSphere(Light);
 //   cout << "scalef is " << scalef << endl;
    glUniform1f(m_pointLightsLocation[index].scale, scalef);
*/
}



float EG_SceneRenderer::computePointLightBoundingSphere(EG_PointLight& Light)
{
    float MaxChannel = fmax(fmax(Light.color.x, Light.color.y), Light.color.z);

    float ret = (-Light.attenuation.linear + sqrtf(Light.attenuation.linear * Light.attenuation.linear - 4 * Light.attenuation.exp * (Light.attenuation.exp - 256 * MaxChannel * Light.diffuseIntensity)))
                /
                2 * Light.attenuation.exp;
    return ret;
}


void EG_SceneRenderer::setPointLightCount(int count)
{
//	glUniform1i(gNumPointLights, count);
	setUniformLocation(gNumPointLights, count);
}




void EG_SceneRenderer::setEyeWorldPos(const glm::vec3& EyePos)
{
//	glUniform3f(m_eyeWorldPosLocation, EyePos.x, EyePos.y, EyePos.z);
	setUniformLocation(m_eyeWorldPosLocation, EyePos);
}

/*
void EG_SceneRenderer::setScreenSize(unsigned int Width, unsigned int Height)
{
    glUniform2f(m_screenSizeLocation, (float)Width, (float)Height);
}
*/

void EG_SceneRenderer::setMatSpecularIntensity(float Intensity)
{
//    glUniform1f(m_matSpecularIntensityLocation, Intensity);
    setUniformLocation(m_matSpecularIntensityLocation, Intensity);
}

void EG_SceneRenderer::setMatSpecularPower(float Power)
{
//    glUniform1f(m_matSpecularPowerLocation, Power);
    setUniformLocation(m_matSpecularPowerLocation, Power);

}



/*
void EG_SceneRenderer::loadSpecialUniformLocation(pipeline& p, int RenderPassID)
{
    p.shadowMatrix = p.Light_BiasMatrix * p.Light_ProjectionMatrix * p.Light_ViewMatrix * p.modelMatrix[p.modelMatrix.size()-1];
    glUniformMatrix4fv(LightMVPmatrix_UniLoc,1,GL_FALSE,&p.shadowMatrix[0][0]);
}
*/




#if 0
void EG_SceneRenderer::Setup_Matrix_forRender1(pipeline& p)
{
    Load_glUniform(Matrices_Loc1, p);
}


void EG_SceneRenderer::Setup_Matrix_forRender2(pipeline& p)
{
    Load_glUniform(Matrices_Loc2, p);
}

#else

/*
void EG_SceneRenderer::Setup_Matrix_forRender(pipeline& p, int RenderPassID)
{
    Load_glUniform(Matrices_UniLoc[RenderPassID], p);
}
*/


/*
void EG_SceneRenderer::Setup_Matrix_forRender1(pipeline& p)
{
    Load_glUniform(Matrices_UniLoc[RENDER_PASS1], p);
}


void EG_SceneRenderer::Setup_Matrix_forRender2(pipeline& p)
{
    Load_glUniform(Matrices_UniLoc[RENDER_PASS2], p);
//    Load_glUniform(Matrices_Loc2, p);
}
*/


/*
void EG_SceneRenderer::Setup_Matrix_forRender(pipeline& p, int RenderPassID)
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
