
#version 330

uniform samplerCube gCubeMap;

in vec3 outPosition;
in vec3 outNormal;
in vec3 outColor;
in vec2 outUV;

out vec4 FragColor;
void main()
{
	FragColor = textureCube(gCubeMap, outPosition);
}



/*
#version 330

uniform samplerCube cubeMap;

in vec3 vertexVector;
in vec3 outNormal;
in vec3 outColor;
in vec2 outUV;

out vec4 FragColor;
void main()
{
	FragColor = textureCube(cubeMap, vertexVector);
//	FragColor = vec4(1.0,0.0,0.0,1.0);
}
*/







