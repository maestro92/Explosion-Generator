#version 330
in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

out vec3 outNormal;
out vec3 outNormal2;
out vec3 outTangent;
out vec3 outColor;
out vec3 position;
out vec2 outUV;
out mat4 outMV_Matrix;
out mat3 outNormalMatrix;




void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	position=vec3(modelViewMatrix*vec4(vertex,1.0));
	outNormal=normalMatrix*normal;
	outNormal2 = normal;
	outTangent=normalMatrix*outTangent;
	outColor=color;
	outUV = UV;
	
	outMV_Matrix = modelViewMatrix;
	outNormalMatrix = normalMatrix;


//	PointLightPos_EyeSpace = vec3(PointLightPos_ModelViewMatrix * vec4(PointlightPos,1.0));

}


