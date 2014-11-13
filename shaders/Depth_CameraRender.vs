#version 330
in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;

//attribute vec3 vertex;

#if 0
uniform mat4 modelViewProjectionMatrix;
void main()
{
	gl_Position = modelViewProjectionMatrix*vec4(vertex,1.0);
}

#else

uniform mat4 m_ModelviewProjection;
void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);
}

#endif