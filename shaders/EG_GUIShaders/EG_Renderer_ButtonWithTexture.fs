#version 330
in vec2 outUV;
uniform sampler2D gTextures;

out vec4 FragColor;

void main()
{
//	FragColor = texture2D(Sampler, outUV);

	FragColor = vec4(1.0,0.0,0.0,1.0);
}