#ifndef EG_ALL_LIGHTS_H_
#define EG_ALL_LIGHTS_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include "define.h"


#include "EG_DirectionalLight.h"
#include "EG_PointLight.h"
#include "EG_SpotLight.h"
#include "Random.h"

using namespace std;

//const static float m_specularIntensity;
//const static float m_specularPower;

const static float SPECULAR_INTENSITY = 1.0f;
const static float SPECULAR_POWER = 32.0f;


class EG_AllLights
{
    public:
        EG_AllLights();
        ~EG_AllLights();

        void init();

        void setSportLights();
        void setDirectionalLight();
        void setPointLights();

        vector<EG_SpotLight> getSpotLights();
        EG_SpotLight getSpotLight(int index);
        int getSpotLightsCount();

        vector<EG_DirectionalLight> getDirectionalLights();
        EG_DirectionalLight& getDirectionalLight(int index);
        int getDirecitonalLightsCount();

        vector<EG_PointLight> getPointLights();
        EG_PointLight& getPointLight(int index);
        int getPointLightsCount();
        void addPointLight();
        void initPointLightWall();

    private:
        vector<EG_SpotLight> spotLights;
        vector<EG_DirectionalLight> directionalLights;
        vector<EG_PointLight> pointLights;

        int spotLightsCount;
        int directionalLightsCount;
        int pointLightsCount;
/*
        EG_SpotLight* spotLights;
        EG_DirectionalLight* directionalLights;
        EG_PointLight* pointLights;
*/

};



#endif // EG_ALL_LIGHTS_H_

