attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;


varying vec3 outNormal;
varying vec3 outTangent;
varying vec3 outColor;
varying vec2 outUV;
varying vec3 position;


uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;





void main()
{
	gl_Position=gl_ModelViewProjectionMatrix*vec4(vertex,1.0);
	position=vec3(gl_ModelViewMatrix*vec4(vertex,1.0));
	outNormal=gl_NormalMatrix*normal;
	outTangent=gl_NormalMatrix*outTangent;
	outColor=color;
	outUV=UV;
}

/*

attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying vec3 outNormal;
varying vec3 outTangent;
varying vec3 outColor;
varying vec2 outUV;
varying vec3 position;

void main()
{
	gl_Position=gl_ModelViewProjectionMatrix*vec4(vertex,1.0);
	position=vec3(gl_ModelViewMatrix*vec4(vertex,1.0));
	outNormal=gl_NormalMatrix*normal;
	outTangent=gl_NormalMatrix*outTangent;
	outColor=color;
	outUV=UV;
}
*/