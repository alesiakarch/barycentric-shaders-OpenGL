#version 410 core

layout(location = 0) out vec4 finalColour;

in vec2 uv;

// t_i parameters, end goal maps that i expect to combine
uniform sampler2D Diffusetex;
uniform sampler2D Shadowtex;
uniform sampler2D Speculartex;

// calculate the light over the final maps?
void main()
{
    if (texture(Shadowtex, uv).a == 0.0)
    {
        finalColour = texture(Diffusetex, uv);
    }
    else if (texture(Speculartex, uv).a > 0.0)
    {
        finalColour = texture(Speculartex, uv);
    }
    else
    {
        finalColour = texture(Shadowtex, uv);
    }
    finalColour = (0.33 * texture(Diffusetex, uv)) + (0.33 * texture(Shadowtex, uv) + (0.33 * texture(Speculartex, uv)));
    //finalColour = texture(Speculartex, uv);
}