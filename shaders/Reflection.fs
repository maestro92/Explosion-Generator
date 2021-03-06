#version 330

uniform samplerCube m_reflectionTexture;
uniform vec3 		m_cameraPosition;


in vec3 outColor;
in vec3 vertexVector;

in vec3 outNormal;
in vec2 outUV;
in vec3 worldVertexPosition;
in vec3 worldNormalDirection;

out vec4 FragColor;
void main()
{
	vec3 incident = normalize(worldVertexPosition - m_cameraPosition);
	vec3 normal = normalize(worldNormalDirection);
	vec3 reflected = reflect(incident,normal);
	FragColor = texture(m_reflectionTexture, reflected);

}




