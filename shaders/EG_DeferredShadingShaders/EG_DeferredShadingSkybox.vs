#version 330

in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;

uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat4 m_ViewMatrix;
uniform mat4 m_ProjectionMatrix;
uniform mat4 m_ModelMatrix;
uniform mat4 m_ViewNoRotateMatrix;
uniform mat3 m_normalMatrix;    

out vec3 vertexVector;	// the vector that points the vertices itself
out vec3 outNormal;
out vec3 outTangent;
out vec3 outColor;
out vec2 outUV;
out vec3 WorldPos0; 


void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);
	vertexVector = vertex;	// object space
	outNormal = normal;
	outTangent = tangent;
	outColor=color;
	outUV = UV;
    WorldPos0      = (m_ModelMatrix * vec4(vertex, 1.0)).xyz;
}