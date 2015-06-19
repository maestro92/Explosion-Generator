#version 330                                                                        
                                                                                    
uniform vec4 u_color;
uniform vec2 u_inverseSize;
uniform sampler2D u_depthTexture;
uniform sampler2D u_spriteTexture;

in float gf_alpha;
in vec2 gf_UV;
                                                                                   
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{                                                                                                  
    vec2 tc = gl_FragCoord.xy * u_inverseSize;
    float depth = texture2D(u_depthTexture, tc).r;
    if(depth < gl_FragCoord.z)
        discard;
    
    float d = depth - gl_FragCoord.z;
    float softness = 1.0 - min(1.0, 40.0*d);
    softness *= softness;
    softness = 1.0 - softness * softness;
    
    float A = gf_alpha * texture2D(u_spriteTexture, gf_UV).r;
    FragColor = u_color * vec4(1,1,1, A*softness);                                                                               
}
