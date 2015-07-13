#version 330

uniform sampler2D u_texture;
uniform sampler2D u_sceneDepthTexture;
uniform vec2 u_inverseSize;
uniform float u_alpha;

in vec2 vf_UV;
out vec4 FragColor;

void main()
{

    vec2 tc = gl_FragCoord.xy * u_inverseSize;
    float depth = texture2D(u_sceneDepthTexture, tc).r;

    if(depth < gl_FragCoord.z)
        discard;


	tc = vf_UV;	 tc.y = 1.0 - tc.y;
	FragColor = texture(u_texture, tc);
	FragColor.a = u_alpha;
    if (FragColor.r == 0.0 && FragColor.g == 0.0 && FragColor.b == 0.0) 
    {
        discard;                                                                    
    }  
}


