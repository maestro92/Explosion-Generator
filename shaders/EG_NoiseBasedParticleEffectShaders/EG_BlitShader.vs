#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;

out vec2 outUV;

uniform float gScrollOffsetTime
uniform float gDepth;

void main()
{
	outUV = UV;
	outUV.s = outUV.s * 0.125 * gScrollOffsetTime;

	gl_Position = vec4(Position, 1.0);
	gl_Position.z = gDepth;
}