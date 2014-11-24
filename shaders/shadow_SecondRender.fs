#version 330
 
uniform sampler2DShadow shadowMap;
uniform vec3 cameraPosition;
uniform vec3 LightPosition;         //Light's actual position
in vec4 lightVertexPosition;        // vertex seen in light's clip space
 
in vec3 outNormal;
in vec3 outTangent;
in vec3 outColor;
in vec3 LightPosition_EyeSpace;
in vec3 position;
in mat3 outNormal_Matrix;
 
out vec4 FragColor;
 
const vec3 ambientColor=vec3(0.01,0.01,0.01);
const vec3 diffuseColor=vec3(0.9,0.9,0.9);
const vec3 specularColor=vec3(1.0,1.0,1.0);

float Cutoff = 0.9857;  // spotlight region cutoff value     
// float Cutoff = 0.9917;  // spotlight region cutoff value    
// float Cutoff = 0.9517;  // spotlight region cutoff value     

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
    float shadowValue = 1.0;
 
    vec3 normal=normalize(outNormal);
    vec3 light2surf = normalize(position - LightPosition_EyeSpace);
 
    // light direction
    vec3 L_direction = normalize(-LightPosition);
 
    // need to multiply the normal matrix to accommodate for the modelview matrix
    L_direction = outNormal_Matrix * L_direction;
 
    float SpotFactor = dot(light2surf, L_direction);
    vec4 Color = vec4(0.0,0.0,0.0,0.0);
 
    // determine lit region
    if(SpotFactor > Cutoff)
    {
        // determine whether pixel is in shadow
        shadowValue = shadow2DProj(shadowMap, lightVertexPosition).r;
 
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