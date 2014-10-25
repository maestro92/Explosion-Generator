attribute vec3 vertex;

uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
}
