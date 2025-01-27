#version 410 core

layout(location = 0) out vec4 finalColour;

in vec2 uv;

void main()
{   

    finalColour = vec4(uv, 0.0, 1.0);

}