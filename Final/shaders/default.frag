#version 330 core

out vec4 FragColor;

void main()
{
    float triangleSize = 1;
    int numTrianglesX = 100;
    int numTrianglesY = 100;

    vec2 screenSize = vec2(800.0, 600.0);
    vec2 triangleStep = screenSize / vec2(numTrianglesX, numTrianglesY);

    ivec2 triangleIndex = ivec2(gl_FragCoord.xy / triangleStep);

    vec3 color = vec3(mod(triangleIndex.x + triangleIndex.y, 2));

    FragColor = vec4(color, 1.0);
}