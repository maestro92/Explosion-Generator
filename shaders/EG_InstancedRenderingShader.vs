#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 color;
layout (location = 4) in vec2 UV;                                       
layout (location = 5) in mat4 WVP;
layout (location = 9) in mat4 World;

                                                    
out vec2 TexCoord0;                                                                 
out vec3 Normal0;                                                                   
out vec3 WorldPos0;                                                                 
flat out int InstanceID;
                                                                                    
void main()                                                                         
{                                                                                   
    gl_Position = WVP * vec4(vertex, 1.0);                                        
    TexCoord0   = UV;                                                         
    Normal0     = (World * vec4(normal, 0.0)).xyz;                                  
    WorldPos0   = (World * vec4(vertex, 1.0)).xyz;                                
    InstanceID = gl_InstanceID;                                 
}