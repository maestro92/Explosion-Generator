#version 330

in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;


out vec3 w_vertex;
out vec3 w_normal;
out vec3 outVertex;
out vec3 outTangent;
out vec3 outColor;
out vec2 outUV;


uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat4 m_ViewMatrix;
uniform mat4 m_ProjectionMatrix;
uniform mat4 m_ModelMatrix;
uniform mat4 m_ViewNoRotateMatrix;
uniform mat3 m_normalMatrix;    


void main()
{
	gl_Position = m_ModelviewProjection * vec4(vertex,1.0);
    w_vertex	= vec3(m_ModelMatrix * vec4(vertex,1.0));
    w_normal	= mat3(m_ModelMatrix) * normal;
}

