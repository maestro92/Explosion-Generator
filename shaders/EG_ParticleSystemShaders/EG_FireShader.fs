#version 330                                                                        
                                                                                    
uniform sampler2D u_texture;                                                        
                                                                                    
in vec2 TexCoord;                                                                   
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{                                                                                                  
    FragColor = vec4(1.0,0.0,0.0,1.0);
 //   FragColor = texture2D(gTexture, TexCoord);   
                                                                           
    if (FragColor.r == 0.0 && FragColor.g == 0.0 && FragColor.b == 0.0) {
        discard;                                                                    
    }                                                                          
}
