#version 330
// layout states that the inputs are triangles, and the output are triangle strips, each with 3 vertices
// http://www.lighthouse3d.com/tutorials/glsl-core-tutorial/geometry-shader-examples/
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
 
in int vInstance[3];
out float gLayer;
 
uniform float InverseSize;

// http://prideout.net/blog/?p=60
// gl_IntanceID is only accessible from the vertex shader
// gl_Layer output variable is only accessible from the geometry shader 

void main()
{
	// gl_in is a built-in variable that allows us to access information about the vertices
    gl_Layer = vInstance[0];
    gLayer = float(gl_Layer) + 0.5;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}