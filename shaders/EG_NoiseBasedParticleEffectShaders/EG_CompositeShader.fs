#version 330                                                                        

in vec2 vf_UV;                     
uniform vec2 u_pixelSize; //1.0/SCREEN_WIDTH, 1.0/SCREEN_HEIGHT                                                                  
uniform sampler2D u_backgroundTexture;                                                        
uniform sampler2D u_particlesTexture;                                                        
                                                                                                                                   
out vec4 FragColor;                                                                 
      
                                                                                    
void main()                                                                         
{                                                                                                  
	vec2 tc = vf_UV;	tc.y = 1.0 - tc.y;

	vec4 dest = texture2D(u_backgroundTexture, tc);
	vec4 src = texture2D(u_particlesTexture, tc);

	float a = 1.0 - src.a;

//	FragColor = dest;

	if(src.a == 1.0f)
	{
		FragColor = src;
	}
	else
	{
		FragColor = dest;
	}


//	FragColor.rgb = src.rgb * a + dest.rgb * (1.0 - a);
//	FragColor.a = 1.0;   


//	FragColor = dest;
//	FragColor = src;

//	FragColor = dest;                                                   
//	FragColor = src;
//	FragColor = vec4(1.0,0.0,0.0,1.0);
}



      /*                                                                              
void main()                                                                         
{                                                                                                  
	vec2 tc = vf_UV;	tc.y = 1.0 - tc.y;

//	vec2 pos = vec2(0.5,0.5);	

	vec4 dest = texture2D(u_backgroundTexture, tc);
	vec4 src = texture2D(u_particlesTexture, tc);

//	float a = 1.0 - src.a;
//	FragColor.rgb = src.rgb * a + dest.rgb * (1.0-a);
//	FragColor.a = 1.0;   


	float a = 1.0 - dest.a;

	FragColor = dest;

	return;

	if(a==0.0f)
	{
//		FragColor = vec4(src.a,src.a,src.a,1.0);
//		return;
//		FragColor.rgb = src.rgb;
//		FragColor.a = 1.0f; 
		FragColor = vec4(1.0,0.0,0.0,1.0);
		return;
	}

//	FragColor = dest;
//	FragColor.a = 1.0f;

//	FragColor.rgb = src.rgb;
//	FragColor.rgb = src.rgb * a + dest.rgb * (1.0 - a);
//	FragColor.a = 1.0;   

	

//	FragColor = dest;                                                   
//	FragColor = src;
//	FragColor = vec4(1.0,0.0,0.0,1.0);
}
*/