#version 410 core

layout(location = 0) out vec4 finalColour;

in vec2 uv;

uniform sampler2D UVTexture;

// potentially add outline detection
// control images to be assigned to each weight
uniform sampler2D blendTexture;

void main()
{
    finalColour = vec4(texture(blendTexture, uv).rgb, 1.0);
}