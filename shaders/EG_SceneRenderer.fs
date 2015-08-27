#version 330
 


#include "BaseLight.hs"
/*
struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation atten;
    float scale;
};
*/

const int MAX_POINT_LIGHTS = 103;  


uniform DirectionalLight gDirectionalLight;

uniform int gNumPointLights;          
uniform PointLight gPointLights[MAX_POINT_LIGHTS];


in vec3 outVertex;
in vec4 lightVertexPosition;       // vertex seen in light's clip space
in vec3 outNormal;
in vec3 outTangent;
in vec3 outColor;


uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat4 m_ViewMatrix;
uniform mat4 m_ProjectionMatrix;
uniform mat4 m_ModelMatrix;
uniform mat4 m_ViewNoRotateMatrix;
uniform mat3 m_normalMatrix;                               
uniform mat4 l_modelViewProjectionMatrix;


uniform sampler2D gShadowMap;
// uniform sampler2DShadow gShadowMap;
uniform vec3 gEyeWorldPos;                                                                  
uniform float gMatSpecularIntensity;                                                        
uniform float gSpecularPower;   

 
out vec4 FragColor;
 
const vec3 ambientColor=vec3(0.01,0.01,0.01);
const vec3 diffuseColor=vec3(0.9,0.9,0.9);
const vec3 specularColor=vec3(1.0,1.0,1.0);

float Cutoff = 0.9857;  // spotlight region cutoff value     
// float Cutoff = 0.9917;  // spotlight region cutoff value    
// float Cutoff = 0.9517;  // spotlight region cutoff value     

bool flag = false;

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
    else
        flag = true;

    return (AmbientColor + DiffuseColor + SpecularColor);
}



vec4 CalcDirectionalLight(vec3 WorldPos, vec3 Normal)
{
    return CalcLightInternal(gDirectionalLight.base,
                             gDirectionalLight.direction,
                             WorldPos,
                             Normal);
}



vec4 CalcPointLight(int Index, vec3 Normal)                                                 
{                                                                                           
    vec3 LightDirection = outVertex - gPointLights[Index].position;                         
    float distance = length(LightDirection);            
    
    if(distance > gPointLights[Index].scale)  
        return vec4(0.0,0.0,0.0,1.0);

                                        
    LightDirection = normalize(LightDirection);                                             
                                                                                            
    vec4 Color = CalcLightInternal(gPointLights[Index].base, LightDirection, outVertex, Normal);       
    float Attenuation =  gPointLights[Index].atten.constant +                               
                         gPointLights[Index].atten.linear * distance +                      
                         gPointLights[Index].atten.exp * distance * distance;               
                                                                                            
    return Color / Attenuation;                                                             
}                                                                                           
   



void main()
{

    vec3 normal = normalize(outNormal);
 
    float shadowValue = 0.0;


    vec4 lightVertexPosition2 = lightVertexPosition;
    lightVertexPosition2 /= lightVertexPosition2.w;

    if ( texture2D( gShadowMap, lightVertexPosition2.xy ).r  >=  lightVertexPosition2.z)
        shadowValue = 1.0;


               
                                                     
    vec4 DirColor = vec4(outColor, 1.0) * CalcDirectionalLight(outVertex, normal); 
   
    vec4 PtColor = vec4(0.0,0.0,0.0,1.0);                                      
    for (int i = 0 ; i < gNumPointLights ; i++) 
        PtColor += CalcPointLight(i, normal);      
    
    vec4 TotalLight = DirColor + PtColor;
    

    PtColor = vec4(outColor,1.0) * PtColor;
    
      
    FragColor = (shadowValue == 0.0) ? vec4(0.1,0.1,0.1,1.0) * TotalLight : vec4(outColor, 1.0) * TotalLight;
}



