#version 330

uniform sampler2DShadow shadowMap; // also the depth texture
//uniform sampler2D shadowMap;

uniform vec3 cameraPosition;


in vec4 lightVertexPosition;		// vertex seen in light's clip space



in vec3 outNormal;
in vec3 outColor;
in vec3 LightPosition_EyeSpace;
in vec3 position;

in mat3 outNormal_Matrix;

out vec4 FragColor;

const vec3 ambientColor=vec3(0.01,0.01,0.01);
const vec3 diffuseColor=vec3(0.9,0.9,0.9);
const vec3 specularColor=vec3(1.0,1.0,1.0);

//float Cutoff = 0.9427;	// 50 degress

float Cutoff = 0.9917;	// 50 degress



in vec3 LightActualPosition;




vec4 CalcLightInternal_Point(vec3 LightDirection, vec3 normal)     
{
	
	vec4 AmbientColor = vec4(ambientColor * outColor,1.0); 
    

    // diffuse 
    float diffuseContribution = 0;
    if( dot(normal, LightDirection) > 0)
    {
        diffuseContribution = dot(normal, LightDirection);
    }
    vec4 DiffuseColor = vec4(diffuseColor * outColor * diffuseContribution,1.0);
    

	

	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
    float specularContribution = 0;
    if( dot(reflection,LightDirection) > 0)
    {
        specularContribution=pow( dot(reflection,LightDirection), 32.0);
    }
    
	vec4 SpecularColor = vec4(specularColor * 4.0 * specularContribution,1.0);


	return (AmbientColor + DiffuseColor + SpecularColor);
}




void main()
{
	// float shadowValue = shadow2DProj(shadowMap, lightVertexPosition).r; // r==1.0 not in shadow
/*
	float shadowValue;
	vec4 lightVertexPosition2 = lightVertexPosition;
	lightVertexPosition2 /= lightVertexPosition2.w;
	if(texture2D(shadowMap, lightVertexPosition2.xy).r >= lightVertexPosition2.z)
		shadowValue = 1.0;
	else
		shadowValue = 0.0;
*/			
			
	float shadowValue = 1.0;
			
			
																								
	vec3 normal=normalize(outNormal);
	vec3 surf2light=normalize(LightPosition_EyeSpace - position);
	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);


	// float diffuseContribution=max(0.0,dot(normal,surf2light));
    float diffuseContribution = 0;
    if( dot(normal, surf2light) > 0)
    {
        diffuseContribution = dot(normal, surf2light);
    }



	// float specularContribution=pow(max(0.0,dot(reflection,surf2light)),16.0);
	float specularContribution = 0;
    if( dot(reflection,surf2light) > 0)
    {
        specularContribution=pow( dot(reflection,surf2light), 16.0);
    }

/////////////

	vec3 light2surf = -surf2light;
	
    // light direction
    vec3 L_direction = normalize(-LightActualPosition);
  //  vec3 L_direction = vec3(1.0,-1.0,0.0);
    L_direction = outNormal_Matrix * L_direction;
    
    // dot product
    float SpotFactor = dot(light2surf, L_direction);
			
			
	vec4 Color = vec4(0.0,0.0,0.0,0.0);			
	if(SpotFactor > Cutoff)				
	{

		shadowValue = shadow2DProj(shadowMap, lightVertexPosition).r;


		Color = CalcLightInternal_Point(-light2surf, normal);
		Color = Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - Cutoff));
	}				
	
			
///////////////	
	// if you're in shadow within the illuminated area	
	if (shadowValue < 0.5)
		FragColor = vec4(0.0,0.0,0.0,0.0);
	//	FragColor = vec4(ambientColor * outColor + 
	//				(shadowValue+0.05) * diffuseContribution * diffuseColor*outColor +
	//				(shadowValue<0.5 ? vec3(0.0,0.0,0.0) : specularContribution*specularColor*shadowValue),1.0);
	else
		FragColor = Color;

//	FragColor=vec4(ambientColor * outColor + 
//					(shadowValue+0.05) * diffuseContribution * diffuseColor*outColor +
//					(shadowValue<0.5 ? vec3(0.0,0.0,0.0) : specularContribution*specularColor*shadowValue),1.0);

					
					// ambient color will always be present	
					//
					// no specular light if shadowValue in shadow									
//	FragColor=vec4(ambientColor * outColor + 
//					(shadowValue+0.05) * diffuseContribution * diffuseColor*outColor +
//					(shadowValue<0.5 ? vec3(0.0,0.0,0.0) : specularContribution*specularColor*shadowValue),1.0);


//	FragColor = vec4(ambientColor*outColor + shadowBool, shadowBool, shadowBool, 1.0);

	
//	FragColor = vec4(shadowBool, shadowBool, shadowBool, 1.0);

}


