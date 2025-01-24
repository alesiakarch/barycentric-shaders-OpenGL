#version 410 core

layout(location = 0) out vec4 finalColour;

in vec2 uv;

// t_i parameters, end goal maps that i expect to combine
uniform sampler2D Diffusetex;
uniform sampler2D Shadowtex;

// calculate the light over the final maps?
void main()
{
    finalColour = texture(Diffusetex, uv);
    //finalColour = texture(Shadowtex, uv);
}