uniform sampler2DShadow shadowMap; // also the depth texture
uniform vec3 cameraPosition;


in vec4 lightVertexPosition;		// vertex seen in light's clip space



in vec3 outNormal;
in vec3 outColor;
in vec3 LightPosition_EyeSpace;
in vec3 position;

out vec4 FragColor;

const vec3 ambientColor=vec3(0.05,0.05,0.05);
const vec3 diffuseColor=vec3(0.7,0.7,0.7);
const vec3 specularColor=vec3(1.0,1.0,1.0);






void main()
{
	float shadowValue = shadow2DProj(shadowMap, lightVertexPosition).r; // r==1.0 not in shader

/*	
	for(float x= -0.01; x<=0.01; x+=0.005)
	{
		for(float y= -0.01; y<-0.01; y+=0.005)
		{
			

		}

	}
*/																		// r==0.0 not in shader




																								
	vec3 normal=normalize(outNormal);
	vec3 surf2light=normalize(LightPosition_EyeSpace - position);
	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
	float diffuseContribution=max(0.0,dot(normal,surf2light));
	float specularContribution=pow(max(0.0,dot(reflection,surf2light)),16.0);
					
					
					// ambient color will always be present	
					//
					// no specular light if shadowValue in shadow									
	FragColor=vec4(ambientColor * outColor + (shadowValue+0.05) * diffuseContribution * diffuseColor*outColor +
											 (shadowValue<0.5 ? vec3(0.0,0.0,0.0) : specularContribution*specularColor*shadowValue),1.0);
//	FragColor = vec4(ambientColor*outColor + shadowBool, shadowBool, shadowBool, 1.0);

	
//	FragColor = vec4(shadowBool, shadowBool, shadowBool, 1.0);

}
