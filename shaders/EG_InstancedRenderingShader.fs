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
          

/*
vec4 CalcPointLight(int Index, vec3 Normal)                                                 
{                                                                                           
    vec3 LightDirection = WorldPos0 - gPointLights[Index].Position;                         
    float Distance = length(LightDirection);                                                
    LightDirection = normalize(LightDirection);                                             
                                                                                            
    vec4 Color = CalcLightInternal(gPointLights[Index].Base, LightDirection, Normal);       
    
    float Attenuation =  gPointLights[Index].Atten.Constant +                               
                         gPointLights[Index].Atten.Linear * Distance +                      
                         gPointLights[Index].Atten.Exp * Distance * Distance;               
                                                                                            
    return Color / Attenuation;                                                             
}                                                                                           
 */      
       
      /*
vec4 CalcPointLight(PointLight l, vec3 Normal)
{
    vec3 LightDirection = WorldPos0 - l.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal);
    float Attenuation = l.Atten.Constant +
        l.Atten.Linear * Distance +
        l.Atten.Exp * Distance * Distance;

    return Color / Attenuation;
} 
       
       */
       
       


       
                                                                                            
void main()                                                                                 
{ 

    vec3 normal = normalize(outNormal);

    vec4 TotalLight = CalcDirectionalLight(outWorldPos, normal);

    FragColor = TotalLight;
        
/*                                                                                  
    vec3 Normal = normalize(Normal0);                                                       
    vec4 TotalLight = CalcDirectionalLight(Normal);                                         
                                                                                            
    for (int i = 0 ; i < gNumPointLights ; i++) 
    {                                           
        TotalLight += CalcPointLight(gPointLights[i], Normal);                              
    }                                                                                    
                                                                                                 
    for (int i = 0 ; i < gNumSpotLights ; i++) {                                            
        TotalLight += CalcSpotLight(gSpotLights[i], Normal);                                
    }                                                                                       
                                                                                                       
    FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight * gColor[InstanceID % 4];                           
*/
}


/*                                                               
struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};                                                                                  
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                                 
    vec3 Direction;                                                                 
};                                                                                  
                                                                                    
struct Attenuation                                                                  
{                                                                                   
    float Constant;                                                                 
    float Linear;                                                                   
    float Exp;                                                                      
};                                                                                  
                                                                                    
struct PointLight                                                                           
{                                                                                           
    BaseLight Base;                                                                         
    vec3 Position;                                                                          
    Attenuation Atten;                                                                      
};                    

struct SpotLight                                                                            
{                                                                                           
    PointLight Base;                                                                        
    vec3 Direction;                                                                         
    float Cutoff_Consine;        // here it's the cosnine of the angle                                                                   
};     
*/

/*
void main()
{
    vec3 Normal = normalize(Normal0);                                                       
    vec4 TotalLight = CalcDirectionalLight(Normal);   


	FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight;
}
*/

 /*    
vec4 CalcSpotLight(SpotLight l, vec3 Normal)
{
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);
    float SpotFactor = dot(LightToPixel, l.Direction);

    if (SpotFactor > l.Cutoff_Consine) {
        vec4 Color = CalcPointLight(l.Base, Normal);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff_Consine));
    }
    else {
        return vec4(0,0,0,0);
    }
}
             
void main()
{   
    FragColor = vec4(1.0,0.0,0.0,1.0);
}
*/