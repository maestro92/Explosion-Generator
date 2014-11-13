#version 330
out vec4 FragColor;

uniform sampler3D VelocityTexture;
uniform sampler3D SourceTexture;
uniform sampler3D Obstacles;

uniform vec3 InverseSize;
uniform float TimeStep;
uniform float Dissipation;

in float gLayer;

// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch30.html
// my attempt on MacCormack Advection Scheme


void main()
{
    vec3 fragCoord = vec3(gl_FragCoord.xy, gLayer);


	vec3 cellVelocity = texture(VelocityTexture, InverseSize * fragCoord).xyz;
	vec3 npos1 = fragCoord - TimeStep * cellVelocity;
	vec3 npos2 = fragCoord + TimeStep * cellVelocity;

	vec3 npos = floor(npos1 + vec3(0.5,0.5,0.5));
	npos = InverseSize * npos;



	vec3 ht = vec3(0.5*InverseSize.x, 0.5*InverseSize.y, 0.5*InverseSize.z);

	vec4 nodeValues[8];

	nodeValues[0] = texture(SourceTexture, npos+vec3(-ht.x, -ht.y, -ht.z));
	nodeValues[1] = texture(SourceTexture, npos+vec3(-ht.x, -ht.y, ht.z));
	nodeValues[2] = texture(SourceTexture, npos+vec3(-ht.x, ht.y, -ht.z));
	nodeValues[3] = texture(SourceTexture, npos+vec3(-ht.x, ht.y, ht.z));
	nodeValues[4] = texture(SourceTexture, npos+vec3(ht.x, -ht.y, -ht.z));
	nodeValues[5] = texture(SourceTexture, npos+vec3(ht.x, -ht.y, ht.z));
	nodeValues[6] = texture(SourceTexture, npos+vec3(ht.x, ht.y, -ht.z));
	nodeValues[7] = texture(SourceTexture, npos+vec3(ht.x, ht.y, ht.z));
	
	vec4 pMin = min(min(min(min(min(min(min(  
    nodeValues[0],  nodeValues [1]), nodeValues [2]), nodeValues [3]),  
    nodeValues[4]), nodeValues [5]), nodeValues [6]), nodeValues [7]); 
	
	vec4 pMax = max(max(max(max(max(max(max(  
    nodeValues[0],  nodeValues [1]), nodeValues [2]), nodeValues [3]),  
    nodeValues[4]), nodeValues [5]), nodeValues [6]), nodeValues [7]);
    
    npos1 = InverseSize * npos1;
    npos2 = InverseSize * npos2;

    vec4 r = texture(SourceTexture, npos1) + 0.5*(texture(SourceTexture, InverseSize * fragCoord) - texture(SourceTexture, npos2));

    FragColor = Dissipation * max(min(r, pMax), pMin);  
}


/*
void main()
{
    vec3 fragCoord = vec3(gl_FragCoord.xy, gLayer);
    float solid = texture(Obstacles, InverseSize * fragCoord).x;
    if (solid > 0) {
        FragColor = vec4(0);
        return;
    }

    vec3 u = texture(VelocityTexture, InverseSize * fragCoord).xyz;

    vec3 coord = InverseSize * (fragCoord - TimeStep * u);
    FragColor = Dissipation * texture(SourceTexture, coord);
}
*/