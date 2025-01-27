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
    vec3 lightDir = normalize(lightPos - fragPos);
    float lightIntensity = max(dot(lightDir, normal), 0.0);
    float outline = abs(normal.x) + abs(normal.y) + abs(normal.z);; // edge detection
    float thinkness = 0.1 + (1.0 - lightIntensity) * 0.5;

    float outlineMask = outline * thinkness;
    finalColour = vec4(0.0, 0.0, 0.0, outlineMask);
}