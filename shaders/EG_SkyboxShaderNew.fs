
#version 330

uniform samplerCube u_cubeMapTexture;

in vec3 vf_position;
in vec3 vf_normal;
in vec3 vf_color;
in vec2 vf_UV;

out vec4 FragColor;
void main()
{
	FragColor = textureCube(u_cubeMapTexture, vf_position);
}






