#version 330
uniform sampler2D gTexture;
uniform vec2 gPixelSize; //1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT
in vec2 outUV;
out vec4 FragColor;

void main()
{
	vec2 pos = gl_FragCoord.xy * gPixelSize;	
	FragColor = vec4(1.0,0.0,0.0,1.0);
	return;
// if it's a depth texture
#if 1
	vec4 value = texture2D(gTexture,pos);
	value.x = pow(value.x,32);
	value.y = pow(value.y,32);
	value.z = pow(value.z,32);
	FragColor = value;
#else
	FragColor = texture2D(gTexture,pos);
#endif
}


