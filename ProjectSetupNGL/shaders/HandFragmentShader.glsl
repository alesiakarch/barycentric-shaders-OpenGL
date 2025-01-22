#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;

// t_i parameters, end goal maps that i expect to combine
uniform sampler2D Handtex;
uniform vec3 diffuse;
uniform vec3 shadow;
uniform vec3 edges;
uniform vec3 reflections;
uniform vec3 specular;

// do i need to add lights to compute?

// calculate the light over the final maps?
void main()
{
    finalColour = texture(Handtex, uv);
}