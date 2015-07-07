#version 330                                                                        
                                                                                    
uniform vec4 u_color;
uniform vec2 u_inverseSize;
uniform sampler2D u_depthTexture;
uniform sampler2D u_spriteTexture;

in float gf_alpha;
in vec2 gf_UV;
                                                                                   
out vec4 FragColor;                                                                 


float zNear = 0.5;
float zFar = 1000.0;
                
                  
float computeZ(float z_b)
{
    float numer = 2.0 * zFar * zNear;
    float denom = zFar + zNear - (zFar - zNear) * (2*z_b - 1);
    float z_e = numer / denom;
    return z_e;
}

                  
                                                                                    
void main()                                                                         
{            

    vec2 tc = gl_FragCoord.xy * u_inverseSize;
    float depth = texture2D(u_depthTexture, tc).r;

    if(depth < gl_FragCoord.z)
        discard;

    float d = depth;
    float softness = 1.0 - min(1.0, 40.0*d);
    softness *= softness;
    softness = 1.0 - softness * softness;
    
    float A = gf_alpha * texture2D(u_spriteTexture, gf_UV).r;                                                                           
    FragColor = u_color * vec4(1,1,1, A*softness);                                                                               

//    FragColor = vec4(1.0,0.0,0.0,1.0);
}



