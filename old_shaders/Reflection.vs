#version 330

in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;

out vec3 outNormal;
out vec2 outUV;
out vec3 worldVertexPosition;
out vec3 worldNormalDirection;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main()
{
	gl_Position=modelViewProjectionMatrix * vec4(vertex,1.0);
	worldVertexPosition = vec3(modelMatrix * vec4(vertex,1.0));
	worldNormalDirection = mat3(modelMatrix) * normal;
}
