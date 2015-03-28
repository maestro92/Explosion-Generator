

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
    pointLights = vector<EG_PointLight>(4, EG_PointLight());

    spotLights[0].ambientIntensity = 0.0f;
    spotLights[0].diffuseIntensity = 0.9f;
    spotLights[0].color = COLOR_WHITE;
    spotLights[0].attenuation.linear = 0.01f;
    spotLights[0].position  = glm::vec3(-20.0, 20.0, 5.0f);
    spotLights[0].direction = glm::vec3(1.0f, -1.0f, 0.0f);
    spotLights[0].cutoff =  20.0f;

    directionalLights[0].ambientIntensity = 0.01f;
    directionalLights[0].color = COLOR_WHITE;
    directionalLights[0].diffuseIntensity = 0.5f;
//    directionalLights[0].direction = glm::vec3(1.0f, -1.0f, 0.0f);
    directionalLights[0].direction = glm::vec3(19.1004, -28.881, -40.5246) * 2.0f;



    pointLights[0].color = COLOR_BLUE;
    pointLights[0].diffuseIntensity = 20.0f;
    pointLights[0].position = glm::vec3(0.0f, 0.0f, 3.0f);
    pointLights[0].attenuation.constant = 0.5f;
    pointLights[0].attenuation.linear = 0.5f;
    pointLights[0].attenuation.exp = 0.5f;

    pointLights[1].color = COLOR_GREEN;
    pointLights[1].diffuseIntensity = 15.0f;
    pointLights[1].position = glm::vec3(0.0f, 1.5f, 5.0f);
    pointLights[1].attenuation.constant = 0.5f;
    pointLights[1].attenuation.linear = 0.5f;
    pointLights[1].attenuation.exp = 0.5f;

    pointLights[2].color = COLOR_CYAN;
    pointLights[2].diffuseIntensity = 15.0f;
    pointLights[2].position = glm::vec3(2.0f, 0.0f, 5.0f);
    pointLights[2].attenuation.constant = 0.5f;
    pointLights[2].attenuation.linear = 0.5f;
    pointLights[2].attenuation.exp = 0.5f;

    pointLights[3].color = COLOR_RED;
    pointLights[3].diffuseIntensity = 15.0f;
    pointLights[3].position = glm::vec3(-2.0f, 2.0f, -2.0f);
    pointLights[3].attenuation.constant = 0.5f;
    pointLights[3].attenuation.linear = 0.5f;
    pointLights[3].attenuation.exp = 0.5f;


 //   initPointLightWall();
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

/*
void EG_AllLights::addPointLight()
{
    EG_PointLight::EG_PointLight(   glm::vec3 Color, float AmbientIntensity, float DiffuseIntensity,
                                glm::vec3 Position, float Constant, float Linear, float Exp)

    EG_PointLight light(COLOR_RED, 0.0f, 5.0f, );
    pointLights.push_back();
}

    pointLights[3].color = COLOR_RED;
    pointLights[3].diffuseIntensity = 15.0f;
    pointLights[3].position = glm::vec3(-2.0f, 2.0f, -2.0f);
    pointLights[3].attenuation.constant = 0.0f;
    pointLights[3].attenuation.linear = 0.0f;
    pointLights[3].attenuation.exp = 0.3f;
*/


