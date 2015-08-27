#version 330
in vec2 vf_UV;
uniform sampler2D u_texture;

out vec4 FragColor;

void main()
{
	FragColor = texture2D(u_texture, vf_UV);
//	FragColor = vec4(1.0,0.0,0.0,1.0);
}