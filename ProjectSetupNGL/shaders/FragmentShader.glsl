#version 410 core

layout(location = 0) out vec4 finalColour;
in vec3 fragColour;

void main()
{
    finalColour = vec4(fragColour.rgb, 1);
}