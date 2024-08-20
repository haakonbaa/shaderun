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
    vec2 pos = 1.5*texCoord.xy + vec2(0.0, 0.0);
    float max_iter = 200;
    // vary c
    float r = 1.0;
    float c_r = r*cos(uTime);
    float c_i = r*sin(uTime);

    float z_r = pos.x;
    float z_i = pos.y;
    float z_r_p = 0;
    float z_i_p = 0;
    float i = 0;
    for (i = 0; i < max_iter; i++) {
        // z = z^2 + c
        z_r_p = z_r;
        z_i_p = z_i;
        z_r = z_r_p * z_r_p - z_i_p * z_i_p + c_r;
        z_i = 2 * z_r_p * z_i_p + c_i;
        if (z_r * z_r + z_i * z_i > 4) {
            break;
        }
    }

    float d = i / max_iter;
    d = pow(d,0.3);
    vec3 col = d * palette(d);
    fragColor = vec4(col, 1.0);
}
