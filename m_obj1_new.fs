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


in vec3 PointLightPos_EyeSpace1;

#define EPSILON 0.00001


// all the types of light struct written here


struct BaseLight
{
    vec3 Color;
    mat4 MV_Matrix;
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
    float Cutoff;        // here it's the cosnine of the angle                                                                   
};     





uniform int gNumPointLights;
uniform int gNumSpotLights;

uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];  

uniform float gMatSpecularIntensity;                                                
uniform float gMatSpecularPower;   



uniform vec3 PointlightPos;
uniform mat4 PointLightPos_ModelViewMatrix;	//ModelViewMatrix for the light position
vec3 PointLightPos_EyeSpace;

	



const vec3 ambientColor=vec3(0.05,0.05,0.05);
const vec3 diffuseColor=vec3(0.7,0.7,0.7);
const vec3 specularColor=vec3(1.0,1.0,1.0);


vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 normal)     
{
	
	vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;  
    
    vec3 myLightDirection = -outNormalMatrix * LightDirection;
    myLightDirection = normalize(myLightDirection);
	


	float diffuseContribution=max(0.0,dot(normal, myLightDirection));
	vec4 DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * diffuseContribution;
	

	vec3 surf2light=normalize(lightPos-position);
	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
	float specularContribution = pow(max(0.0,dot(reflection,surf2light)), gMatSpecularPower);
	vec4 SpecularColor = vec4(Light.Color, 1.0f) * gMatSpecularIntensity * specularContribution;


	return (AmbientColor + DiffuseColor + SpecularColor);
}




vec4 CalcLightInternal1(BaseLight Light, vec3 LightDirection, vec3 normal)     
{
	
	vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;  
    
    vec3 myLightDirection = normalize(LightDirection);


	float diffuseContribution=max(0.0,dot(normal, myLightDirection));
	vec4 DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * diffuseContribution;
	

	vec3 surf2light=normalize(PointLightPos_EyeSpace-position);
	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
	float specularContribution = pow(max(0.0,dot(reflection,surf2light)), gMatSpecularPower);
	vec4 SpecularColor = vec4(Light.Color, 1.0f) * gMatSpecularIntensity * specularContribution;


	return (AmbientColor + DiffuseColor + SpecularColor);
}


vec4 CalcDirectionalLight(vec3 normal)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, normal);
}


vec4 CalcPointLight(int Index, vec3 normal)                                                 
{   
      /*                 
    vec3 surf2light=normalize(PointLightPos_EyeSpace - position);
    float Distance = length(surf2light);
	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
	float diffuseContribution=max(0.0,dot(normal,surf2light));
	float specularContribution=pow(max(0.0,dot(reflection,surf2light)),16.0);
       
    vec4 Color = vec4((ambientColor +  diffuseContribution * diffuseColor + specularContribution*specularColor),1.0);                
  	float Attenuation =  gPointLights[Index].Atten.Constant +                               
             gPointLights[Index].Atten.Linear * Distance +                      
             gPointLights[Index].Atten.Exp * Distance * Distance;                      

    return Color / Attenuation;                                                             
     */    
                                                                              
    vec3 LightDirection = PointLightPos_EyeSpace - position;
    
                          
    float Distance = length(LightDirection);                                                
    LightDirection = normalize(LightDirection);                                             
                                                                                            
    vec4 Color = CalcLightInternal1(gPointLights[Index].Base, LightDirection, normal);       
    
    float Attenuation =  gPointLights[Index].Atten.Constant +                               
                         gPointLights[Index].Atten.Linear * Distance +                      
                         gPointLights[Index].Atten.Exp * Distance * Distance;               
                                                                                            
    return Color / Attenuation;                                                             

}      




void main()
{
	vec3 normal=normalize(outNormal);
	PointLightPos_EyeSpace = vec3(PointLightPos_ModelViewMatrix * vec4(PointlightPos,1.0)); 

	vec4 TotalLight = CalcDirectionalLight(normal);

    for (int i = 0 ; i < gNumPointLights ; i++) 
    { 
        TotalLight += CalcPointLight(i, normal);                                            
    }                                                                                       
        	

	gl_FragColor = texture2D(texture0, outUV) * TotalLight;

}