void EG_AllLights::initPointLightWall()
{
//    EG_PointLight::EG_PointLight(   glm::vec3 Color, float AmbientIntensity, float DiffuseIntensity,
//                                glm::vec3 Position, float Constant, float Linear, float Exp)

    EG_PointLight pLight;

    int iterations = 25;
    float interval = 50/(float)iterations;


    /// -z side
    for (int i=0; i<iterations; i++)
//    for (int i=0; i<6; i++)
    {


     //   if(i!=3)
     //       continue;
        glm::vec3 col = ColorList[i%6];
   //     glm::vec3 pos(-25.0f+i*4, 10.0f, -24.0f);
    //     glm::vec3 pos(-25.0f+i*10 - 0.5, 5.0f, -24.0f);


/*
        float amount = -25.0f+i*10 - 0.5;
        glm::vec3 pos(amount, 5.0f, -25.0f + amount-2);
//        float DiffuseInt = RandRange(1.0f, 30.0f);

//        float DiffuseInt = RandRange(1.0f, 30.0f);
        float DiffuseInt = 10.0f;

        pLight = EG_PointLight(col, 0.0f, DiffuseInt, pos, 0.5f, 0.5f, 0.5f);
        pointLights.push_back(pLight);
*/


    //    float amount = -25.0f+i*10 - 0.5;
        float amount = -25.0f+i*interval  - 0.5;
        glm::vec3 pos(amount, 5.0f, -24.0f);
//        glm::vec3 pos(amount, 5.0f, -25.0f + amount-2);
      //  float DiffuseInt = 10.0f;
     //   float DiffuseInt = RandRange(1.0f, 30.0f);
        float DiffuseInt = RandRange(1.0f, 5.0f);
        pLight = EG_PointLight(col, 0.0f, DiffuseInt, pos, 0.5f, 0.5f, 0.5f);
        pointLights.push_back(pLight);

    }

    /// +z side
    for (int i=0; i<25; i++)
    {
        glm::vec3 col = ColorList[i%6];
        float amount = -25.0f+i*2 - 0.5;

        glm::vec3 pos(amount, 1.0f, 24.0f);
//        glm::vec3 pos(amount, 5.0f, -25.0f + amount-2);
       // float DiffuseInt = 10.0f;
     //   float DiffuseInt = RandRange(1.0f, 30.0f);
        float DiffuseInt = RandRange(1.0f, 5.0f);
        pLight = EG_PointLight(col, 0.0f, DiffuseInt, pos, 0.5f, 0.5f, 0.5f);
        pointLights.push_back(pLight);

    }


    /// +x Side
    for (int i=0; i<25; i++)
    {
        glm::vec3 col = ColorList[i%6];

    //    float amount = -25.0f+i*10 - 0.5;
   //     float amount =
        glm::vec3 pos(24.5f, 5.0f, (-25.0f+i*2 - 0.5));
//        glm::vec3 pos(amount, 5.0f, -25.0f + amount-2);
       // float DiffuseInt = 10.0f;
     //   float DiffuseInt = RandRange(1.0f, 30.0f);
        float DiffuseInt = RandRange(1.0f, 5.0f);
        pLight = EG_PointLight(col, 0.0f, DiffuseInt, pos, 0.5f, 0.5f, 0.5f);
        pointLights.push_back(pLight);

    }


    /// -x side
    for (int i=0; i<25; i++)
    {
        glm::vec3 col = ColorList[i%6];

    //    float amount = -25.0f+i*10 - 0.5;
   //     float amount =
        glm::vec3 pos(-24.5f, 1.0f, (-25.0f+i*2 - 0.5));
//        glm::vec3 pos(amount, 5.0f, -25.0f + amount-2);
       // float DiffuseInt = 10.0f;
     //   float DiffuseInt = RandRange(1.0f, 30.0f);
        float DiffuseInt = RandRange(1.0f, 5.0f);
        pLight = EG_PointLight(col, 0.0f, DiffuseInt, pos, 0.5f, 0.5f, 0.5f);
        pointLights.push_back(pLight);
    }

}




/// SpotLights
vector<EG_SpotLight> EG_AllLights::getSpotLights()
{
    return spotLights;
}

EG_SpotLight EG_AllLights::getSpotLight(int index)
{
    return spotLights[index];
}

int EG_AllLights::getSpotLightsCount()
{
    spotLightsCount = spotLights.size();
    return spotLightsCount;
}


/// Directional Lights
vector<EG_DirectionalLight> EG_AllLights::getDirectionalLights()
{
    return directionalLights;
}

EG_DirectionalLight& EG_AllLights::getDirectionalLight(int index)
{
    return directionalLights[index];
}

int EG_AllLights::getDirecitonalLightsCount()
{
    directionalLightsCount = directionalLights.size();
    return directionalLightsCount;
}


/// PointLights
vector<EG_PointLight> EG_AllLights::getPointLights()
{
    return pointLights;
}

EG_PointLight& EG_AllLights::getPointLight(int index)
{
    return pointLights[index];
}

int EG_AllLights::getPointLightsCount()
{
    pointLightsCount = pointLights.size();
    return pointLightsCount;
}
