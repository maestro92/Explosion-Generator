#version 330


struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

uniform DirectionalLight gDirectionalLight;


//uniform sampler2D positionMap;  
//uniform sampler2D colorMap;
//uniform sampler2D normalMap;


uniform sampler2D gPositionMap;  
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;
uniform sampler2DShadow shadowMap;


uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;

uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat4 m_ViewMatrix;
uniform mat4 m_ProjectionMatrix;
uniform mat4 m_ModelMatrix;
uniform mat4 m_ViewNoRotateMatrix;
uniform mat3 m_normalMatrix;   




uniform vec3 lightPosition_ObjectSpace;         //Light's actual outVertex
in vec4 vertexPosition_LightEyeSpace;        // vertex seen in light's clip space
in vec3 lightPosition_CameraEyeSpace;
in mat3 l_normalMatrix;

in vec3 outVertex;
in vec3 outNormal;
in vec3 outTangent;
in vec3 outColor;
in vec2 outUV;



uniform vec2 gScreenSize;
 
out vec4 FragColor;
 
const vec3 ambientColor=vec3(0.01,0.01,0.01);
const vec3 diffuseColor=vec3(0.9,0.9,0.9);
const vec3 specularColor=vec3(1.0,1.0,1.0);

float Cutoff = 0.9857;  // spotlight region cutoff value     


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
//    vec3 surf2camera=normalize(-outVertex);
//    vec3 reflection=-reflect(surf2camera,normal);

    vec3 camera2surf=normalize(outVertex);
    vec3 reflection=reflect(camera2surf,normal);
    float specularContribution = 0;
    if( dot(reflection,LightDirection) > 0)
        specularContribution=pow( dot(reflection,LightDirection), 32.0);
    vec4 SpecularColor = vec4(specularColor * 4.0 * specularContribution,1.0);
 
    // return sum
    return (AmbientColor + DiffuseColor + SpecularColor);
}




/*
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
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, gSpecularPower);
        
        if (SpecularFactor > 0.0) 
            SpecularColor = vec4(Light.color, 1.0) * gMatSpecularIntensity * SpecularFactor;
        
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}
*/

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
//        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos);
        vec3 EyeToVertex = normalize(WorldPos);
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

    vec3 lightDirection = mat3(m_ViewMatrix) * gDirectionalLight.direction;
    lightDirection = normalize(lightDirection);

    return CalcLightInternal(gDirectionalLight.base,
                             lightDirection,
                             WorldPos,
                             Normal);
}


vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / gScreenSize;
}


#if 1

/*
void main()
{
    // assume everything not in shadow at first
    vec2 texCoord = CalcTexCoord();
    vec3 worldPos = texture(gPositionMap, texCoord).xyz;
    vec3 colorValue = texture(gColorMap, texCoord).xyz;
  
    // remember to check the normal map from the Geometry Pass
    vec3 normal = texture(gNormalMap, texCoord).xyz;
    normal = normalize(normal);
  
    vec3 light2surf = normalize(outVertex - lightPosition_CameraEyeSpace);

    FragColor = vec4(colorValue, 0.0) * CalcLightPerFragment(-light2surf, normal);

//    FragColor = vec4(colorValue, 1.0) ;//* CalcDirectionalLight(worldPos, normal);
}
*/


void main()
{
    // assume everything not in shadow at first
  //  return;
    vec2 texCoord = CalcTexCoord();
    vec3 worldPos = texture(gPositionMap, texCoord).xyz;
    vec3 colorValue = texture(gColorMap, texCoord).xyz;
  
    // remember to check the normal map from the Geometry Pass
    vec3 normal = texture(gNormalMap, texCoord).xyz;
    normal = normalize(normal);
  
    // vec3 light2surf = normalize(outVertex - lightPosition_CameraEyeSpace);
  //  light2surf = normalize(outVertex - lightPosition_CameraEyeSpace);

 //   vec3 light2surf = mat3(m_ViewMatrix) * gDirectionalLight.direction;
 //   light2surf = normalize(light2surf);
 //   vec4 Color = CalcLightPerFragment(-light2surf, normal);

 //   FragColor = vec4(colorValue,1.0) * CalcLightPerFragment(light2surf, normal);
        // the view matrix doesn't work here
    FragColor = vec4(colorValue,1.0) * CalcDirectionalLight(worldPos, normal);

 //   FragColor = vec4(colorValue,1.0);
 //   FragColor = vec4(normal,1.0);

 //   FragColor = CalcLightPerFragment(-light2surf, normal);
}



