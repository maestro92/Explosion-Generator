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


#if 1
uniform mat4 m_ModelviewProjection;
uniform mat4 m_Modelview;
uniform mat3 m_normalMatrix;
uniform mat4 l_modelViewProjectionMatrix;
#else
uniform mat4 modelViewProjectionMatrix;
uniform mat4 l_modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
#endif

uniform mat4 lightPosition_ModelViewMatrix;	//ModelViewMatrix for the light position
uniform vec3 lightPosition_ObjectSpace;  //Light's actual position 



out vec4 lightVertexPosition;		// vertex seen in light's clip space
out vec3 outNormal;
out vec3 outTangent;
out vec3 outColor;


out vec3 position;					// position of the vertex (the specific vertex we're looking at right now) in EyeSpace, which is the desired correct position in 3D world
out vec3 LightPosition_EyeSpace;  // lightPosition_ObjectSpace * ModelView_Matrix, so lightPosition_ObjectSpace is in the actual correct position in the 3D world

out mat3 l_normalMatrix;

// out mat3 l_normalMatrix;


#if 01
void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);
	lightVertexPosition = l_modelViewProjectionMatrix * vec4(vertex,1.0);

	// per-fragment shading part
	position = vec3(m_Modelview*vec4(vertex,1.0));
	outNormal = m_normalMatrix * normal;
	outTangent = tangent;
	outColor=color;	

	LightPosition_EyeSpace = vec3(lightPosition_ModelViewMatrix * vec4(lightPosition_ObjectSpace,1.0));
	l_normalMatrix = mat3(lightPosition_ModelViewMatrix);
}

#else

void main()
{
	gl_Position = modelViewProjectionMatrix*vec4(vertex,1.0);
	lightVertexPosition = l_modelViewProjectionMatrix * vec4(vertex,1.0);

	// per-fragment shading part
	position = vec3(modelViewMatrix*vec4(vertex,1.0));
	outNormal = normalMatrix * normal;
	outTangent = tangent;
	outColor=color;	
	LightPosition_EyeSpace = vec3(LightPosition_ModelViewMatrix * vec4(lightPosition_ObjectSpace,1.0));

	l_normalMatrix = mat3(LightPosition_ModelViewMatrix);
	LightActualPosition = lightPosition_ObjectSpace;
}
#endif