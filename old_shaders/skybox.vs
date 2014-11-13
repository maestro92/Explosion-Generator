#version 330
in vec3 vertex;
in vec3 color;

out vec3 outColor;
out vec3 vertexVector;	// the vector that points the vertices itself


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;




void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	outColor=color;
	vertexVector = vertex;	// object space
}
