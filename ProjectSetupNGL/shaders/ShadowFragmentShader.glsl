#version 410 core

layout(location = 0) out vec4 finalColour;


in vec3 normal;
in vec2 uv;
in vec3 baryCoords;
in vec4 fragPos;

// texture maps that are going to be blended
uniform sampler2D ShadowTexture1;

// uniform vec3 lightPos;
// uniform vec4 viewPos;


// calculate the light over the final maps?
void main()
{   
    finalColour = texture(ShadowTexture1, uv); // just the texture
}