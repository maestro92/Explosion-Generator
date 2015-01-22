#version 330

uniform samplerCube gCubeMap;
uniform samplerCube cubeMap;

in vec3 vertexVector;
in vec3 outNormal;
in vec3 outTangent;
in vec3 outColor;
in vec2 outUV;
in vec3 WorldPos0; 


uniform vec3 stencilFlag;


layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;    
layout (location = 4) out vec3 StencilOut;	
layout (location = 5) out vec3 DepthOut;	



void main()
{

	WorldPosOut     = WorldPos0;	
//	DiffuseOut      = textureCube(cubeMap, vertexVector).xyz;
	DiffuseOut      = textureCube(gCubeMap, vertexVector).xyz;
//	DiffuseOut		= vec3(1.0,0.0,0.0);
//	NormalOut       = normalize(outNormal);					
//	TexCoordOut     = vec3(outUV, 0.0);	
	StencilOut		= stencilFlag;
//	StencilOut		= vec3(1.0,0.0,0.0);
	DepthOut		= vec3(1.0f,1.0f,1.0f);
}




