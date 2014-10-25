
void main()
{
    // obtains the GL_Projection matrix (the one you set up with glMatrixMode(GL_PROECTION))
    // as well as the glMatrixMode(GL_MODELVIEW)

    // the vertex shader is responsible for at least writing one variable
    // gl_position
	// gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = gl_Vertex;
}