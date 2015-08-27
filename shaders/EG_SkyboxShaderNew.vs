#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 UV;


out vec3 vf_position;	// the vector that points the vertices itself
out vec3 vf_normal;
out vec3 vf_color;
out vec2 vf_UV;


uniform mat4 u_modelViewProjMat;

void main()
{
	gl_Position = u_modelViewProjMat*vec4(position,1.0);

	vf_position = position;	// object space
	vf_normal 	= normal;
	vf_color	= color;
	vf_UV 		= UV;
}


