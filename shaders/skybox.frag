#version 330

uniform samplerCube cubeMap;

in vec3 outColor;
in vec3 vertexVector;
out vec4 FragColor;
void main()
{
	
	FragColor = textureCube(cubeMap, vertexVector);
}




