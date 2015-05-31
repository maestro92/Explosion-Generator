#version 330

layout (location = 0) in vec3 	vertex;
layout (location = 1) in vec3 	normal;
layout (location = 2) in vec3 	tangent;
layout (location = 3) in vec3 	color;
layout (location = 4) in vec2 	UV;
layout (location = 5) in ivec4 	boneIDs;
layout (location = 6) in vec4 	weights;


uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat4 m_ViewMatrix;
uniform mat4 m_ProjectionMatrix;
uniform mat4 m_ModelMatrix;
uniform mat4 m_ViewNoRotateMatrix;
uniform mat3 m_normalMatrix;   

const int MAX_BONES = 100;

uniform mat4 gBones[MAX_BONES];
 
out vec3 outVertex;

void main()
{
    mat4 boneTransform = gBones[boneIDs[0]] * weights[0];
    boneTransform     += gBones[boneIDs[1]] * weights[1];
    boneTransform     += gBones[boneIDs[2]] * weights[2];
    boneTransform     += gBones[boneIDs[3]] * weights[3];
  
	vec4 newPos = boneTransform * vec4(vertex, 1.0);
	gl_Position = m_ModelviewProjection * newPos;
}