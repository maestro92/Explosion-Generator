
#include "Lighting_Technique.h"


Lighting_Technique::Lighting_Technique()
{

}

void Lighting_Technique::Init(unsigned int shaderID)
{

    m_dirLightLocation.Direction = glGetUniformLocation( shaderID, "gDirectionalLight.Direction");
    m_dirLightLocation.Color = glGetUniformLocation( shaderID, "gDirectionalLight.Base.Color");
    m_dirLightLocation.AmbientIntensity = glGetUniformLocation( shaderID, "gDirectionalLight.Base.AmbientIntensity");
    m_dirLightLocation.DiffuseIntensity = glGetUniformLocation( shaderID, "gDirectionalLight.Base.DiffuseIntensity");
    m_matSpecularIntensityLocation = glGetUniformLocation( shaderID, "gMatSpecularIntensity");
    m_matSpecularPowerLocation = glGetUniformLocation( shaderID, "gMatSpecularPower");

    m_numPointLightsLocation = glGetUniformLocation( shaderID, "gNumPointLights");
    m_numSpotLightsLocation = glGetUniformLocation( shaderID, "gNumSpotLights");



    cout << " nice " << endl;
    /*
    if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
        m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION||
        m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION)
    {
        cout << " error for directional light " << endl;
        exit(1);
    }
*/


    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation) ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
        m_pointLightsLocation[i].Color = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.MV_Matrix", i);
        m_pointLightsLocation[i].MV_MatrixLocation = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
        m_pointLightsLocation[i].AmbientIntensity = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
        m_pointLightsLocation[i].Position = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
        m_pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
        m_pointLightsLocation[i].Atten.Constant = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
        m_pointLightsLocation[i].Atten.Linear = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
        m_pointLightsLocation[i].Atten.Exp = glGetUniformLocation( shaderID, Name);


/*
        if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION)
        {
            cout << "error for mainshader pointLight " << endl;
            exit(1);
        }
*/


    }




    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation) ; i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Color", i);
        m_spotLightsLocation[i].Color = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.MV_Matrix", i);
        m_spotLightsLocation[i].MV_MatrixLocation = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Normal_Matrix", i);
        m_spotLightsLocation[i].Normal_MatrixLocation = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.AmbientIntensity", i);
        m_spotLightsLocation[i].AmbientIntensity = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.DiffuseIntensity", i);
        m_spotLightsLocation[i].DiffuseIntensity = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
        m_spotLightsLocation[i].Direction = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Position", i);
        m_spotLightsLocation[i].Position = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Atten.Constant", i);
        m_spotLightsLocation[i].Atten.Constant = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Atten.Linear", i);
        m_spotLightsLocation[i].Atten.Linear = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Atten.Exp", i);
        m_spotLightsLocation[i].Atten.Exp = glGetUniformLocation( shaderID, Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
        m_spotLightsLocation[i].Cutoff = glGetUniformLocation( shaderID, Name);

/*
        if (m_spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION)
        {
            cout << "error for mainshader spotlight " << endl;
            exit(1);
        }
*/
    }


}


void Lighting_Technique::SetDirectionalLight(const DirectionalLight& Light)
{
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
    glm::vec3 Direction = Light.Direction;
    Direction = glm::normalize(Direction);
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void Lighting_Technique::SetMatSpecularIntensity(float Intensity)
{
    glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void Lighting_Technique::SetMatSpecularPower(float Power)
{
    glUniform1f(m_matSpecularPowerLocation, Power);
}

void Lighting_Technique::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
    glUniform1i(m_numPointLightsLocation, NumLights);

    for (unsigned int i = 0 ; i < NumLights ; i++)
    {
        glUniformMatrix4fv(m_pointLightsLocation[i].MV_MatrixLocation,1,GL_FALSE,&pLights[i].MV_Matrix[0][0]);
        glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
        glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
}





void Lighting_Technique::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
    glUniform1i(m_numSpotLightsLocation, NumLights);

    for (unsigned int i = 0 ; i < NumLights ; i++)
    {
        glUniformMatrix4fv(m_spotLightsLocation[i].MV_MatrixLocation,1,GL_FALSE,&pLights[i].MV_Matrix[0][0]);
        glUniformMatrix4fv(m_spotLightsLocation[i].Normal_MatrixLocation,1,GL_FALSE,&pLights[i].Normal_Matrix[0][0]);

        glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_spotLightsLocation[i].Position,  pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glm::vec3 Direction = pLights[i].Direction;
        Direction = glm::normalize(Direction);
        glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
        glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(ToRadian(pLights[i].Cutoff)));
        glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(m_spotLightsLocation[i].Atten.Linear,   pLights[i].Attenuation.Linear);
        glUniform1f(m_spotLightsLocation[i].Atten.Exp,      pLights[i].Attenuation.Exp);
    }
}

void Lighting_Technique::LoadingModeViewMatrix_ForPointLights(pipeline& m_pipeline, PointLight* pLights, glm::mat4 View_Matrix, unsigned int index)
{
    m_pipeline.pushMatrix();
        m_pipeline.translate(pLights[index].Position.x, pLights[index].Position.y, pLights[index].Position.z);
        glm::mat4 PointLightPos_modelMatrix = m_pipeline.getModelMatrix();
    m_pipeline.popMatrix();
 //   m_LightingEffect->PointLightPos_modelViewMatrix = LightPos_viewMatrix * PointLightPos_modelMatrix;
    pLights[index].MV_Matrix = View_Matrix * PointLightPos_modelMatrix;
}


void Lighting_Technique::LoadingModeViewMatrix_ForSpotLights(pipeline& m_pipeline, SpotLight* pLights, glm::mat4 View_Matrix, unsigned int index)
{
    m_pipeline.pushMatrix();
        m_pipeline.translate(pLights[index].Position.x, pLights[index].Position.y, pLights[index].Position.z);
        glm::mat4 Pos_modelMatrix = m_pipeline.getModelMatrix();
    m_pipeline.popMatrix();
 //   m_LightingEffect->PointLightPos_modelViewMatrix = LightPos_viewMatrix * PointLightPos_modelMatrix;
    pLights[index].MV_Matrix = View_Matrix * Pos_modelMatrix;
    pLights[index].Normal_Matrix = glm::mat3(View_Matrix * Pos_modelMatrix);
}
