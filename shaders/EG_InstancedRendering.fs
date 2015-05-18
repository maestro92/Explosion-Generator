#version 330

#include "BaseLight.hs"

const int MAX_POINT_LIGHTS = 2;   
const int MAX_SPOT_LIGHTS = 2;   


in vec3 outWorldPos;
in vec3 outNormal;
in vec3 outColor; 

out vec4 FragColor;



uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
// uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];  

uniform sampler2D gSampler;

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;
uniform vec4 gColor[4];   
        

vec4 CalcLightInternal(BaseLight Light,
                       vec3 LightDirection,
                       vec3 WorldPos,
                       vec3 Normal)
{
    // ambient light
    vec4 AmbientColor = vec4(Light.color, 1.0) * Light.ambientIntensity;
    
    // diffuse light
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0.0) 
    {

        DiffuseColor = vec4(Light.color, 1.0) * Light.diffuseIntensity * DiffuseFactor;

        // specular light
        vec3 EyeToVertex = normalize(gEyeWorldPos - WorldPos);
//        vec3 EyeToVertex = normalize(WorldPos);
        vec3 reflection = reflect(EyeToVertex,Normal);
        reflection = normalize(reflection);
        float SpecularFactor = dot(reflection, LightDirection);
        SpecularFactor = pow(SpecularFactor, gSpecularPower);
        
        if (SpecularFactor > 0.0) 
            SpecularColor = vec4(Light.color, 1.0) * gMatSpecularIntensity * SpecularFactor;   
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}



vec4 CalcDirectionalLight(vec3 WorldPos, vec3 Normal)
{
    return CalcLightInternal(gDirectionalLight.base,
                             gDirectionalLight.direction,
                             WorldPos,
                             Normal);
}
          


       
                                                                                            
void main()                                                                                 
{ 

    vec3 normal = normalize(outNormal);
    vec4 TotalLight = CalcDirectionalLight(outWorldPos, normal);
    FragColor = TotalLight;
}


