varying vec3 outNormal;
varying vec3 outTangent;
varying vec3 outColor;
varying vec3 outUV;
varying vec3 outLightPos;
varying vec3 position;
uniform sampler2D texture0;
uniform vec3 cameraPosition;
uniform vec3 lightPos;

const vec3 ambientColor=vec3(0.05,0.05,0.05);
const vec3 diffuseColor=vec3(0.7,0.7,0.7);
const vec3 specularColor=vec3(1.0,1.0,1.0);

void main()
{
		vec3 normal=normalize(outNormal);
		vec3 surf2light=normalize(lightPos-position);
		vec3 surf2camera=normalize(-position);
		vec3 reflection=-reflect(surf2camera,normal);
		float diffuseContribution=max(0.0,dot(normal,surf2light));
		float specularContribution=pow(max(0.0,dot(reflection,surf2light)),4.0);
		gl_FragColor=vec4(ambientColor*outColor+diffuseContribution *diffuseColor*outColor+ specularContribution*specularColor,1.0);
}
