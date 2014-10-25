#version 330
const int MAX_POINT_LIGHTS = 2;   
const int MAX_SPOT_LIGHTS = 2;   



in vec3 outNormal;
in vec3 outNormal2;
in vec3 outTangent;
in vec3 outColor;
in vec2 outUV;
in vec3 outLightPos;
in vec3 position;

in mat4 outMV_Matrix;
in mat3 outNormalMatrix;
uniform sampler2D texture0;
uniform vec3 cameraPosition;
uniform vec3 lightPos;

#define EPSILON 0.00001


// all the types of light struct written here


struct BaseLight
{
    vec3 Color;
    mat4 MV_Matrix;
    mat4 Normal_Matrix;
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
    BaseLight Base;                                                                        
    vec3 Direction;                                                                         
    vec3 Position;
    Attenuation Atten;    
    float Cutoff;        // here it's the cosnine of the angle                                                                   
};     

vec3 LightPos_EyeSpace;
mat3 Norm_Matrix;


uniform int gNumPointLights;
uniform int gNumSpotLights;

uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];  

uniform float gMatSpecularIntensity;                                                
uniform float gMatSpecularPower;   




	



const vec3 ambientColor=vec3(0.05,0.05,0.05);
const vec3 diffuseColor=vec3(0.7,0.7,0.7);
const vec3 specularColor=vec3(1.0,1.0,1.0);


vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 normal)     
{
	
	vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;  
    
    vec3 myLightDirection = -outNormalMatrix * LightDirection;
    myLightDirection = normalize(myLightDirection);
	


    // diffuse 
    float diffuseContribution = 0;
    if( dot(normal, myLightDirection) > 0)
    {
        diffuseContribution = dot(normal, myLightDirection);
    }
	vec4 DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * diffuseContribution;
	

    // Specular
	vec3 surf2light=normalize(lightPos-position);
	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
    float specularContribution = 0;
    if( dot(reflection,surf2light) > 0)
    {
        specularContribution=pow( dot(reflection,surf2light), gMatSpecularPower);
    }
	vec4 SpecularColor = vec4(Light.Color, 1.0f) * gMatSpecularIntensity * specularContribution;


	return (AmbientColor + DiffuseColor + SpecularColor);
}




vec4 CalcLightInternal_Point(BaseLight Light, vec3 LightDirection, vec3 normal)     
{
	
	vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;  
    


    // diffuse 
    float diffuseContribution = 0;
    if( dot(normal, LightDirection) > 0)
    {
        diffuseContribution = dot(normal, LightDirection);
    }
    vec4 DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * diffuseContribution;
    

	

	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
    float specularContribution = 0;
    if( dot(reflection,LightDirection) > 0)
    {
        specularContribution=pow( dot(reflection,LightDirection), gMatSpecularPower);
    }
    
	vec4 SpecularColor = vec4(Light.Color, 1.0f) * gMatSpecularIntensity * specularContribution;


	return (AmbientColor + DiffuseColor + SpecularColor);
}


vec4 CalcDirectionalLight(vec3 normal)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, normal);
}


vec4 CalcPointLight(int Index, vec3 normal, vec3 eyespace_pos)                                                 
{   

    vec3 LightDirection = eyespace_pos - position;
                          
    float Distance = length(LightDirection);                                                
    LightDirection = normalize(LightDirection);                                             
                                                                                            
    vec4 Color = CalcLightInternal_Point(gPointLights[Index].Base, LightDirection, normal);       
    
    float Attenuation =  gPointLights[Index].Atten.Constant +                               
                         gPointLights[Index].Atten.Linear * Distance +                      
                         gPointLights[Index].Atten.Exp * Distance * Distance;               
                                                                                            
    return Color / Attenuation;                                                             
}      



vec4 CalcSpotLight(int Index, vec3 normal, vec3 eyespace_pos, vec3 L_direction)                                                 
{   

    vec3 light2surf = position - eyespace_pos;
                          
    float Distance = length(light2surf);                                                
    light2surf = normalize(light2surf);                                             
                             
    Norm_Matrix = mat3(gSpotLights[Index].Base.MV_Matrix);    
    vec3 L_direction1 = Norm_Matrix * L_direction;
          
    float SpotFactor = dot(light2surf, L_direction1);      
          
    if (SpotFactor > gSpotLights[Index].Cutoff)
    {                                                        
        vec4 Color = CalcLightInternal_Point(gSpotLights[Index].Base, -light2surf, normal);       
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - gSpotLights[Index].Cutoff)); 
    }

    else
        return vec4(0,0,0,0);                                                            
}   
                 

/*
vec4 CalcSpotLight(int Index, vec3 normal, vec3 eyespace_pos, vec3 L_direction)                                                 
{   

    vec3 surf2light = eyespace_pos - position;
                          
    float Distance = length(surf2light);                                                
    surf2light = normalize(surf2light);                                             
                             
    Norm_Matrix = mat3(gSpotLights[Index].Base.MV_Matrix);    
    
                                                                          
    vec4 Color = CalcLightInternal_Point(gSpotLights[Index].Base, surf2light, normal);       
    
   // float SpotFactor = dot(surf2light, )

    float Attenuation =  gSpotLights[Index].Atten.Constant +                               
                         gSpotLights[Index].Atten.Linear * Distance +                      
                         gSpotLights[Index].Atten.Exp * Distance * Distance;               
                                                                                            
    return Color / Attenuation;                                                             
}      
*/



void main()
{
	vec3 normal=normalize(outNormal);

	vec4 TotalLight = CalcDirectionalLight(normal);

    for (int i = 0 ; i < gNumPointLights ; i++) 
    { 
    	LightPos_EyeSpace = vec3(gPointLights[i].Base.MV_Matrix * vec4(gPointLights[i].Position,1.0)); 
        TotalLight += CalcPointLight(i, normal, LightPos_EyeSpace);                                            
    }                                                                                       
        	
    for (int i = 0 ; i < gNumSpotLights ; i++) 
    { 
        LightPos_EyeSpace = vec3(gSpotLights[i].Base.MV_Matrix * vec4(gSpotLights[i].Position,1.0)); 
        TotalLight += CalcSpotLight(i, normal, LightPos_EyeSpace, gSpotLights[i].Direction);                                            
    }  

	gl_FragColor = texture2D(texture0, outUV) * TotalLight;

}


