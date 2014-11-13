#version 330

uniform samplerCube cubeMap;
uniform vec3 cameraPosition;


in vec3 outColor;
in vec3 vertexVector;

in vec3 outNormal;
in vec2 outUV;
in vec3 worldVertexPosition;
in vec3 worldNormalDirection;

out vec4 FragColor;
void main()
{
	vec3 incident = normalize(worldVertexPosition - cameraPosition);
	vec3 normal = normalize(worldNormalDirection);
	vec3 reflected = reflect(incident,normal);
	FragColor = texture(cubeMap, reflected);

}




