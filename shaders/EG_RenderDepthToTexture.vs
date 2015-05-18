#version 330
in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;



uniform mat4 m_ModelviewProjection;
void main()
{
	gl_Position = m_ModelviewProjection*vec4(position,1.0);
}

