#version 330

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 UV;

out vec2 vf_UV;

uniform mat4 u_modelViewProjMat;
uniform mat4 u_mvp;
void main()
{

//	gl_Position = u_modelViewProjMat * vec4(position,1.0);
/*
	vec3 pos = position;
	pos.x = 0.0f;
	pos.z = 0.0f;
*/
	gl_Position = u_mvp * vec4(position,1.0);
	vf_UV = UV;
}
