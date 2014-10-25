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

in mat3 outNormalMatrix;
uniform sampler2D texture0;
uniform vec3 cameraPosition;
uniform vec3 lightPos;

#define EPSILON 0.00001


// all the types of light struct written here


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
                                                                                    
                                                                             
struct PointLight                                                                           
{                                                                                           
    BaseLight Base;                                                                         
    vec3 Position;                                                                          
    float Constant;                                                                 
    float Linear;                                                                   
    float Exp;                                                                    
};                    

struct SpotLight                                                                            
{                                                                                           
    PointLight Base;                                                                        
    vec3 Direction;                                                                         
    float Cutoff;        // here it's the cosnine of the angle                                                                   
};     


const vec3 ambientColor=vec3(0.5,0.5,0.5);
const vec3 diffuseColor=vec3(0.7,0.7,0.7);
const vec3 specularColor=vec3(1.0,1.0,1.0);


uniform int gNumPointLights;
uniform int gNumSpotLights;

uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];  

uniform float gMatSpecularIntensity;                                                
uniform float gMatSpecularPower;   

void main()
{
		vec3 normal=normalize(outNormal);
		vec3 surf2light=normalize(lightPos-position);
		vec3 surf2camera=normalize(-position);
		vec3 reflection=-reflect(surf2camera,normal);

		float diffuseContribution = 0;
		if( dot(normal,surf2light) > 0)
		{
			diffuseContribution = dot(normal,surf2light);
		}


		float specularContribution = 0;
		if( dot(reflection,surf2light) > 0)
		{
			specularContribution=pow( dot(reflection,surf2light), 1.0);
		}
	
	
		gl_FragColor=vec4( ambientColor*outColor + 
							diffuseContribution * diffuseColor * outColor + 
							specularContribution*specularColor,1.0);
	
}
