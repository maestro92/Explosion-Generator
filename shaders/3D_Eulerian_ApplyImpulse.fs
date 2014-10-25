#version 330
out vec4 FragColor;

uniform vec3 Point;
uniform float Radius;
uniform vec3 FillColor;

in float gLayer;

void main()
{
    float d = distance(Point, vec3(gl_FragCoord.xy, gLayer));
    if (d < Radius) {
        float a = (Radius - d) * 0.5;
        a = min(a, 1.0);
        FragColor = vec4(FillColor, a);
    } else {
        FragColor = vec4(0);
    }
}