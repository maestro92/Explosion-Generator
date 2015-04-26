#version 330

uniform mat4 m_ModelviewProjection;

layout (location = 0) in vec3 vertex;
layout (location = 3) in vec2 UV;

out vec2 outUV;


void main()
{
	outUV = UV;
	gl_Position= m_ModelviewProjection*vec4(vertex,1.0);
}



