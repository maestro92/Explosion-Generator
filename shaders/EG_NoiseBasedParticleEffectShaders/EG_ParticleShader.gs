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
uniform mat3 u_modelViewMat;
uniform mat4 u_modelViewProjMat;  

uniform mat4 u_modelViewProjMat2;                                                                   
uniform vec3 u_cameraPosition;    
uniform vec3 u_cameraViewDir;

uniform float u_pointSize;

in vec3 vg_position[1];
in vec3 vg_velocity[1];
in float vg_alpha[1];

out float gf_alpha;
out vec2 gf_UV;

const float Epsilon = 0.0000001;


void main()                                                                         
{                     
#if 1
    vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(u_cameraPosition - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up);                                               
    right = normalize(right);

    up = cross(right, toCamera);
    up = normalize(up);

    gf_alpha = vg_alpha[0];
    Pos -= (right * 0.5);                                                           
    gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0);                                             
    gf_UV = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos += up;                                                                   
    gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0);                                             
    gf_UV = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos -= up;                                                                   
    Pos += right;                                                                   
    gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0);                                             
    gf_UV = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos += up;                                                                   
    gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0);                                             
    gf_UV = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();   
    
#else  

    vec3 u = u_modelViewMat * vg_velocity[0];
    float w = u_pointSize;
    float h = w * 2.0;


    // Determine 't', which represents Z-aligned magnitude.
    // By default, t = 0.0.
    // If velocity aligns with Z, increase t towards 1.0.
    // If total speed is negligible, increase t towards 1.0.
    float t = 0.0;
    float nz = abs(normalize(u).z);
    if (nz > 1.0 - Epsilon)
        t = (nz - (1.0 - Epsilon)) / Epsilon;
    else if (dot(u,u) < Epsilon)
        t = (Epsilon - dot(u,u)) / Epsilon;


    // Compute screen-space velocity
    u.z = 0.0;
    u = normalize(u);

    // Lerp the orientation vector if screen-space velocity is negligible
    u = normalize( mix(u,vec3(1,0,0),t));
    h = mix(h,w,t);

    // Compute the change-of-basis matrix for the billboards
    vec3 v = vec3(-u.y, u.x, 0);
    vec3 a = u * u_modelViewMat;
    vec3 b = v * u_modelViewMat;
    vec3 c = cross(a,b);
    mat3 basis = mat3(a,b,c);

    // Compute the four offset vectors
    vec3 N = basis * vec3(0,w,0);
    vec3 S = basis * vec3(0,-w,0);
    vec3 E = basis * vec3(-h,0,0);
    vec3 W = basis * vec3(h,0,0);
    
    // Emit the quad:
    vec3 p = vg_position[0];
    gf_alpha = vg_alpha[0];
    gf_UV = vec2(0,0); gl_Position = u_modelViewProjMat * vec4(p+N+W,1); EmitVertex();
    gf_UV = vec2(1,0); gl_Position = u_modelViewProjMat * vec4(p+N+E,1); EmitVertex();
    gf_UV = vec2(0,1); gl_Position = u_modelViewProjMat * vec4(p+S+W,1); EmitVertex();
    gf_UV = vec2(1,1); gl_Position = u_modelViewProjMat * vec4(p+S+E,1); EmitVertex();
    EndPrimitive();     
#endif                                                         
}                                                                                   



/*
                                                              
    vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(gCameraPosition - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
 //   vec3 right = cross(toCamera, up);                                               
    vec3 right = cross(gCameraViewDir, up);
                     
    // we're growing a quad around a point
    
    // the initial point is considered to be
    // at the center of the bottom of the quad                                                             
    
    // generating two front facing triangles from it
    // subtracting half of right 
    Pos -= (right * 0.5);                                                           
    gl_Position = gu_modelViewProjMatMatrix * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += 1.0;                                                                   
    gl_Position = gu_modelViewProjMatMatrix * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y -= 1.0;                                                                   
    Pos += right;                                                                   
    gl_Position = gu_modelViewProjMatMatrix * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += 1.0;                                                                   
    gl_Position = gu_modelViewProjMatMatrix * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();    
    */   