#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 UV;


out vec3 outPosition;	// the vector that points the vertices itself
out vec3 outNormal;
out vec3 outColor;
out vec2 outUV;


uniform mat4 m_ModelviewProjection;

void main()
{
	gl_Position = m_ModelviewProjection*vec4(position,1.0);

	outPosition = position;	// object space
	outNormal = normal;
	outColor=color;
	outUV = UV;
}


/*
#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 UV;

out vec3 vertexVector;	// the vector that points the vertices itself
out vec3 outNormal;
out vec3 outColor;
out vec2 outUV;


uniform mat4 m_ModelviewProjection;

void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);

	vertexVector = vertex;	// object space
	outNormal = normal;
	outColor=color;
	outUV = UV;
}
*/
