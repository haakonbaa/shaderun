#version 330 core
in vec3 texCoord;
out vec4 FragColor;
void main()
{
    float r = (texCoord.x + 1.0) / 2.0;
    float g = (texCoord.y + 1.0) / 2.0;
    FragColor = vec4(r, g, 1.0, 1.0);
}
