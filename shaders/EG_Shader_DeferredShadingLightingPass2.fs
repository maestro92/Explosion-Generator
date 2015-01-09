#version 330
 

uniform vec3 lightPosition_ObjectSpace;         //Light's actual position

in vec3 position;
in vec3 outNormal;
in vec3 outTangent;
in vec3 outColor;

in vec3 lightPosition_CameraEyeSpace;
in mat3 l_normalMatrix;
 
out vec4 FragColor;
 
const vec3 ambientColor=vec3(0.01,0.01,0.01);
const vec3 diffuseColor=vec3(0.9,0.9,0.9);
const vec3 specularColor=vec3(1.0,1.0,1.0);

vec4 CalcLightPerFragment(vec3 LightDirection, vec3 normal)
{
    // ambient light
    vec4 AmbientColor = vec4(ambientColor * outColor,1.0);
 
    // diffuse light
    float diffuseContribution = 0;
    if( dot(normal, LightDirection) > 0)
        diffuseContribution = dot(normal, LightDirection);
    vec4 DiffuseColor = vec4(diffuseColor * outColor * diffuseContribution,1.0);
 
    // specular light
    vec3 surf2camera=normalize(-position);
    vec3 reflection=-reflect(surf2camera,normal);
    float specularContribution = 0;
    if( dot(reflection,LightDirection) > 0)
        specularContribution=pow( dot(reflection,LightDirection), 32.0);
    vec4 SpecularColor = vec4(specularColor * 4.0 * specularContribution,1.0);
 
    // return sum
    return (AmbientColor + DiffuseColor + SpecularColor);
}
 
void main()
{
    // assume everything not in shadow at first
 
    vec3 normal=normalize(outNormal);
    vec3 light2Surf = normalize(position - lightPosition_CameraEyeSpace);
 
    // light direction
    vec3 L_direction = normalize(-lightPosition_ObjectSpace);
 
    // need to multiply the normal matrix to accommodate for the modelview matrix
    L_direction = l_normalMatrix * L_direction;
 
    FragColor = CalcLightPerFragment(-light2Surf, normal);
}


/*
#version 330
     
uniform vec3 cameraPosition;     
                                                                        
in vec3 outVertex;                                                              
in vec3 outNormal;                                                                    
in vec3 outTangent;
in vec3 outColor;                                               
in vec2 outUV;    

in vec4 vertexPosition_LightEyeSpace;
in vec3 lightPosition_CameraEyeSpace;
in mat3 l_normalMatrix;

out vec4 FragColor;

const vec3 ambientColor=vec3(0.01,0.01,0.01);
const vec3 diffuseColor=vec3(0.9,0.9,0.9);
const vec3 specularColor=vec3(1.0,1.0,1.0);
	         
			
vec4 CalcLightPerFragment(vec3 LightDirection, vec3 normal)
{
    // ambient light
    vec4 AmbientColor = vec4(ambientColor * outColor,1.0);
 
    // diffuse light
    float diffuseContribution = 0;
    if( dot(normal, LightDirection) > 0)
        diffuseContribution = dot(normal, LightDirection);
    vec4 DiffuseColor = vec4(diffuseColor * outColor * diffuseContribution,1.0);
 
    // specular light
    vec3 surf2camera=normalize(-position);
    vec3 reflection=-reflect(surf2camera,normal);
    float specularContribution = 0;
    if( dot(reflection,LightDirection) > 0)
        specularContribution=pow( dot(reflection,LightDirection), 32.0);
    vec4 SpecularColor = vec4(specularColor * 4.0 * specularContribution,1.0);
 
    // return sum
    return (AmbientColor + DiffuseColor + SpecularColor);
}

			
vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / gScreenSize;
}



out vec4 FragColor;			
											
void main()									
{											
	vec3 fragNormal = normalize(outNormal);
	vec3 light2Surf = normalize(outVertex - Ligh)




}
*/