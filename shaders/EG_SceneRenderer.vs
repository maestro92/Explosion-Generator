#version 330

in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;


uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat4 m_ViewMatrix;
uniform mat4 m_ProjectionMatrix;
uniform mat4 m_ModelMatrix;
uniform mat4 m_ViewNoRotateMatrix;
uniform mat3 m_normalMatrix;                               
uniform mat4 l_modelViewProjectionMatrix;

uniform mat4 u_lightModelViewProjMat;

out vec3 outVertex;					// position of the vertex (the specific vertex we're looking at right now) in EyeSpace, which is the desired correct position in 3D world
out vec4 lightVertexPosition;
out vec3 outNormal;
out vec3 outTangent;
out vec3 outColor;


void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);
	lightVertexPosition = l_modelViewProjectionMatrix * vec4(vertex,1.0);
	
//	lightVertexPosition = u_lightModelViewProjMat * vec4(vertex,1.0);
	

//	outVertex 	= vec3(m_ModelMatrix * vec4(vertex,1.0));
	outVertex 	= (m_ModelMatrix * vec4(vertex, 1.0)).xyz;



//	outVertex1	= vertex;
    outNormal 	= vec3(m_ModelMatrix * vec4(normal,0.0));   
	outTangent 	= tangent;
	outColor	= color;	
}

