#version 330

uniform samplerCube 	gCubeMap;
uniform vec3 			gEyeWorldPos;
uniform vec3 			stencilFlag;


in vec3 w_vertex;
in vec3 w_normal;
in vec3 outVertex;
in vec3 outTangent;
in vec3 outColor;
in vec2 outUV;


uniform mat4 m_ModelviewProjection;

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;    
layout (location = 4) out vec3 StencilOut;	
layout (location = 5) out vec3 DepthOut;


float CalcDepthValue(vec3 worldPos)
{
    vec4 Clip_DepthPosition = m_ModelviewProjection * vec4(worldPos,1.0);
    float depthValue = Clip_DepthPosition.z / Clip_DepthPosition.w; // Depth in NDC coordinates
    return (depthValue + 1.0) * 0.5;    // Depth value in range [0,1], same range as the depth buffer
}



void main()
{
	float depthValue = CalcDepthValue(outVertex);

	WorldPosOut     = w_vertex;	

//	vec3 incident 	= normalize(worldVertexPosition - cameraPosition);
//	vec3 normal 	= normalize(worldNormalDirection);

	vec3 incident 	= normalize(w_vertex - gEyeWorldPos);
	vec3 normal 	= normalize(w_normal);
	vec3 reflected 	= reflect(incident,normal);
	DiffuseOut 		= vec3(texture(gCubeMap, reflected));
//	DiffuseOut 		= vec3(1.0,0.0,0.0);
	StencilOut		= stencilFlag;
	DepthOut		= vec3(depthValue,depthValue,depthValue);
}




