#version 330


layout (location = 0) in vec3 Position;
layout (location = 3) in float BirthTime;
layout (location = 4) in vec3 Velocity;


out vec3 vPosition;
out float vBirthTime;
out vec3 vVelocity;

uniform sampler3D Sampler;
uniform vec3 Size;
uniform vec3 Extent;
uniform float Time;
// uniform float TimeStep = 5.0;
uniform float TimeStep = 0.01;
uniform float InitialBand = 0.1;
uniform float SeedRadius = 0.25;
uniform float PlumeCeiling = 20.0;
uniform float PlumeBase = -3;

const float TwoPi = 6.28318530718;
const float UINT_MAX = 4294967295.0;

uint randhash(uint seed)
{
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return i;
}

float randhashf(uint seed, float b)
{
    return float(b * randhash(seed)) / UINT_MAX;
}

vec3 SampleVelocity(vec3 p)
{
    vec3 tc;
    tc.x = (p.x + Extent.x) / (2 * Extent.x);
    tc.y = (p.y + Extent.y) / (2 * Extent.y);
    tc.z = (p.z + Extent.z) / (2 * Extent.z);
    return texture(Sampler, tc).xyz;
}

void main()
{
    vPosition = Position;
    vBirthTime = BirthTime;

    // Seed a new particle as soon as an old one dies:
    if (BirthTime == 0.0 || Position.y > PlumeCeiling) 
	{
        uint seed = uint(Time * 1000.0) + uint(gl_VertexID);
        float theta = randhashf(seed++, TwoPi);
        float r = randhashf(seed++, SeedRadius);
        float y = randhashf(seed++, InitialBand);
        vPosition.x = r * cos(theta);
        vPosition.y = y;
//        vPosition.y = -5.0;

        vPosition.z = r * sin(theta);
        vBirthTime = 1.0;
    }

    // Move the particles forward using a half-step to reduce numerical issues:
    vVelocity = SampleVelocity(Position);
    vec3 midx = Position + 0.5f * TimeStep * vVelocity;
    vVelocity = SampleVelocity(midx);
    vPosition += TimeStep * vVelocity;
}
#if 0
layout (location = 0) in vec3 Position;
layout (location = 3) in float BirthTime;
layout (location = 4) in vec3 Velocity;

out vec3 v_position;
out float v_birthTime;
out vec3 v_velocity;

uniform sampler3D Sampler;
uniform vec3 Size;
uniform vec3 Extent;
uniform float Time;
uniform float TimeStep = 5.0;
uniform float InitialBand = 0.1;
uniform float SeedRadius = 0.25;
uniform float PlumeCeiling = 3.0;
uniform float PlumeBase = -3;

const float TWO_PI = 6.28318530718;
const float UINT_MAX = 4294967295.0;
/*
uniform sampler3D velocityTexture;
uniform vec3 size;
uniform vec3 extent;
uniform float time;
uniform float timeStep = 5.0;
uniform float initialBand = 0.1;
uniform float seedRadius = 0.25;
uniform float plumeCeiling = 3.0;
uniform float plumeBase = -3;

const float TWO_PI = 6.283185;
const float UINT_MAX = 4294967295.0;
*/

uint randhash(uint seed)
{
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return i;
}

float randhashf(uint seed, float b)
{
    return float(b * randhash(seed)) / UINT_MAX;
}

vec3 sampleVelocity(vec3 p)
{
	vec3 tc;
	tc.x = (p.x + Extent.x) / (2 * Extent.x);
	tc.y = (p.y + Extent.y) / (2 * Extent.y);
	tc.z = (p.z + Extent.z) / (2 * Extent.z);
	return texture(Sampler, tc).xyz;
}



void main()
{
	v_position = Position;
	v_birthTime = BirthTime;

	// Seed a new particle as soon as an old one dies
	if(BirthTime == 0.0 || Position.y > PlumeCeiling)
	{
		uint seed = uint(Time * 1000.0) + uint(gl_VertexID);
		float theta = randhashf(seed++, TWO_PI);
		float r = randhashf(seed++, SeedRadius);
		float y = randhashf(seed++, InitialBand);
		v_position.x = r * cos(theta);
		v_position.y = y;
		v_position.z = r * sin(theta);
		v_birthTime = Time;
	}

	v_velocity = sampleVelocity(Position);
	vec3 midx = Position + 0.5f * TimeStep * v_velocity;
	v_velocity = sampleVelocity(midx);
	v_position += TimeStep * v_velocity;

}
#endif