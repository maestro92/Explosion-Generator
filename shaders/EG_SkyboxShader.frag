#version 330

uniform samplerCube cubeMap;

in vec3 vertexVector;
in vec3 outNormal;
in vec3 outTangent;
in vec3 outColor;
in vec2 outUV;

out vec4 FragColor;
void main()
{
	FragColor = textureCube(cubeMap, vertexVector);
}




