#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 color;
layout (location = 4) in vec2 UV;                                       
layout (location = 5) in mat4 WVP;
layout (location = 9) in mat4 World;

                 
out vec3 outWorldPos;
out vec3 outNormal;
out vec3 outTangent;
out vec3 outColor;
out vec2 outUV;
                                                                                    
void main()                                                                         
{                                                                                   
    gl_Position = WVP * vec4(position, 1.0);                                        
    outWorldPos = (World * vec4(position, 1.0)).xyz;                                
    outNormal   = (World * vec4(normal, 0.0)).xyz;                                  
    outUV   	= UV;                                                                          
}