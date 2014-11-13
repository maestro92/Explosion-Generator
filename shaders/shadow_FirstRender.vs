#version 330
in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;

uniform mat4 m_ModelviewProjection;

void main()
{
	gl_Position= m_ModelviewProjection*vec4(vertex,1.0);
}



/*
#version 330
in vec3 vertex;
//attribute vec3 vertex;

uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
}
*/