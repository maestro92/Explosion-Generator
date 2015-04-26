#version 330

uniform mat4 m_ModelviewProjection;

layout (location = 0) in vec3 vertex;

void main()
{
	gl_Position= m_ModelviewProjection*vec4(vertex,1.0);
}



