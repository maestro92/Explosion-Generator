#version 330
out vec3 FragColor;
uniform sampler3D Velocity;
uniform sampler3D Temperature;
uniform sampler3D Density;
uniform float AmbientTemperature;
uniform float TimeStep;
uniform float Sigma;
uniform float Kappa;

in float gLayer;

void main()
{
    ivec3 TC = ivec3(gl_FragCoord.xy, gLayer);
    float T = texelFetch(Temperature, TC, 0).r;
    vec3 V = texelFetch(Velocity, TC, 0).xyz;

    FragColor = V;

    if (T > AmbientTemperature) {
        float D = texelFetch(Density, TC, 0).x;
        FragColor += (TimeStep * (T - AmbientTemperature) * Sigma - D * Kappa ) * vec3(0, -1, 0);
    }
}
