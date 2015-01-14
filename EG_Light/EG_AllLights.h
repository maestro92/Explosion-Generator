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

using namespace std;

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

        vector<EG_DirectionalLight> getDirectionalLights();
        EG_DirectionalLight getDirectionalLight(int index);

        vector<EG_PointLight> getPointLights();
        EG_PointLight getPointLights(int index);

    private:
        vector<EG_SpotLight> spotLights;
        vector<EG_DirectionalLight> directionalLights;
        vector<EG_PointLight> pointLights;
/*
        EG_SpotLight* spotLights;
        EG_DirectionalLight* directionalLights;
        EG_PointLight* pointLights;
*/

};



#endif // EG_ALL_LIGHTS_H_

