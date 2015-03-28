#version 330

in vec3 	vertex;
in vec3 	normal;
in vec3 	tangent;
in vec3 	color;
in vec2 	UV;
in ivec4 	boneIDs;
in vec4 	weights;


uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat4 m_ViewMatrix;
uniform mat4 m_ProjectionMatrix;
uniform mat4 m_ModelMatrix;
uniform mat4 m_ViewNoRotateMatrix;
uniform mat3 m_normalMatrix;   

const int MAX_BONES = 100;

uniform gBones[MAX_BONES];
 
out vec3 outVertex;
out vec3 outNormal;
out vec3 outTangent;
out vec3 outColor;
out vec2 outUV;


void main()
{
	mat4 boneTransform = gBones[BoneIDs[0]] * Weights[0];
	for(int i=1; i<4; i++)
		boneTransform += gBones[BonesID[i]] * weights[i];

	vec4 newPos = boneTransform * vec4(vertex, 1.0);
	gl_Position = m_ModelviewProjection * vec4(vertex, 1.0);
	outUV = UV;

	vec4 newNormal = boneTransform * vec4(normal, 0.0);

	outNormal = (m_ModelMatrix * newNormal).xyz
	outVertex = (m_ModelMatrix * newPos).xyz

	outTangent 	= tangent;
	outColor 	= color;
}
