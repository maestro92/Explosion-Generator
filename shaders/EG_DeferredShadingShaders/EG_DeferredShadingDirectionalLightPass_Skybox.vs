
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

    


uniform mat4 lightPosition_ModelViewMatrix;	//ModelViewMatrix for the light position
uniform vec3 lightPosition_ObjectSpace;  //Light's actual position 



out vec3 outVertex;
out vec3 outNormal;
out vec3 outTangent;
out vec3 outColor;
out vec2 outUV;


out vec4 vertexPosition_LightEyeSpace;		// vertex seen in light's clip space
out vec3 lightPosition_CameraEyeSpace;  // lightPosition_ObjectSpace * ModelView_Matrix, so lightPosition_ObjectSpace is in the actual correct position in the 3D world

out mat3 l_normalMatrix;



void main()
{
	gl_Position = m_ModelviewProjection*vec4(vertex,1.0);
}

