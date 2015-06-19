#version 330

layout (location = 0) in vec3 Position;
layout (location = 3) in float BirthTime;
layout (location = 4) in vec3 Velocity;

uniform mat4 u_modelViewProjMat;
uniform float u_time;
uniform float u_fadeRate;

out vec3 vg_position;
out vec3 vg_velocity;
out float vg_alpha;

void main()
{
	gl_Position = u_modelViewProjMat * vec4(Position,1.0);

	vg_position = Position;
	vg_velocity = normalize(Velocity);
	vg_alpha = max(0.0, 1.0-(u_time - BirthTime) * u_fadeRate);
}