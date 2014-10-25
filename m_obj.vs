attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

varying vec3 outNormal;
varying vec3 outTangent;
varying vec3 outColor;
varying vec2 outUV;
varying vec3 position;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	position=vec3(modelViewMatrix*vec4(vertex,1.0));
	outNormal=normalMatrix*normal;
	outTangent=normalMatrix*outTangent;
	outColor=color;
	outUV=UV;
}
