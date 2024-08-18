#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec2 test;
out vec3 texCoord;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    texCoord = vec3(0.0, aPos.xy);
    test = vec2(0.0, 1.0);
}
