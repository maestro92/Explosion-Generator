#version 330

uniform mat4 u_modelViewProjMat;

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 UV;

out vec2 vf_UV;

void main()
{
	vf_UV = UV;
	gl_Position= u_modelViewProjMat * vec4(position,1.0);
}



