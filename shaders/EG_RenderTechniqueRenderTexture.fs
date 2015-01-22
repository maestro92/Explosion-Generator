#version 330
uniform sampler2D gTexture;
uniform vec2 gPixelSize; //1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT
in vec2 outUV;
out vec4 FragColor;

void main()
{
	vec2 pos = gl_FragCoord.xy * gPixelSize;	
	FragColor=texture2D(gTexture,pos);
}


