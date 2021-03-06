#version 330                                                                        

in vec2 vf_UV;                                                                                   
uniform sampler2D u_backgroundTexture;                                                        
uniform sampler2D u_particlesTexture;                                                        
                                                                                                                                   
out vec4 FragColor;                                                                 
      
                                                                                    
void main()                                                                         
{                                                                                                  
	vec2 tc = vf_UV;	tc.y = 1.0 - tc.y;

	vec4 dest = texture2D(u_backgroundTexture, tc);
	vec4 src = texture2D(u_particlesTexture, tc);

	float particle_alpha = 1 - src.a;

	FragColor.rgb = src.rgb * particle_alpha + dest.rgb * (1.0 - particle_alpha);
	FragColor.a = 1.0;   
	
	/*
	// a is the contribution from the src.rgb (we calculate it through alpha
	// remember int he particleTexture, the particles part are from [1,0], the fullest particle has alpha value 0.0, the actual background has alpha value 1.0
	float particle_alpha = 1 - src.a;

	if(a==1.0)
		FragColor = vec4(0.0,1.0,0.0,1.0);
	else if(a==0.0)
	{
		FragColor.rgb = dest.rgb * (1.0 - a);
		FragColor.a = 1.0;
	}	
	else
		FragColor = vec4(1.0,1.0,0.0,1.0);
*/
//	FragColor.rgb = src.rgb * particle_alpha + dest.rgb * (1.0 - particle_alpha);
//	FragColor.a = 1.0;   


//	FragColor = dest;
//	FragColor = src;
//	FragColor.a = 1.0f;
/*
	if(a == 1.0f)
	{
		FragColor = vec4(0.0,0.0,1.0,1.0);
	}
	else
	{
		FragColor = vec4(0.0,1.0,0.0,1.0);
	}
*/



//	FragColor = dest;
//	FragColor = src;

//	FragColor = dest;                                                   
//	FragColor = src;
//	FragColor = vec4(1.0,0.0,0.0,1.0);











/*
	vec2 tc = vf_UV;	tc.y = 1.0 - tc.y;

	vec4 dest = texture2D(u_backgroundTexture, tc);
	vec4 src = texture2D(u_particlesTexture, tc);

	float particle_alpha = 1 - src.a;

	FragColor.rgb = src.rgb * particle_alpha + dest.rgb * (1.0 - particle_alpha);
	FragColor.a = 1.0;   
*/
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