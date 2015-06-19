#version 330

layout (location = 0) in vec3 Position;
layout (location = 3) in vec2 UV;

uniform mat4 u_modelViewProjMat;  
uniform float u_depth;

out vec2 vf_UV;

void main()
{
	gl_Position = u_modelViewProjMat * vec4(Position, 1.0);
//	gl_Position.z = 0.5;
	vf_UV = UV;
}