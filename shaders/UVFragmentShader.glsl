#version 410 core

layout(location = 0) out vec4 finalColour;

in vec2 uv;

// pass down the mesh uv coordinates to the quad shader
void main()
{   

    finalColour = vec4(uv, 0.0f, 1.0f);

}