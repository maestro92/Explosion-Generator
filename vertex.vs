#version 120
out vec3 position;
out vec3 normal;



void main()
{
    // obtains the GL_Projection matrix (the one you set up with glMatrixMode(GL_PROECTION))
    // as well as the glMatrixMode(GL_MODELVIEW)

    // the vertex shader is responsible for at least writing one variable
    // gl_position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = gl_NormalMatrix * gl_Normal;
	// the glNormalMatrix; inverse tranpose of the current ModelViewMatrix
	// gl_Normal: the current normal vector: Example glNormal3f(0.0,0.0,1.0)
}