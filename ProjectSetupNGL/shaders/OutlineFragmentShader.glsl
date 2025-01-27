#version 410 core

layout(location = 0) out vec4 finalColour;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

// texture maps that are going to be blended
uniform vec3 lightPos;
uniform vec3 viewPos;


// calculate the light over the final maps?
void main()
{   
    float outline = 0.0;
    finalColour = vec4(0.0, 0.0, 0.0, outline);
}