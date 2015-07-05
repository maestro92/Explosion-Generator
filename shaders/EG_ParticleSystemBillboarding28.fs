#version 330                                                                        
                                                                                    
uniform sampler2D gColorMap;                                                        
                                                                                    
in vec2 TexCoord;                                                                   
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{            


                    
                        /*                                                  
    if (FragColor.r == 0.0 && FragColor.g == 0.0 && FragColor.b == 0.0) 
    {           
        discard;                                                                    
    } 
                      */  
                    
    FragColor = texture2D(gColorMap, TexCoord);                       
    
    FragColor = vec4(1.0,1.0,0.0,1.0);                                                                       
    if (FragColor.r >= 0.9 && FragColor.g >= 0.9 && FragColor.b >= 0.9) 
    {           
        discard;                                                                    
    } 
                                                                               
}