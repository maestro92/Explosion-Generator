#version 330

layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;  

// we are going emit a quad for each incoming vertex, so we
// declare the maximum as four vertices

// geometry shader 
// vs: per vertex
// fs: per pixel
// gs: per primitive (dot, line, triangle)

// so here we get one triangle
uniform mat4 u_modelViewProjMat;                                                                   
uniform vec3 u_cameraPosition;    
uniform vec3 u_cameraViewDir;

out vec2 TexCoord;

void main()                                                                         
{                     
 //   mat4 mvp = gModelViewProjectionMatrix;
                                                              
    vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(u_cameraPosition - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up);                                               
    right = normalize(right);

    up = cross(right, toCamera);
    up = normalize(up);

    // we're growing a quad around a point
    
    // the initial point is considered to be
    // at the center of the bottom of the quad                                                             
    
    // generating two front facing triangles from it
    // subtracting half of right 
    Pos -= (right * 0.5);                                                           
    gl_Position = u_modelViewProjMat * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos += up;                                                                   
    gl_Position = u_modelViewProjMat * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos -= up;                                                                   
    Pos += right;                                                                   
    gl_Position = u_modelViewProjMat * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos += up;                                                                   
    gl_Position = u_modelViewProjMat * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();        
}
