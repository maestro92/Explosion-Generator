#version 330

uniform mat4 m_ModelviewProjection;

layout (location = 0) in vec3 vertex;
layout (location = 2) in vec3 color;

out vec3 outColor;

void main()
{
	outColor = color;
	gl_Position= m_ModelviewProjection*vec4(vertex,1.0);
}



