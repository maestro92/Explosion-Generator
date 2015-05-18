#version 330
out float FragColor;

uniform sampler3D Velocity;
uniform sampler3D Obstacles;
uniform float HalfInverseCellSize;

in float gLayer;

void main()
{
    ivec3 T = ivec3(gl_FragCoord.xy, gLayer);

    // Find neighboring velocities:
    vec3 vN = texelFetchOffset(Velocity, T, 0, ivec3(0, 1, 0)).xyz;
    vec3 vS = texelFetchOffset(Velocity, T, 0, ivec3(0, -1, 0)).xyz;
    vec3 vE = texelFetchOffset(Velocity, T, 0, ivec3(1, 0, 0)).xyz;
    vec3 vW = texelFetchOffset(Velocity, T, 0, ivec3(-1, 0, 0)).xyz;
    vec3 vU = texelFetchOffset(Velocity, T, 0, ivec3(0, 0, 1)).xyz;
    vec3 vD = texelFetchOffset(Velocity, T, 0, ivec3(0, 0, -1)).xyz;

    // Find neighboring obstacles:
    vec3 oN = texelFetchOffset(Obstacles, T, 0, ivec3(0, 1, 0)).xyz;
    vec3 oS = texelFetchOffset(Obstacles, T, 0, ivec3(0, -1, 0)).xyz;
    vec3 oE = texelFetchOffset(Obstacles, T, 0, ivec3(1, 0, 0)).xyz;
    vec3 oW = texelFetchOffset(Obstacles, T, 0, ivec3(-1, 0, 0)).xyz;
    vec3 oU = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, 1)).xyz;
    vec3 oD = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, -1)).xyz;

    // Use obstacle velocities for solid cells:
    if (oN.x > 0) vN = oN.yzx;
    if (oS.x > 0) vS = oS.yzx;
    if (oE.x > 0) vE = oE.yzx;
    if (oW.x > 0) vW = oW.yzx;
    if (oU.x > 0) vU = oU.yzx;
    if (oD.x > 0) vD = oD.yzx;

    FragColor = HalfInverseCellSize * (vE.x - vW.x + vN.y - vS.y + vU.z - vD.z);
}