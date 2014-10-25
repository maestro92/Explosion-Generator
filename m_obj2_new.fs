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
    BaseLight Base;                                                                        
    vec3 Direction;                                                                         
    vec3 Position;
    Attenuation Atten;    
    float Cutoff;        // here it's the cosnine of the angle                                                                   
};     

vec3 PointLightPos_EyeSpace;



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
	

    // specular 
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
    

	float diffuseContribution=max(0.0,dot(normal, LightDirection));
	vec4 DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * diffuseContribution;
	

	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
	float specularContribution = pow(max(0.0,dot(reflection,LightDirection)), gMatSpecularPower);
	vec4 SpecularColor = vec4(Light.Color, 1.0f) * gMatSpecularIntensity * specularContribution;


	return (AmbientColor + DiffuseColor + SpecularColor);
}


vec4 CalcDirectionalLight(vec3 normal)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, normal);
}


vec4 CalcPointLight(int Index, vec3 normal, vec3 eyespace_pos)                                                 
{       

    vec3 surf2light=normalize(eyespace_pos - position);
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
                                             
                                             /*                                 
  //  vec3 LightDirection = PointLightPos_EyeSpace - position;
    vec3 LightDirection = eyespace_pos - position;
                          
    float Distance = length(LightDirection);                                                
    LightDirection = normalize(LightDirection);                                             
                                                                                            
    vec4 Color = CalcLightInternal_Point(gPointLights[Index].Base, LightDirection, normal);       
    
    float Attenuation =  gPointLights[Index].Atten.Constant +                               
                         gPointLights[Index].Atten.Linear * Distance +                      
                         gPointLights[Index].Atten.Exp * Distance * Distance;               
                                                                                            
    return Color / Attenuation;                                                             
*/
}      




void main()
{
	vec3 normal=normalize(outNormal);

//	PointLightPos_EyeSpace = vec3(gPointLights[0].Base.MV_Matrix * vec4(gPointLights[0].Position,1.0)); 

	vec4 TotalLight = CalcDirectionalLight(normal);

    for (int i = 0 ; i < gNumPointLights ; i++) 
    { 
    	PointLightPos_EyeSpace = vec3(gPointLights[i].Base.MV_Matrix * vec4(gPointLights[i].Position,1.0)); 
        TotalLight += CalcPointLight(i, normal, PointLightPos_EyeSpace);                                            
    }                                                                                       
        	

	gl_FragColor = texture2D(texture0, outUV) * TotalLight;

}