/*
void main()
{
    // assume everything not in shadow at first
    float shadowValue = 1.0;

    vec2 texCoord = CalcTexCoord();
 //   vec3 worldPos = texture(gPositionMap, texCoord).xyz;
    vec3 colorValue = texture(gColorMap, texCoord).xyz;
  
    // remember to check the normal map from the Geometry Pass
    vec3 normal = texture(gNormalMap, texCoord).xyz;
    normal = normalize(normal);
  
    vec3 light2surf = normalize(outVertex - lightPosition_CameraEyeSpace);
 
    // light direction
    vec3 l_direction = normalize(-lightPosition_ObjectSpace);
 
    // need to multiply the normal matrix to accommodate for the modelview matrix
    l_direction = l_normalMatrix * l_direction;




    float SpotFactor = dot(light2surf, l_direction);
    vec4 Color = vec4(0.0,0.0,0.0,0.0);

    // determine lit region
    if(SpotFactor > Cutoff)
    {
        // determine whether pixel is in shadow
        shadowValue = shadow2DProj(shadowMap, vertexPosition_LightEyeSpace).r;
 
        // calculate spotlight color
        Color = CalcLightPerFragment(-light2surf, normal);
        Color = Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - Cutoff));
    }
 
    // if pixel in shadows, we give black
    if (shadowValue == 0)
        FragColor = vec4(0.0,0.0,0.0,0.0);
 
    else    // we give the supposed spotlight color
        FragColor = Color;
}
*/



/*

void main()
{
    // assume everything not in shadow at first
    float shadowValue = 1.0;

    vec2 texCoord = CalcTexCoord();
 //   vec3 worldPos = texture(gPositionMap, texCoord).xyz;
    vec3 colorValue = texture(gColorMap, texCoord).xyz;
  
    // remember to check the normal map from the Geometry Pass
    vec3 normal = texture(gNormalMap, texCoord).xyz;
    normal = normalize(normal);
  
//    vec3 light2surf = normalize(outVertex - lightPosition_CameraEyeSpace);
    vec3 light2surf = normalize(outVertex - lightPosition_CameraEyeSpace);

    // light direction
    vec3 l_direction = normalize(-lightPosition_ObjectSpace);
 
    // need to multiply the normal matrix to accommodate for the modelview matrix
    l_direction = l_normalMatrix * l_direction;

#if 0
//////////
// if we want pure directional light, these two are equivalent
//////////
  
    light2surf = l_direction;

    or

    light2surf = -lightPosition_ObjectSpace;
    light2surf = mat3(m_ViewMatrix) * light2surf;
    light2surf = normalize(light2surf);
#endif

    float SpotFactor = dot(light2surf, l_direction);
    vec4 Color = vec4(0.0,0.0,0.0,0.0);

    // determine lit region
    if(SpotFactor > Cutoff)
    {
        // determine whether pixel is in shadow
        shadowValue = shadow2DProj(shadowMap, vertexPosition_LightEyeSpace).r;
 
        // calculate spotlight color
        Color = CalcLightPerFragment(-light2surf, normal);
        Color = Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - Cutoff));
    }
 
    // if pixel in shadows, we give black
    if (shadowValue == 0)
        FragColor = vec4(0.0,0.0,0.0,0.0);
 
    else    // we give the supposed spotlight color
        FragColor = Color;
}
*/



#else



vec4 CalcDirectionalLight(vec3 WorldPos, vec3 Normal)
{
    return CalcLightInternal(gDirectionalLight.Base,
							 gDirectionalLight.Direction,
							 WorldPos,
							 Normal);
}




void main()
{
    vec2 texCoord = CalcTexCoord();
    vec3 worldPos = texture(positionMap, texCoord).xyz;
    vec3 colorValue = texture(colorMap, texCoord).xyz;
    vec3 normal = texture(normalMap, texCoord).xyz;
    normal = normalize(normal);

	FragColor = vec4(Color, 1.0) * CalcDirectionalLight(worldPos, normal);
}
#endif