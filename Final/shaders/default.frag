#version 330 core

out vec4 FragColor;

uniform float time;

void main()
{
    vec3 color = vec3(sin(time), cos(time), 0.5);
    FragColor = vec4(color, 1.0);
}