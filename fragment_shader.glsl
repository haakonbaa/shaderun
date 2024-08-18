#version 330 core
in vec3 texCoord;
out vec4 fragColor;
uniform float uTime;

vec3 palette( in float t)
{
    vec3 a = vec3(0.5,0.5,0.5);
    vec3 b = vec3(0.5,0.5,0.5);
    vec3 c = vec3(1.0,1.0,1.0);
    vec3 d = vec3(0.0,0.33,0.67);
    return a + b*cos( 6.28318*(c*t+d) );
}

void main()
{
    vec2 pos = texCoord.xy;
    vec3 finalColor = vec3(0.0);

    float d;
    vec3 col;
    for (float i = 0.0; i < 3.0; i++) {
        pos = 1.5*(fract(1.5*pos)-0.5);
        col = palette(length(texCoord.xy) + i*0.5 + 0.5*uTime);
        d = length(pos)*exp(-length(texCoord.xy));
        d = sin(14.0*d + 3.0*uTime);
        d = 0.5*(d + 1.0);
        d = 0.1/d;
        d = pow(d, 1.2);
        finalColor += d*col/(4.0*i+1.0);
    }
    fragColor = vec4(finalColor, 1.0);
}
