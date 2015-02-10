#version 330

struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

/*
struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};
*/

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

/*
struct SpotLight
{
    BaseLight Base;
    vec3 position;
    vec3 direction;
    float cutoff;
    Attenuation atten;
};
*/


in vec3 outVertex;
in vec3 outNormal;
in vec3 outTangent;
in vec3 outColor;
in vec2 outUV;


uniform PointLight gPointLight;

uniform sampler2D gPositionMap;
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;
uniform sampler2D gStencilMap;

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
uniform vec2 gScreenSize;
 
out vec4 FragColor;
 


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


vec4 CalcPointLight(vec3 WorldPos, vec3 Normal)
{
    vec3 LightDirection = WorldPos - gPointLight.position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(gPointLight.base, LightDirection, WorldPos, Normal);

    float Attenuation =  gPointLight.atten.constant +
                         gPointLight.atten.linear * Distance +
                         gPointLight.atten.exp * Distance * Distance;

    Attenuation = max(1.0, Attenuation);

    return Color / Attenuation;
//    return Color;
}




vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / gScreenSize;
}


void main()									
{											
/*  
    vec2 TexCoord = CalcTexCoord();
    vec3 WorldPos = texture(gPositionMap, TexCoord).xyz;
    vec3 Color = texture(gColorMap, TexCoord).xyz;
    vec3 Normal = texture(gNormalMap, TexCoord).xyz;
    Normal = normalize(Normal);

    FragColor = vec4(Color, 1.0) * CalcPointLight(WorldPos, Normal);  
*/
    
    vec2 texCoord = CalcTexCoord();
    vec3 worldPos = texture(gPositionMap, texCoord).xyz;
    vec3 colorValue = texture(gColorMap, texCoord).xyz;
    vec3 normal = texture(gNormalMap, texCoord).xyz;
    FragColor = vec4(colorValue,1.0) * CalcPointLight(worldPos, normal);        
    
 //   FragColor = vec4(colorValue,1.0);
 //   FragColor = vec4(gScreenSize.x/1024.0,0.0,0.0,1.0);

}
