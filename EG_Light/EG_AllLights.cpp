

#include "EG_AllLights.h"

EG_AllLights::EG_AllLights()
{

}

EG_AllLights::~EG_AllLights()
{

}

void EG_AllLights::init()
{
    spotLights = vector<EG_SpotLight>(1, EG_SpotLight());
    directionalLights = vector<EG_DirectionalLight>(1, EG_DirectionalLight());
    pointLights = vector<EG_PointLight>(3, EG_PointLight());

    spotLights[0].ambientIntensity = 0.0f;
    spotLights[0].diffuseIntensity = 0.9f;
    spotLights[0].color = COLOR_WHITE;
    spotLights[0].attenuation.linear = 0.01f;
    spotLights[0].position  = glm::vec3(-20.0, 20.0, 5.0f);
    spotLights[0].direction = glm::vec3(1.0f, -1.0f, 0.0f);
    spotLights[0].cutoff =  20.0f;

    directionalLights[0].ambientIntensity = 0.01f;
    directionalLights[0].color = COLOR_WHITE;
    directionalLights[0].diffuseIntensity = 0.9f;
    directionalLights[0].direction = glm::vec3(1.0f, -1.0f, 0.0f);

    pointLights[0].diffuseIntensity = 0.2f;
    pointLights[0].color = COLOR_GREEN;
    pointLights[0].position = glm::vec3(0.0f, 1.5f, 5.0f);
    pointLights[0].attenuation.constant = 0.0f;
    pointLights[0].attenuation.linear = 0.0f;
    pointLights[0].attenuation.exp = 0.3f;

    pointLights[1].diffuseIntensity = 0.2f;
    pointLights[1].color = COLOR_RED;
    pointLights[1].position = glm::vec3(2.0f, 0.0f, 5.0f);
    pointLights[1].attenuation.constant = 0.0f;
    pointLights[1].attenuation.linear = 0.0f;
    pointLights[1].attenuation.exp = 0.3f;

    pointLights[2].diffuseIntensity = 0.2f;
    pointLights[2].color = COLOR_BLUE;
    pointLights[2].position = glm::vec3(0.0f, 0.0f, 3.0f);
    pointLights[2].attenuation.constant = 0.0f;
    pointLights[2].attenuation.linear = 0.0f;
    pointLights[2].attenuation.exp = 0.3f;

}

void EG_AllLights::setSportLights()
{

}

void EG_AllLights::setDirectionalLight()
{

}

void EG_AllLights::setPointLights()
{

}

vector<EG_SpotLight> EG_AllLights::getSpotLights()
{
    return spotLights;
}

EG_SpotLight EG_AllLights::getSpotLight(int index)
{
    return spotLights[index];
}

vector<EG_DirectionalLight> EG_AllLights::getDirectionalLights()
{
    return directionalLights;
}

EG_DirectionalLight EG_AllLights::getDirectionalLight(int index)
{
    return directionalLights[index];
}

vector<EG_PointLight> EG_AllLights::getPointLights()
{
    return pointLights;
}

EG_PointLight EG_AllLights::getPointLights(int index)
{
    return pointLights[index];
}

