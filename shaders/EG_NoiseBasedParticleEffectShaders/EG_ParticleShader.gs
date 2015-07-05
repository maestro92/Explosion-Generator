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

uniform mat3 u_modelViewMat2;
uniform mat4 u_modelViewProjMat2;                                                                   
uniform vec3 u_cameraPosition;    
uniform vec3 u_cameraViewDir;

//uniform float u_pointSize;
float pointSize = 0.2;

in vec3 vg_position[1];
in vec3 vg_velocity[1];
in float vg_alpha[1];

out float gf_alpha;
out vec2 gf_UV;
out vec3 gf_position;

const float Epsilon = 0.0000001;


void main()                                                                         
{                     
#if 1

    vec3 u = u_modelViewMat2 * vg_velocity[0];
//    vec3 u = u_modelViewMat2 * vg_velocity[0];
    float w = pointSize;
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

    // mix: linear interpolate between two values
    // the manual says mix(x,y,a), and the equation is x*(1-a) + y*a
    // so a (or t in our case) should be less than 1

    // Lerp the orientation vector if screen-space velocity is negligible
    u = normalize( mix(u,vec3(1,0,0),t));
    h = mix(h,w,t);

    // Compute the change-of-basis matrix for the billboards
    vec3 v = vec3(-u.y, u.x, 0);
    vec3 a = u * u_modelViewMat2;
    vec3 b = v * u_modelViewMat2;
    vec3 c = cross(a,b);
    mat3 basis = mat3(a,b,c);

    // Compute the four offset vectors
    vec3 N = basis * vec3(0,w,0);
    vec3 S = basis * vec3(0,-w,0);
    vec3 E = basis * vec3(-h,0,0);
    vec3 W = basis * vec3(h,0,0);
    
    /*
    // Emit the quad:
//    vec3 p = vg_position[0];
    vec3 p = gl_in[0].gl_Position.xyz;  
    gf_alpha = vg_alpha[0];
    gf_UV = vec2(0.0,0.0); gl_Position = u_modelViewProjMat2 * vec4(p+N+W,1); EmitVertex();
    gf_UV = vec2(1.0,0.0); gl_Position = u_modelViewProjMat2 * vec4(p+N+E,1); EmitVertex();
    gf_UV = vec2(0.0,1.0); gl_Position = u_modelViewProjMat2 * vec4(p+S+W,1); EmitVertex();
    gf_UV = vec2(1.0,1.0); gl_Position = u_modelViewProjMat2 * vec4(p+S+E,1); EmitVertex();
    EndPrimitive();    
    */
    
    
    
    vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(u_cameraPosition - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up);                                               
    right = normalize(right);
    up = cross(right, toCamera);
    up = normalize(up);

    basis = mat3(right, up, toCamera);
    #if 0
        N = basis * vec3(0,w,0);
        S = basis * vec3(0,-w,0);
        E = basis * vec3(-h,0,0);
        W = basis * vec3(h,0,0);
    #else
        N = 2*basis * vec3(0,h,0);
        S = basis * vec3(0,-h,0);
        E = basis * vec3(-w,0,0);
        W = basis * vec3(w,0,0);
    #endif

    gf_alpha = vg_alpha[0];
    #if 0
        Pos -= (right * 0.5);       gf_UV = vec2(0.0, 0.0); gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0); EmitVertex();                                                                   
        Pos += up;                  gf_UV = vec2(0.0, 1.0); gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0); EmitVertex();                                                                   
        Pos -= up; Pos += right;    gf_UV = vec2(1.0, 0.0); gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0); EmitVertex();                                                                   
        Pos += up;                  gf_UV = vec2(1.0, 1.0); gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0); EmitVertex();   
    #else
     //   Pos -= (right * 0.5);       
        gf_UV = vec2(0.0, 0.0); gl_Position = u_modelViewProjMat2 * vec4(Pos+N+W,1); EmitVertex();   
                                                                        
     //   Pos += up;                  
        gf_UV = vec2(0.0, 1.0); gl_Position = u_modelViewProjMat2 * vec4(Pos+N+E,1); EmitVertex();   
                                                                        
     //   Pos -= up; Pos += right;    
        gf_UV = vec2(1.0, 0.0); gl_Position = u_modelViewProjMat2 * vec4(Pos+S+W,1); EmitVertex();      
                                                                     
     //   Pos += up;                  
        gf_UV = vec2(1.0, 1.0); gl_Position = u_modelViewProjMat2 * vec4(Pos+S+E,1); EmitVertex();   
        EndPrimitive(); 
    #endif     

#else

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
    gf_position = Pos;                                                  
    EmitVertex();                                                                   
                                                                                    
    Pos += up;                                                                   
    gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0);                                             
    gf_UV = vec2(0.0, 1.0);  
    gf_position = Pos;                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos -= up;                                                                   
    Pos += right;                                                                   
    gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0);                                             
    gf_UV = vec2(1.0, 0.0);   
    gf_position = Pos;                                                     
    EmitVertex();                                                                   
                                                                                    
    Pos += up;                                                                   
    gl_Position = u_modelViewProjMat2 * vec4(Pos, 1.0);                                             
    gf_UV = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();   

#endif                                           
}                                                                                   
