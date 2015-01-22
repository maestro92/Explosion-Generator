#version 330
                                                                        
in vec3 WorldPos0;                                                        
in vec3 Normal0;                                                                    
in vec3 outTangent;
in vec3 outColor;                                               
in vec2 TexCoord0;    

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;    
layout (location = 4) out vec3 StencilOut;											              



uniform vec3 stencilFlag;


void main()									
{											
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
}
