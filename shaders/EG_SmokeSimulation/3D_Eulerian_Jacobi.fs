#version 330
out vec4 FragColor;

uniform sampler3D Pressure;
uniform sampler3D Divergence;
uniform sampler3D Obstacles;

uniform float Alpha;
uniform float InverseBeta;

in float gLayer;

void main()
{
    ivec3 T = ivec3(gl_FragCoord.xy, gLayer);

    // Find neighboring pressure:
    vec4 pN = texelFetchOffset(Pressure, T, 0, ivec3(0, 1, 0));
    vec4 pS = texelFetchOffset(Pressure, T, 0, ivec3(0, -1, 0));
    vec4 pE = texelFetchOffset(Pressure, T, 0, ivec3(1, 0, 0));
    vec4 pW = texelFetchOffset(Pressure, T, 0, ivec3(-1, 0, 0));
    vec4 pU = texelFetchOffset(Pressure, T, 0, ivec3(0, 0, 1));
    vec4 pD = texelFetchOffset(Pressure, T, 0, ivec3(0, 0, -1));
    vec4 pC = texelFetch(Pressure, T, 0);

    // Find neighboring obstacles:
    vec3 oN = texelFetchOffset(Obstacles, T, 0, ivec3(0, 1, 0)).xyz;
    vec3 oS = texelFetchOffset(Obstacles, T, 0, ivec3(0, -1, 0)).xyz;
    vec3 oE = texelFetchOffset(Obstacles, T, 0, ivec3(1, 0, 0)).xyz;
    vec3 oW = texelFetchOffset(Obstacles, T, 0, ivec3(-1, 0, 0)).xyz;
    vec3 oU = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, 1)).xyz;
    vec3 oD = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, -1)).xyz;

    // Use center pressure for solid cells:
    if (oN.x > 0) pN = pC;
    if (oS.x > 0) pS = pC;
    if (oE.x > 0) pE = pC;
    if (oW.x > 0) pW = pC;
    if (oU.x > 0) pU = pC;
    if (oD.x > 0) pD = pC;

    vec4 bC = texelFetch(Divergence, T, 0);
    FragColor = (pW + pE + pS + pN + pU + pD + Alpha * bC) * InverseBeta;
}