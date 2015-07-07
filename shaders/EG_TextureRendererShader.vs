#version 330

/*
in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;
*/
layout (location = 0) in vec3 vertex;
layout (location = 3) in vec2 UV;

out vec2 outUV;

uniform mat4 m_ModelviewProjection;

void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);
	outUV=UV;
}
