#version 330
uniform sampler2D texture;
uniform vec2 pixelSize; //1.0/640.0, 1.0/480.0
in vec2 outUV;
out vec4 FragColor;
void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize;		//0..640, 0..480 
	FragColor=texture2D(texture,pos);
//	FragColor = vec4(0.0,0.0,1.0,0.0);
}


/*
uniform sampler2D texture;
uniform vec2 pixelSize; //==x: 1/640,y=1/480
varying vec2 outUV;

void main()
{
	vec2 pos=gl_FragCoord.xy*pixelSize;
	gl_FragColor=texture2D(texture,pos);
}
*/