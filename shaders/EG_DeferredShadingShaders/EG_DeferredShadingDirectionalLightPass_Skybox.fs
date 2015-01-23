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

/*
struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation atten;
};


struct SpotLight
{
    BaseLight Base;
    vec3 position;
    vec3 direction;
    float cutoff;
    Attenuation atten;
};
*/



uniform DirectionalLight gDirectionalLight;


uniform sampler2D gPositionMap;  
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;
uniform sampler2D gStencilMap;
uniform sampler2DShadow gShadowMap;

uniform vec2 gScreenSize;
 

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
uniform mat4 l_modelViewProjectionMatrix;


in vec3 outVertex;
in vec3 outNormal;
in vec3 outTangent;
in vec3 outColor;
in vec2 outUV;

out vec4 FragColor;
 
const vec3 ambientColor=vec3(0.01,0.01,0.01);
const vec3 diffuseColor=vec3(0.9,0.9,0.9);
const vec3 specularColor=vec3(1.0,1.0,1.0);

float Cutoff = 0.9857;  // spotlight region cutoff value     



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




vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / gScreenSize;
}





void main()
{
    // assume everything not in shadow at first
  //  return;

    vec2 texCoord = CalcTexCoord();
    vec3 worldPos = texture(gPositionMap, texCoord).xyz;
    vec3 colorValue = texture(gColorMap, texCoord).xyz;
  
    // remember to check the normal map from the Geometry Pass
    vec3 normal = texture(gNormalMap, texCoord).xyz;

  //  normal = normalize(normal);
    vec3 stencilValue = texture(gStencilMap, texCoord).xyz;


 //   FragColor = (stencilValue.x == 1.0) ?  vec4(colorValue,1.0) : (vec4(colorValue,1.0) * CalcDirectionalLight(worldPos, normal));


    vec4 vertexPosition_LightEyeSpace = l_modelViewProjectionMatrix * vec4(worldPos,1.0);
    float shadowValue = 1.0;
    shadowValue = shadow2DProj(gShadowMap, vertexPosition_LightEyeSpace).r;


    if(stencilValue.x == 1.0)
        FragColor = vec4(colorValue,1.0);
    else
        FragColor = (shadowValue == 0.0) ?  vec4(0.0,0.0,0.0,1.0) : vec4(colorValue,1.0) * CalcDirectionalLight(worldPos, normal);

}

