#version 330
in vec3 vertex;
in vec2 UV;

out vec2 outUV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	outUV=UV;
}
