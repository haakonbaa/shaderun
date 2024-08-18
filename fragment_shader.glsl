#version 330 core
in vec3 texCoord;
in vec2 test;
out vec4 FragColor;

uniform float uTime;

void main()
{
    float r = (texCoord.x + 1.0) / 2.0;
    float g = (texCoord.y + 1.0) / 2.0;
    FragColor = vec4(mod(uTime,1), 0.0, 0.0, 1.0);
}
