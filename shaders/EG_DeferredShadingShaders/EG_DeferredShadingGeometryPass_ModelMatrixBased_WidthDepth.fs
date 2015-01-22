#version 330
                                                                        
in vec3 WorldPos0;                                                        
in vec3 Normal0;     
in vec3 outVertex;                                                               
in vec3 outTangent;
in vec3 outColor;                                               
in vec2 TexCoord0;    

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;    
layout (location = 4) out vec3 StencilOut;											              
layout (location = 5) out vec3 DepthOut;


uniform vec3 stencilFlag;
uniform mat4 m_ModelviewProjection;


float CalcDepthValue(vec3 worldPos)
{
    vec4 Clip_DepthPosition = m_ModelviewProjection * vec4(worldPos,1.0);
    float depthValue = Clip_DepthPosition.z / Clip_DepthPosition.w; // Depth in NDC coordinates
    return (depthValue + 1.0) * 0.5;    // Depth value in range [0,1], same range as the depth buffer
}




void main()									
{								

	float depthValue = CalcDepthValue(outVertex);
			
	WorldPosOut     = WorldPos0;	
	
	
	// depends if you had texture or color
	// when we saved our spheres from blender, we also had color values saved in the obj file
	// so that's what color/outColor is about
	// texture is something separate.
					
//	DiffuseOut      = texture(gColorMap, TexCoord0).xyz;	
	DiffuseOut      = outColor;	
//	DiffuseOut		= vec3(1.0,0.0,0.0);
	NormalOut       = normalize(Normal0);					
	TexCoordOut     = vec3(TexCoord0, 0.0);	

	StencilOut		= stencilFlag;		
	DepthOut		= vec3(depthValue, depthValue, depthValue);
}
