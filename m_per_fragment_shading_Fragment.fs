#version 120

in vec3 position;
in vec3 normal;

// if you want the light to be in the scene, in stead of on the player
// u mulitply it by the ModelView Matrix
uniform vec3 lightPos;


uniform vec3 mambient;
uniform vec3 mdiffuse;
uniform vec3 mspecular;
uniform float shininess;

uniform vec3 lambient;
uniform vec3 ldiffuse;
uniform vec3 lspecular;
//#version 330

out vec4 FragColor;
void main()
{

	float dist=length(position-lightPos);	//distance from light-source to surface
//	float att=1.0/(1.0+0.1*dist+0.01*dist*dist);	//attenuation (constant,linear,quadric)
	float att=1.0/(1.0+0.1*dist+0.001*dist*dist);	//attenuation (constant,linear,quadric)


	// ambient
	vec3 ambient = mambient * lambient;
	
	// diffuse
	vec3 surf2light = normalize(lightPos - position);
	vec3 norm = normalize(normal); // to fix interpolation
	float dcont=max(0.0,dot(norm,surf2light));
	vec3 diffuse = dcont * mdiffuse * ldiffuse;

	// specular
	vec3 surf2view = normalize (-position);  // the camera(the viewer) is always 0,0,0
	vec3 reflection = reflect(-surf2light, norm);
	float scont = pow(max(0.0, dot(surf2view, reflection)), shininess);

	vec3 specular = scont * lspecular * mspecular;

	FragColor = vec4((ambient + diffuse + specular)*att, 1.0);
//	FragColor = vec4(1.0,0.0,0.0,1.0);
}