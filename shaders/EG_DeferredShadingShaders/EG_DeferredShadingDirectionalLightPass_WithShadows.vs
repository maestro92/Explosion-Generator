/*
attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 color;
*/
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

    
vec3 lightPosition = vec3(-19.1004, 28.881, 40.5246);

out vec3 outVertex;
out vec3 outNormal;
out vec3 outTangent;
out vec3 outColor;
out vec2 outUV;



void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);
}

