#version 330
in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;


out vec3 vertexVector;	// the vector that points the vertices itself
out vec3 outNormal;
out vec3 outTangent;
out vec3 outColor;
out vec2 outUV;


uniform mat4 m_ModelviewProjection;

void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);

	vertexVector = vertex;	// object space
	outNormal = normal;
	outTangent = tangent;
	outColor=color;
	outUV = UV;
}

/*
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;




void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);

	vertexVector = vertex;	// object space
	outNormal = normal;
	outTangent = tangent;
	outColor=color;
	outUV = UV;
}
*/