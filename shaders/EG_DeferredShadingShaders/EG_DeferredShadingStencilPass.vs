#version 330                                                                        
     

in vec3 vertex;                                                               
in vec3 normal;                                                                    
in vec3 tangent;  
in vec3 color;  
in vec2 UV;  


//uniform mat4 gWVP;
//uniform mat4 gWorld;
                
uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat4 m_ViewMatrix;
uniform mat4 m_ProjectionMatrix;
uniform mat4 m_ModelMatrix;
uniform mat4 m_ViewNoRotateMatrix;
uniform mat3 m_normalMatrix;                
                                           
                                                               
out vec3 Normal0;    
out vec3 outTangent;                                                               
out vec3 outColor;                                                                 
out vec2 TexCoord0;  

out vec3 WorldPos0;

void main()
{       
    gl_Position    = m_ModelviewProjection * vec4(vertex, 1.0);
//    Normal0        = (m_ModelMatrix * vec4(normal, 0.0)).xyz;   
    Normal0        = m_normalMatrix * normal;   
    outTangent     = tangent;
    outColor	   = color;		
    TexCoord0      = UV;        

    WorldPos0      = (m_ModelMatrix * vec4(vertex, 1.0)).xyz;
}